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


#ifndef BOOK_H
#define BOOK_H
#include <string>
#include <opencv2/core/core.hpp>
#include <opencv2/nonfree/features2d.hpp>


namespace BR {

/**
 * @class Class represents book.
 */
  class Book
  {

  public:
    /**
     * Default contructor.
     */
    Book();
    /**
     * Contructor with settings book attributes.
     * @param isbn Book isbn.
     * @param title Book title.
     * @param author Book author.
     * @param image File name with image with book.
     */
    Book(std::string isbn, std::string title, std::string author, std::string image);
    
    /**
     * Get book image.
     * @return cv::Mat with book image.
     */
    cv::Mat getImage() const;
    
    /**
     * Get human readable book attributes.
     * @return std::string with human readable book attributes.
     */
    std::string toString() const;
    /**
     * Telss if book is empty.
     * @return true if book don't have attributes, false otherwise.
     */
    operator bool () const;
    /**
     * Telss if book is empty.
     * @return true if book don't have attributes, false otherwise.
     */
    bool empty() const;
  private:
    std::string isbn, title, author;
    cv::Mat image;

    void storeAndProcessImage(std::string filename);

    //for surf
    cv::vector<cv::KeyPoint> keypoints;
    cv::Mat descriptors;
    friend class Database;
    friend class Recognizer;

    //statics
    static cv::SURF SURF;
    //static cv::BFMatcher matcher;
  };
  
}

#endif // BOOK_H
