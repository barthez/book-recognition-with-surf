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

BR::Recognizer::Recognizer()
{
  id = BR::Recognizer::counter++;
  current_book = NULL;
}

BR::Recognizer::Recognizer(int camID)
{
  id = BR::Recognizer::counter++;
  open(camID);
  current_book = NULL;
}

BR::Recognizer::Recognizer(std::string movie_filename)
{
  id = BR::Recognizer::counter++;
  open(movie_filename);
  current_book = NULL;
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

void BR::Recognizer::setDatabase(const BR::Database & db)
{
  this->db = db;
}


BR::Book & BR::Recognizer::getCurrentBook()
{
  return *current_book;
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
    //TODO: add to current frame info about book
    //std::cout << current_book->toString() << '\n';
    cv::putText(output_frame, current_book->toString(),cv::Point(20,20),cv::FONT_HERSHEY_PLAIN, 1, cv::Scalar(0,255,0));
  }
  cv::imshow(windowID(), output_frame);
}

bool BR::Recognizer::next(bool find)
{
  if (! source.isOpened() ) throw new RecognizerException("Select source before running");
  if (source.read(current_frame)) {
      //TODO: Do the magic
      cv::Mat tmp_img, canny_img, out;
      cv::cvtColor(current_frame, tmp_img, cv::COLOR_BGR2GRAY);
      //cv::cvtColor(tmp_img, out, cv::COLOR_GRAY2BGR);
      //current_frame = tmp_img;
      //cv::Mat mask = cv::Mat::zeros(out.rows, out.cols, CV_32FC1);
      //int w = current_frame.cols/2;
      //int h = current_frame.rows/2;
      //cv::rectangle(mask, cv::Point(w - 150, h-200), cv::Point(w+150, h+200), cv::Scalar(1,1,1), CV_FILLED);
      //tmp_img.convertTo(out, CV_32F, 1.0f/255.0f);
      //out = out.mul(mask);
      //out.convertTo(tmp_img, CV_8U, 255);
      
      if (find) db.find(tmp_img, &current_book);
      return true;
    }
    return false;
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









