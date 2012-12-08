#include <iostream>

#include <Recognizer.h>
#include <Exceptions.h>

using namespace BR;
//!cv::initModule_features2d() || //
int main(int argc, char **argv)
{
  
  Book book1("1", "1", "1", "data/DSC03425.JPG");
  std::cout << "1";
  Book book2("1", "1", "1", "data/DSC03425.JPG");
  std::cout << "2";
 /* Book book3("1", "1", "1", "data/DSC03422.JPG");
  std::cout << "3";
  Book book4("1", "1", "1", "data/DSC03423.JPG");
  std::cout << "4";
  Book book5("1", "1", "1", "data/DSC03424.JPG");
  std::cout << "5\n";
  */
  Database db;
  db.addBook(&book1);
  db.addBook(&book2);
  /*db.addBook(&book3);
  db.addBook(&book4);
  db.addBook(&book5);
  */
  try
  {
    Recognizer r("data/MOV03427.MPG");
    r.setDatabase(db);
    r.next();
    r.showCurrentFrame(true);

    cv::waitKey(2000);
  }
  catch (Exception e)
  {
    std::cout << e.getFullMessage() << "\n";
  }
  return 0;
}
