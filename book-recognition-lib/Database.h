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


#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <list>
#include <tinyxml.h>
#include <opencv2/core/core.hpp>
#include "Book.h"

namespace BR {
  class Database
  {
  public:
    /**
     * Default constructor.
     * @param as 
     */
    Database(bool as = false);
    /**
     * Contructore with filename with database to open.
     * @param filename Database file name.
     * @param as
     */
    Database(std::string filename, bool as = false);
    ~Database();
    
    /**
     * Load database from file.
     * @param filename Database file name.
     */
    void load(std::string filename);    
    /**
     * Save database to file.
     * @param filename Database file name.
     */
    void save(std::string filename);
    /**
     * Add book to database.
     * @param book Book to add.
     */
    void addBook(Book * book);
    
    /**
     * Find book in database.
     * @param image Image with book
     * @param H 
     * @return Pointer to found book. Do not remove it.
     */
    Book* find(cv::Mat image, cv::Mat & H);

    /**
     * Tells if database was saved.
     * @return true if was, false otherwise.
     */
    bool isSaved() const;
    /**
     * Clear database.
     */
    void clear();
    /**
     * Add book to database from image.
     * @param image Image with book
     * @param Points with book corners.
     * @param title Book title.
     * @param author Book author.
     * @param isbn Book isbn.
     */
    void fixAndAdd(cv::Mat& image, const std::vector< cv::Point2f > points, std::string title, std::string author, std::string isbn);
  private:    
    void initialize(bool as);
    bool autosave;
    bool saved;

    std::string filename;
    std::vector<Book*> books;

    

    //for recognizing
    static cv::Ptr<cv::DescriptorMatcher> matcher; 
    cv::vector<cv::KeyPoint> scene_keypoints;
    cv::Mat scene_descriptors;
  };

}

#endif // DATABASE_H
