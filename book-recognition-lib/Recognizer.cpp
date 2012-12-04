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

BR::Recognizer::Recognizer()
{

}

BR::Recognizer::Recognizer(int camID)
{
  open(camID);
}

BR::Recognizer::Recognizer(std::string movie_filename)
{
  open(movie_filename);
}

bool BR::Recognizer::open(int camID)
{
  return source.open(camID);
}

bool BR::Recognizer::open(std::string movie_filename)
{
  return source.open(movie_filename);
}

BR::Book BR::Recognizer::getCurrentBook()
{
  return current_book;
}

const cv::Mat& BR::Recognizer::getCurrentFrame() const
{
  return current_frame;
}

bool BR::Recognizer::next()
{
    if (source.read(current_frame)) {
      //TODO: Do the magic
      //cv::Mat tmp_img;
      //prepate_image(current_frame, tmp_img)
      //db.find(tmp_img, current_book);
      return true;
    }
    return false;
}










