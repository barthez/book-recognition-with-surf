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


#include "Book.h"
#include <opencv2/highgui/highgui.hpp>

cv::SURF BR::Book::SURF = cv::SURF(400); //hessianThreshold ?

BR::Book::Book()
{

}

BR::Book::Book(std::string isbn, std::string title, std::string author, std::string image): 
  isbn(isbn), title(title), author(author)
{
  storeAndProcessImage(image);
}

BR::Book::Book(std::string isbn, std::string title, std::string author, cv::Mat image): 
  isbn(isbn), title(title), author(author)
{
  this->image = image.clone();
  processImage();
}


bool BR::Book::empty() const
{
  return isbn.empty();
}

BR::Book::operator bool () const
{
  return empty();
}

cv::Mat BR::Book::getImage() const
{
  return image;
}

std::string BR::Book::toString() const {
  std::stringstream ss;
  ss << "(" << isbn << ") '" << title << "' - " << author;
  return ss.str();
}

void BR::Book::storeAndProcessImage (std::string filename)
{
  //this->filename = filename.substr(0,filename.length() - 3) + "xml";
  image = cv::imread(filename, 0); //loading grayscale image
  processImage();
 }
 
void BR::Book::processImage()
{
  SURF(image, cv::Mat(), keypoints, descriptors);
}
