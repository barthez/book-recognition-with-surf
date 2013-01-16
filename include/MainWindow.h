#pragma once

#include <Recognizer.h>

#include <gtkmm-2.4\gtkmm.h>

class MainWindow : public Gtk::Window
{
public:
  MainWindow(void);
  ~MainWindow(void);
private:
  Gtk::Button start_stop_button;
  Gtk::Image image_place;

  Gtk::VBox main_box;


};

