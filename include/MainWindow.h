#pragma once

#include <Recognizer.h>

#include <gtkmm.h>
#include <opencv2/core/core.hpp>

#include <thread>
#include <atomic>
#include <mutex>

enum class STREAM_SOURCE
{
  CAMERA,
  FILE
};

class MainWindow : public Gtk::Window
{
public:
  MainWindow(void);
  ~MainWindow(void);
private:
  void makeMenu();

  Glib::Dispatcher dispatcher;
  cv::Mat image;
  Glib::RefPtr<Gdk::Pixbuf> pixbuf;

  Gtk::EventBox image_event_box;

  Gtk::Button start_stop_button;
  Gtk::Button find_button;
  Gtk::Button add_book_button;
  Gtk::Image image_place;
  Gtk::Menu source_menu;
  Gtk::OptionMenu option_menu;
  Gtk::Label source_label;

  Gtk::VBox main_box;
  Gtk::HBox buttons_box;
  
  Gtk::Statusbar status_bar;

    //for menu
  Glib::RefPtr<Gtk::UIManager> m_refUIManager;
  Glib::RefPtr<Gtk::ActionGroup> m_refActionGroup;
  Glib::ustring ui;

  BR::Database db;
  BR::Recognizer recognizer;
  STREAM_SOURCE stream_source;

  std::atomic<bool> find;
  std::atomic<bool> run;
  //Glib::Thread * showing_image_thread;
  std::thread * showing_image_thread;
  std::mutex showing_image_mutex;

  bool read_image;

  bool canQuit();
  void setFilters();

  Gtk::FileFilter xml_filter, all_filter, video_filter;

  //callbacks
  void on_start_stop_button_clicked();
  void on_quit_menu_item_clicked();
  void on_load_database_menu_item_clicked();
  void on_save_database_menu_item_clicked();
  void on_clear_database_menu_item_clicked();
  void on_source_menu_changed(STREAM_SOURCE);
  void on_find_button_clicked();
  void on_add_book_button_clicked();
  bool on_on_image_place_clicked(GdkEventButton * button);

  bool on_delete_event(GdkEventAny * event) override;

  void showing_frames();

  void getFrame(bool find_book);


};

