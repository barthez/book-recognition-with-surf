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


#include "Recognizer.h"
#include "Exceptions.h"

#include <opencv2/imgproc/imgproc.hpp>

int BR::Recognizer::counter = 0;

void BR::Recognizer::initialize()
{
  id = BR::Recognizer::counter++;
  current_book = NULL;
  hold_flag = false;
}


BR::Recognizer::Recognizer()
{
  initialize();
}

BR::Recognizer::Recognizer(int camID)
{
  initialize();
  open(camID);
}

BR::Recognizer::Recognizer(std::string movie_filename)
{
  initialize();
  open(movie_filename);
}

bool BR::Recognizer::open(int camID)
{
  cam = true;
  cam_id = camID;
  return source.open(camID);
}

bool BR::Recognizer::open(std::string movie_filename)
{
  cam = false;
  filename = movie_filename;
  return source.open(movie_filename);
}

void BR::Recognizer::setDatabase(BR::Database * db)
{
  this->db = db;
}


const BR::Book * BR::Recognizer::getCurrentBook()
{
  return current_book;
}

const cv::Mat& BR::Recognizer::getCurrentFrame() const
{
  if (current_frame.empty()) throw new RecognizerException("Call next() before geting frame");
  return current_frame;
}
void BR::Recognizer::showCurrentFrame(bool show_book)
{
  cv::Mat output_frame = getCurrentFrame().clone();
  if (show_book && current_book) {
    //cv::putText(output_frame, current_book->toString(),cv::Point(20,20),cv::FONT_HERSHEY_PLAIN, 1, cv::Scalar(0,255,0));
    
    std::vector< cv::Point2f > object_corners(4);
    std::vector< cv::Point2f > scene_corners(4);
    object_corners[0] = cv::Point(0, 0);
    object_corners[1] = cv::Point(current_book->image.cols, 0);
    object_corners[2] = cv::Point(current_book->image.cols, current_book->image.rows);
    object_corners[3] = cv::Point(0, current_book->image.rows);
    
    cv::perspectiveTransform( object_corners, scene_corners, homography);
    for(int i=0; i<4; ++i) {
      //std::cout << "Point(" << scene_corners[i].x << ", " << scene_corners[i].y << ")\n";
      cv::line(output_frame, scene_corners[i], scene_corners[ (i+1) %4], cv::Scalar(0, 255, 0), 4);
    }
  }
  cv::imshow(windowID(), output_frame);
}

cv::Mat BR::Recognizer::getCurrentFrame(bool show_book)
{
  last_frame = getCurrentFrame().clone();
  if (show_book && current_book) {
    cv::putText(last_frame, current_book->toString(),cv::Point(20,20),cv::FONT_HERSHEY_PLAIN, 1, cv::Scalar(0,255,0));
    
    std::vector< cv::Point2f > object_corners(4);
    std::vector< cv::Point2f > scene_corners(4);
    object_corners[0] = cv::Point(0, 0);
    object_corners[1] = cv::Point(current_book->image.cols, 0);
    object_corners[2] = cv::Point(current_book->image.cols, current_book->image.rows);
    object_corners[3] = cv::Point(0, current_book->image.rows);
    
    cv::perspectiveTransform( object_corners, scene_corners, homography);
    for(int i=0; i<4; ++i) {
      //std::cout << "Point(" << scene_corners[i].x << ", " << scene_corners[i].y << ")\n";
      cv::line(last_frame, scene_corners[i], scene_corners[ (i+1) %4], cv::Scalar(0, 255, 0), 4);
    }
  }
  return last_frame;
}

bool BR::Recognizer::next(bool find)
{
  if (! source.isOpened() ) throw new RecognizerException("Select source before running");
  if (hold_flag) return true;
  
  if (source.read(current_frame)) {
      cv::Mat tmp_img;
      cv::cvtColor(current_frame, tmp_img, cv::COLOR_BGR2GRAY);
      if (find) {
        current_book = db->find(tmp_img, homography);
        
        hold();
      }
      return true;
    }
    return false;
}

bool BR::Recognizer::hold(bool h)
{
  bool old = hold_flag;
  hold_flag = h;
  if (!hold_flag) {
    current_book = NULL;
    homography = cv::Mat();
  }
  return old;
}


std::string BR::Recognizer::windowID()
{
  if (window_id.empty()) {
    std::stringstream ss;
    ss << "Recognizer " << id << " (";
    if (cam) {
      ss << "Camera " << cam_id;
    } else {
      ss << "Movie " << filename;
    }
    ss << ")";
    window_id = ss.str();
  }
  return window_id;
}









