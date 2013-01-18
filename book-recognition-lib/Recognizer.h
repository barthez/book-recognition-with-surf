/*
    Copyright (c) 2012, Bartłomiej Bułat <bartek.bulat@gmail.com>
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


#ifndef RECOGNIZER_H
#define RECOGNIZER_H

#include <string>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "Book.h"
#include "Database.h"

namespace BR {

/**
 * @class Class which do job connected with recognition.
 */
  class Recognizer
  {
  public:
    /**
     * Default constructor.
     */
    Recognizer();
    /**
     * Contructor opens camera specified by camID.
     * @param camId Camera id 
     */
    Recognizer(int camID);
    /**
     * Contructor opens video file.
     * @param movie_filename Video file name.
     */
    Recognizer(std::string movie_filename);
    
    /**
     * Open camera.
     * @param camID Camera id.
     * @return true if succeded, false otherwise.
     */
    bool open(int camID);
    /**
     * Open video file.
     * @param movie_filename Video file name.
     * @return true if succeded, false otherwise.
     */
    bool open(std::string movie_filename);
    
    /**
     * Set database with books.
     * @param db Pointer to BR::Database with books.
     */
    void setDatabase(Database * db);
    
    const cv::Mat & getCurrentFrame() const;
    /**
     * Shows current frame.
     * @param show_book If on image book will be shown.
     */
    void showCurrentFrame(bool show_book = true);
    /**
     * Get current frame from input.
     * @param show_book If on image book will be shown.
     * @return cv::Mat with current frame.
     */
    cv::Mat getCurrentFrame(bool show_book);
    /**
     * Get current recognized book
     * @return 
     */
    Book & getCurrentBook();
    /**
     * 
     * @param find
     * @return 
     */
    bool next(bool find = false);
    /**
     * 
     * @param h
     * @return 
     */
    bool hold(bool h = true);
    
  private:
    static int counter;
    int id;
    std::string window_id;
    std::string filename;
    int cam_id;
    bool cam;
    bool hold_flag;
    
    cv::VideoCapture source;
    cv::Mat current_frame, last_frame;
    Database *db;
    Book * current_book;
    
    cv::Mat homography;
    
    void drawBookContour();
    
    protected:
    std::string windowID();
    void initialize();
  };
  
}

#endif // RECOGNIZER_H
