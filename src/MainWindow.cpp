#include <iostream>

#include "MainWindow.h"

MainWindow::MainWindow(void) :
  start_stop_button("Start"),
  stream_source(STREAM_SOURCE::CAMERA),
  source_label("Source:"),
  run(false),
  showing_image_thread(nullptr)
{
  set_title("Book reogition");
  set_size_request(800, 600);

  makeMenu();
  setFilters();

  Gtk::Menu_Helpers::MenuList& list_upd = source_menu.items();
  list_upd.push_back(
    Gtk::Menu_Helpers::MenuElem("From camera", sigc::bind(sigc::mem_fun(*this,
    &MainWindow::on_source_menu_changed), STREAM_SOURCE::CAMERA)));
  list_upd.push_back(
    Gtk::Menu_Helpers::MenuElem("From file", sigc::bind(sigc::mem_fun(*this,
    &MainWindow::on_source_menu_changed), STREAM_SOURCE::FILE)));

  option_menu.set_menu(source_menu);

  start_stop_button.set_sensitive(false);
    
  add(main_box);
  main_box.pack_start(*(m_refUIManager->get_widget("/MenuBar")), Gtk::PACK_SHRINK);
  main_box.pack_start(image_place);
  main_box.pack_end(buttons_box, Gtk::PACK_SHRINK);

  buttons_box.pack_end(start_stop_button);
  buttons_box.pack_start(source_label, Gtk::PACK_SHRINK);
  buttons_box.pack_start(option_menu, Gtk::PACK_SHRINK);

  start_stop_button.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_start_stop_button_clicked));
 
  //showing
  main_box.show();
  main_box.show_all_children(true);

  //control
  recognizer.setDatabase(&db);
}


MainWindow::~MainWindow(void)
{
}

void MainWindow::makeMenu()
{
  ui = "<ui>\n"
    "<menubar name='MenuBar'>\n"
    "<menu action='MenuFile'>\n"
    "<menuitem action='LoadDatabase' />\n"
    "<menuitem action='SaveDatabase' />\n"
    "<menuitem action='ClearDatabase' />\n"
    "<menuitem action='Quit' />\n"
    "</menu>\n"
    "</menubar>\n"
    "</ui>\n";

  m_refActionGroup = Gtk::ActionGroup::create();
  m_refActionGroup->add(Gtk::Action::create("MenuFile", "_File"));
  m_refActionGroup->add(Gtk::Action::create("Quit", "Exit"), sigc::mem_fun(*this, &MainWindow::on_quit_menu_item_clicked));
  m_refActionGroup->add(Gtk::Action::create("LoadDatabase", "_Load database"), sigc::mem_fun(*this, &MainWindow::on_load_database_menu_item_clicked));
  m_refActionGroup->add(Gtk::Action::create("SaveDatabase", "_Store database"), sigc::mem_fun(*this, &MainWindow::on_save_database_menu_item_clicked));
  m_refActionGroup->add(Gtk::Action::create("ClearDatabase", "_Clear database"), sigc::mem_fun(*this, &MainWindow::on_clear_database_menu_item_clicked));

  m_refUIManager = Gtk::UIManager::create();
  m_refUIManager->insert_action_group(m_refActionGroup);
  add_accel_group(m_refUIManager->get_accel_group());
  m_refUIManager->add_ui_from_string(ui);
}

void MainWindow::on_quit_menu_item_clicked()
{
  if (!canQuit())
  {
    hide();
  }
}

bool MainWindow::on_delete_event(GdkEventAny* event)
{
  //TODO: Dialog
  return canQuit();
}

bool MainWindow::canQuit()
{
  on_start_stop_button_clicked();
  if (!db.isSaved())
  {
    Gtk::MessageDialog dialog(*this, "Do you want to store databese before quit?", false, Gtk::MESSAGE_QUESTION, Gtk::BUTTONS_YES_NO);
    dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);

    switch (dialog.run())
    {
    case Gtk::RESPONSE_YES:
      on_save_database_menu_item_clicked();
      break;
    case Gtk::RESPONSE_CANCEL:
      return true;
    }
  }
  return false;
}

void MainWindow::on_start_stop_button_clicked()
{
  if (run)
  {
    run = false;
    showing_image_thread->join();
    //delete showing_image_thread;
    showing_image_thread = nullptr;
    start_stop_button.set_label("Start");
    option_menu.set_sensitive(true);
  }
  else
  {
    run = true;
    showing_image_thread = Glib::Thread::create(sigc::mem_fun(*this, &MainWindow::showing_frames), true);
    //showing_image_thread = new std::thread(&MainWindow::showing_frames, this);
    start_stop_button.set_label("Stop");
    option_menu.set_sensitive(false);
  }
}

void MainWindow::on_load_database_menu_item_clicked()
{
  Gtk::FileChooserDialog dialog("Choose a file", Gtk::FILE_CHOOSER_ACTION_OPEN);
  dialog.set_transient_for(*this);

  dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
  dialog.add_button("OK", Gtk::RESPONSE_OK);

  dialog.add_filter(xml_filter);
  dialog.add_filter(all_filter);

  if (dialog.run() == Gtk::RESPONSE_OK)
  {
    db.load(dialog.get_filename());
  }
}

void MainWindow::on_save_database_menu_item_clicked()
{
  Gtk::FileChooserDialog dialog("Choose a file", Gtk::FILE_CHOOSER_ACTION_SAVE);
  dialog.set_transient_for(*this);

  dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
  dialog.add_button("OK", Gtk::RESPONSE_OK);

  dialog.add_filter(xml_filter);
  dialog.add_filter(all_filter);

  if (dialog.run() == Gtk::RESPONSE_OK)
  {
    std::string file = dialog.get_filename();
    if (file.substr(file.length() - 4) != ".xml")
    {
      file = file + ".xml";
    }
    db.save(file);
  }
}

void MainWindow::on_clear_database_menu_item_clicked()
{
  if (Gtk::MessageDialog(*this, "Do you want to clear current database?", false, Gtk::MESSAGE_QUESTION, Gtk::BUTTONS_YES_NO).run() == Gtk::RESPONSE_YES)
  {
    db.clear();
  }
}

void MainWindow::on_source_menu_changed(STREAM_SOURCE source)
{
  start_stop_button.set_sensitive(true);
  switch (source)
  {
  case STREAM_SOURCE::CAMERA:
    recognizer.open(0);
    break;
  case STREAM_SOURCE::FILE:
    Gtk::FileChooserDialog dialog("Choose a video file", Gtk::FILE_CHOOSER_ACTION_SAVE);
    dialog.set_transient_for(*this);
    dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
    dialog.add_button("OK", Gtk::RESPONSE_OK);
    dialog.add_filter(video_filter);
    dialog.add_filter(all_filter);
    if (dialog.run() == Gtk::RESPONSE_OK)
    {
      recognizer.open(dialog.get_filename());
    }
    break;  
  }
}

void MainWindow::setFilters()
{
  xml_filter.set_name("XML files");
  xml_filter.add_mime_type("text/xml");
  xml_filter.add_mime_type("application/xml");
  
  all_filter.set_name("Any files");
  all_filter.add_pattern("*");

  video_filter.set_name("Video files");
  video_filter.add_mime_type("video/mpeg");
  video_filter.add_mime_type("video/mp4");
  video_filter.add_mime_type("video/quicktime");
  video_filter.add_mime_type("video/x-ms-wmv");
  video_filter.add_mime_type("video/ogg");
  video_filter.add_mime_type("video/webm");
  video_filter.add_mime_type("video/x-flv");
}

void MainWindow::showing_frames()
{
    cv::Mat tmp;
  try
  {
  while (run)
  {
    recognizer.next();
    tmp = recognizer.getCurrentFrame(false);
    image = tmp.clone();
    pixbuf = Gdk::Pixbuf::create_from_data(image.data, Gdk::COLORSPACE_RGB, false, 8, image.cols, image.rows, image.step);
    image_place.set(pixbuf);
    image_place.queue_draw();
  }
  }
  catch (...)
  {
    std::cout << "EXCEPTIOn\n";
  }
}
