/*
    Copyright (c) 2012, <copyright holder> <email>
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:
        * Redistributions of source code must retain the above copyright
        notice, this list of conditions and the following disclaimer.
        * Redistributions in binary form must reproduce the above copyright
        notice, this list of conditions and the following disclaimer in the
        documentation and/or other materials provided with the distribution.
        * Neither the name of the <organization> nor the
        names of its contributors may be used to endorse or promote products
        derived from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY <copyright holder> <email> ''AS IS'' AND ANY
    EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL <copyright holder> <email> BE LIABLE FOR ANY
    DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
    (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
    ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/


#include "Database.h"
#include "Exceptions.h"

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <tinyxml.h>

cv::Ptr<cv::DescriptorMatcher> BR::Database::matcher = cv::DescriptorMatcher::create("FlannBased");

void BR::Database::initialize(bool as)
{
  autosave = as;
  saved = true;
}


BR::Database::Database(bool as)
{
  initialize(as);
}

BR::Database::Database(std::string filename, bool as)
{
  initialize(as);
  load(filename);
}

void BR::Database::load(std::string filename)
{
  this->filename = filename;
  size_t dir_pos = filename.find_last_of("\\/");
  std::string filename_prefix = dir_pos == std::string::npos ? "" : filename.substr(0, dir_pos + 1);
  std::string book_image_filename;
  std::string book_info_filename;

  TiXmlDocument document(filename.c_str());
  if (!document.LoadFile())
  {
    throw DatabaseException("unable to load file: " + filename);
  }
  //get books
  TiXmlElement * element = document.FirstChildElement("book");
  do
  {
    Book * book = new Book();
    //read XML
    TiXmlElement * child = element->FirstChildElement("isbn");
    book->isbn = child->GetText();
    child = child->NextSiblingElement("title");
    book->title = child->GetText();
    child = child->NextSiblingElement("author");
    book->author = child->GetText();
    child = child->NextSiblingElement("image_filename");
    book_image_filename = filename_prefix +  child->GetText();
    child = child->NextSiblingElement("image_info_filename");
    if ( child == NULL ) throw new DatabaseException("Unknown file stucture");
    book_info_filename = filename_prefix + child->GetText();

    //load structures
    cv::FileStorage fs(book_info_filename, cv::FileStorage::READ);
    cv::read(fs["keypoints"], book->keypoints);
    fs["descriptors"] >> book->descriptors;
    fs.release();

    //load image
    book->image = cv::imread(book_image_filename);
    
    books.push_back(book);
  } while(element = element->NextSiblingElement("book"));
}

void BR::Database::save(std::string filename)
{
  //find 
  size_t dir_pos = filename.find_last_of("\\/");
  std::string filename_prefix = dir_pos == std::string::npos ? "" : filename.substr(0, dir_pos + 1);
  std::string db_name = dir_pos == std::string::npos ? filename : filename.substr(dir_pos + 1, filename.length() - dir_pos - 5);

  TiXmlDocument document(filename.c_str());
  document.LinkEndChild(new TiXmlDeclaration("1.0", "", "" ));
  std::for_each(books.begin(), books.end(), [this, &document, &filename_prefix, &filename, &db_name](Book * book)
  {
    //set image filename
    std::string book_info_filename = book->isbn + book->author + book->title + db_name + ".xml";
    std::string book_image_filename = book->isbn + book->author + book->title + db_name + ".jpg";
    std::string book_info_filename_store = filename_prefix + book_info_filename;
    std::string book_image_filename_store = filename_prefix + book_image_filename;

    //create XML
    TiXmlElement * book_xml = new TiXmlElement("book");
    TiXmlElement * isbn = new TiXmlElement("isbn");
    isbn->LinkEndChild(new TiXmlText(book->isbn.c_str()));
    TiXmlElement * title = new TiXmlElement("title");
    title->LinkEndChild(new TiXmlText(book->title.c_str()));
    TiXmlElement * author = new TiXmlElement("author");
    author->LinkEndChild(new TiXmlText(book->author.c_str()));
    TiXmlElement * image_filename = new TiXmlElement("image_filename");
    image_filename->LinkEndChild(new TiXmlText(book_image_filename.c_str()));
    TiXmlElement * image_info_filename = new TiXmlElement("image_info_filename");
    image_info_filename->LinkEndChild(new TiXmlText(book_info_filename.c_str()));

    book_xml->LinkEndChild(isbn);
    book_xml->LinkEndChild(title);
    book_xml->LinkEndChild(author);
    book_xml->LinkEndChild(image_filename);
    book_xml->LinkEndChild(image_info_filename);
    document.LinkEndChild(book_xml);

    //store image
    cv::imwrite(book_image_filename_store, book->image);

    //store structures
    cv::FileStorage fs(book_info_filename_store, cv::FileStorage::WRITE);
    cv::write(fs, "keypoints", book->keypoints);
    fs << "descriptors" << book->descriptors;
    fs.release();
  });
  document.SaveFile();
  saved = true;
}

BR::Book * BR::Database::find(cv::Mat image, cv::Mat& H)
{
  double min_dist, threshold, global_minimum;
  std::vector< cv::DMatch > matches_of_best;
  //std::vector< std::vector< cv::DMatch > > matches_list;
  global_minimum = 100;
  Book * best_book = NULL;
  
  Book::SURF(image, cv::Mat(), scene_keypoints, scene_descriptors);
  
  for(auto book = books.begin(); book != books.end(); ++book) {
    std::vector< cv::DMatch > matches, good_matches;  
    matcher->match((*book)->descriptors, scene_descriptors, matches);
    good_matches = std::vector< cv::DMatch >(matches.size());
    auto result = std::minmax_element(matches.begin(), 
                                              matches.end(), 
                                              [](const cv::DMatch &a, const cv::DMatch &b) -> bool {return a.distance < b.distance;});
    
    min_dist = result.first->distance;
    
    //matches_list.push_back(matches);
    
    if (global_minimum > min_dist) {
       best_book = *book;
       matches_of_best = matches;
       global_minimum = min_dist;
    }
    
  }
  
  
  threshold = global_minimum*3;
  std::vector< cv::DMatch > good_matches;  
  
  good_matches = std::vector< cv::DMatch >(matches_of_best.size());
  
  auto last_good = std::copy_if(matches_of_best.begin(), 
                      matches_of_best.end(), 
                      good_matches.begin(), 
                      [&threshold](const cv::DMatch &a) -> bool { return a.distance < threshold; });
  
  good_matches.resize(last_good - good_matches.begin());
  
  std::vector<cv::Point2f> object, scene;
  for(auto it=good_matches.begin(); it != good_matches.end(); ++it) {
     object.push_back( best_book->keypoints[it->queryIdx].pt );
     scene.push_back( scene_keypoints[it->trainIdx].pt );
  }
  
  H = cv::findHomography(object, scene, CV_RANSAC);
  
  //TODO: Find best match by size (area) of affilite transformation of book cover. Bigger area -> better match (?)
  
  return best_book; 
}


BR::Database::~Database()
{
  if (autosave) {
    save(filename);
  }
}

void BR::Database::addBook(Book * book)
{
  books.push_back(book);
  saved = false;
}

bool BR::Database::isSaved() const
{
  return saved;
}


void BR::Database::clear()
{
  for (Book *b : books)
  {
    delete b;
  }
  books.clear();
}

void BR::Database::fixAndAdd(cv::Mat & image, cv::vector<cv::Point2f>, std::string title, std::string author, std::string isbn)
{
}

