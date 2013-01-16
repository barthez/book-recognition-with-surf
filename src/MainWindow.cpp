#include "MainWindow.h"

MainWindow::MainWindow(void) :
  start_stop_button("Start")
{
  set_title("Book reogition");

  set_size_request(800, 600);

  add(main_box);
  main_box.pack_start(image_place);
  main_box.pack_end(start_stop_button);
  
  //showing
  main_box.show();
  main_box.show_all_children(true);
}


MainWindow::~MainWindow(void)
{
}
