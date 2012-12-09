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

#include <opencv2/highgui/highgui.hpp>

cv::Ptr<cv::DescriptorMatcher> BR::Database::matcher = cv::DescriptorMatcher::create("BruteForce");

BR::Database::Database(bool as)
{
  autosave = as;
}

BR::Database::Database(std::string filename, bool as)
{
  autosave = as;
  this->filename = filename;
  load(filename);
}

void BR::Database::load(std::string filename)
{
  //Parse XML and load books
}

void BR::Database::save(std::string filename)
{
  //Save database to XML file
}

bool BR::Database::find(cv::Mat image, BR::Book ** out)
{
  //TODO SURF magic
  //get descriptors like in Book::storeAndProcessImage use some comparator some like this
   //-- Step 3: Matching descriptor vectors with a brute force matcher
  /*BruteForceMatcher< L2<float> > matcher;
  std::vector< DMatch > matches;
  matcher.match( descriptors_1, descriptors_2, matches );
  full exapmle http://docs.opencv.org/doc/tutorials/features2d/feature_description/feature_description.html#feature-description
  */
  cv::vector<cv::KeyPoint> keypoints;
  cv::Mat descriptors;
  Book::SURF(image, cv::Mat(), keypoints, descriptors);
  uint max = 0;
  matcher = cv::DescriptorMatcher::create("BruteForce");
  for(auto book = books.begin(); book != books.end(); ++book) {
    std::vector< cv::DMatch > matches;  
    //std::vector< std::vector< cv::DMatch > > matches;  
    matcher->match((*book)->descriptors,descriptors, matches);
    std::cout << (*book)->toString() << ": " << (*book)->keypoints.size() << ", " << matches.size() << '\n';
    //TODO: When it match or not?
    //if (matches.size() > max) {
    //  max = matches.size();
    //  *out = *book;
    //}
  }
  //std::cout << out->toString() << ": " << max << '\n';
  
  
  //DEBUG
  //cv::Mat tmp;
  //cv::drawMatches(books.front()->image, books.front()->keypoints, books.back()->image, books.back()->keypoints, matches, tmp);
  //cv::imwrite("file.bmp", tmp);
  //DEBUG
  return true; 
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
}






