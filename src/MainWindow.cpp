#include <iostream>
#include <vector>

#include <opencv2/imgproc/imgproc.hpp>

#include "MainWindow.h"
#include "BookDataDialog.h"
#include <Exceptions.h>

MainWindow::MainWindow(void) :
  start_stop_button("Start"),
  find_button("Find book"),
  add_book_button("Add book"),
  stream_source(STREAM_SOURCE::CAMERA),
  source_label("Source:"),
  run(false),
  find(false),
  showing_image_thread(nullptr)
{
  set_title("Book recognition");
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

  dispatcher.connect([&]()
  {
    showing_image_mutex.lock();
    pixbuf = Gdk::Pixbuf::create_from_data(image.data, Gdk::COLORSPACE_RGB, false, 8, image.cols, image.rows, image.step);
    image_place.set(pixbuf);
    showing_image_mutex.unlock();
  });

  option_menu.set_menu(source_menu);

  start_stop_button.set_sensitive(false);
  add_book_button.set_sensitive(false);
  find_button.set_sensitive(false);
    
  add(main_box);
  main_box.pack_start(*(m_refUIManager->get_widget("/MenuBar")), Gtk::PACK_SHRINK);
  main_box.pack_start(image_event_box);
  main_box.pack_end(buttons_box, Gtk::PACK_SHRINK);

  image_event_box.add(image_place);

  buttons_box.pack_end(start_stop_button);
  buttons_box.pack_start(source_label, Gtk::PACK_SHRINK);
  buttons_box.pack_start(option_menu, Gtk::PACK_SHRINK);
  buttons_box.pack_start(find_button, Gtk::PACK_SHRINK);
  buttons_box.pack_start(add_book_button, Gtk::PACK_SHRINK);

  start_stop_button.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_start_stop_button_clicked));
  find_button.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_find_button_clicked));
  add_book_button.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_add_book_button_clicked));
  image_event_box.signal_button_press_event().connect(sigc::mem_fun(*this, &MainWindow::on_on_image_place_clicked));

 
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
  if (run)
  {
    Gtk::MessageDialog("You cannot quit during run").run();
    return true;
  }
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
  find_button.set_sensitive(true);
  if (run) //stopping
  {
    find = run = false;
    std::this_thread::sleep_for(std::chrono::milliseconds(100)); //wait for other thread to finish
    showing_image_thread->join();
    delete showing_image_thread;
    showing_image_thread = nullptr;
    start_stop_button.set_label("Start");
    option_menu.set_sensitive(true);
    add_book_button.set_sensitive(true);
  }
  else //starting
  {
    run = true;
    //showing_image_thread = Glib::Thread::create(sigc::mem_fun(*this, &MainWindow::showing_frames), true);
    showing_image_thread = new std::thread(&MainWindow::showing_frames, this);
    start_stop_button.set_label("Stop");
    option_menu.set_sensitive(false);
    add_book_button.set_sensitive(false);
  }
}

void MainWindow::on_load_database_menu_item_clicked()
{
  if (run) //running
  {
    Gtk::MessageDialog("You can't load database while running").run();
    return;
  }
  Gtk::FileChooserDialog dialog("Choose a file", Gtk::FILE_CHOOSER_ACTION_OPEN);
  dialog.set_transient_for(*this);

  dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
  dialog.add_button("OK", Gtk::RESPONSE_OK);

  dialog.add_filter(xml_filter);
  dialog.add_filter(all_filter);

  if (dialog.run() == Gtk::RESPONSE_OK)
  {
    try {
      db.load(dialog.get_filename());
    } catch (BR::Exception * ex) {
      Gtk::MessageDialog(ex->getFullMessage().c_str()).run();
      return;
    }
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
  add_book_button.set_sensitive(false);
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
//    gdk_threads_enter();
    getFrame(false);
  //  gdk_threads_leave();
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }
  }
  catch (...) //in case when video is over
  {
    Gtk::MessageDialog("Video reached end\n").run();
    //on_start_stop_button_clicked();
    //gdk_threads_leave();
  }
}

bool MainWindow::on_on_image_place_clicked(GdkEventButton * button)
{
  if (!read_image)
  {
    return true;
  }
  static int count = 0;
  static std::vector<cv::Point2f> points(4);
  points[count] = cv::Point2f(button->x, button->y);
  if (++count == 4)
  {
    count = 0;
    read_image = false;
    BookDataDialog bdd;
    if (bdd.run() == Gtk::RESPONSE_OK)
    {
      db.fixAndAdd(image, points, bdd.getTitle(), bdd.getAuthor(), bdd.getISBN());
      Gtk::MessageDialog("Book was added to database").run();
    }
  }
  
  return true;
}

void MainWindow::on_add_book_button_clicked()
{
  read_image = true;
}

void MainWindow::on_find_button_clicked()
{
  on_start_stop_button_clicked();
  getFrame(true);
  recognizer.hold(false);
}

void MainWindow::getFrame(bool find_book)
{
  recognizer.next(find_book);
  cv::Mat tmp = recognizer.getCurrentFrame(true);
  cv::Mat tmp2;
  cv::cvtColor(tmp, tmp2, CV_BGR2RGB);
  showing_image_mutex.lock();
  cv::resize(tmp2, image, cv::Size(image_event_box.get_width(), image_event_box.get_height()));
//  pixbuf = Gdk::Pixbuf::create_from_data(image.data, Gdk::COLORSPACE_RGB, false, 8, image.cols, image.rows, image.step);
  //image_place.set(pixbuf);
  //image_place.queue_draw();
  showing_image_mutex.unlock();
  dispatcher.emit();
  //showing_image_mutex.unlock();
  //dispatcher.emit();
  //std::this_thread::yield();
}

