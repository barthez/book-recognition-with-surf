#include <iostream>

#include "MainWindow.h"

#include <Recognizer.h>
#include <Exceptions.h>

#include <glibmm/thread.h>

using namespace BR;

int main(int argc, char **argv)
{
#if 1
  try
  {
  Glib::thread_init();
    gdk_threads_init();
  
  Gtk::Main brws(argc, argv);

  MainWindow win;

  Gtk::Main::run(win);
  }
  catch (...)
  {
    std::cout << "EXCEPTIOn\n";
  }
  gdk_threads_leave();
  return 0;

  /*
  std::cout << "Loading books ... ";
  Book book1("ISBN", "Kot w stanie czystym", "Terry Prachet", "data/pratchett.jpg");
  Book book2("ISBN", "O kotach", "Doris Lessing", "data/okotach.jpg");
  Book book3("ISBN", "Kot Simona", "Simon Tofield", "data/kot1.jpg");
  Book book4("ISBN", "Kot Simona za płotem", "Simon Tofield", "data/kot2.jpg");
  Book book5("ISBN", "Kot Simona i kociokwik", "Simon Tofield", "data/kot3.jpg");
  Book book6("ISBN", "Kot Simona kontra reszta świata", "Simon Tofield", "data/kot4.jpg");
  std::cout << "end\n";
  db.addBook(&book1);
  db.addBook(&book2);
  db.addBook(&book3);
  db.addBook(&book4);
  db.addBook(&book5);
  db.addBook(&book6);
  db.save("db/db.xml");
  return 0;
  //*/
#endif
#if 0
  Database db;
  db.load("db/db.xml");

  std::cout << "Controls\n\tq - quit\n\tf - find matching book\n\th - hold current frame\n\tu - unhold\n";
  
  try
  {
    Recognizer r(0);
    //Recognizer r("data/MOV03427.MPG");
    r.setDatabase(&db);
    char ch;
    bool and_find = false;
    while(r.next(and_find)) {
      and_find = false;
      r.showCurrentFrame(true);
      ch = cv::waitKey(10);
      if (ch == 'q') break;
      if (ch == 'f') and_find = true;
      if (ch == 'h') r.hold(true);
      if (ch == 'u') r.hold(false);
    }
  }
  catch (Exception e)
  {
    std::cout << "ERROR: " << e.getFullMessage() << "\n";
  }
  return 0;
#endif
}
