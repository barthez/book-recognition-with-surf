#pragma once

#include <Recognizer.h>

#include <gtkmm-2.4\gtkmm.h>

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

  Gtk::Button start_stop_button;
  Gtk::Image image_place;
  Gtk::Menu source_menu;
  Gtk::OptionMenu option_menu;
  Gtk::Label source_label;

  Gtk::VBox main_box;
  Gtk::HBox buttons_box;

    //for menu
  Glib::RefPtr<Gtk::UIManager> m_refUIManager;
  Glib::RefPtr<Gtk::ActionGroup> m_refActionGroup;
  Glib::ustring ui;

  BR::Database db;
  BR::Recognizer recognizer;
  STREAM_SOURCE stream_source;

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

  bool on_delete_event(GdkEventAny * event) override;


};

