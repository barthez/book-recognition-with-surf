#include "MainWindow.h"

MainWindow::MainWindow(void) :
  start_stop_button("Start")
{
  set_title("Book reogition");
  set_size_request(800, 600);

  makeMenu();

  Gtk::Menu_Helpers::MenuList& list_upd = source_menu.items();
  list_upd.push_back(
    Gtk::Menu_Helpers::MenuElem("From file", sigc::bind(sigc::mem_fun(*this,
    &MainWindow::on_source_menu_changed), STREAM_SOURCE::FILE)));
  list_upd.push_back(
    Gtk::Menu_Helpers::MenuElem("From camera", sigc::bind(sigc::mem_fun(*this,
    &MainWindow::on_source_menu_changed), STREAM_SOURCE::CAMERA)));

  option_menu.set_menu(source_menu);
    
  add(main_box);
  main_box.pack_start(*(m_refUIManager->get_widget("/MenuBar")), Gtk::PACK_SHRINK);
  main_box.pack_end(buttons_box);

  buttons_box.pack_end(start_stop_button);
  buttons_box.pack_start(option_menu);

  start_stop_button.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_start_stop_button_clicked));
 
  //showing
  main_box.show();
  main_box.show_all_children(true);

  //control
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
  db.addBook(new BR::Book());
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
    return false;
  }
}

void MainWindow::on_start_stop_button_clicked()
{
}

void MainWindow::on_load_database_menu_item_clicked()
{
  Gtk::FileChooserDialog dialog("Choose a file", Gtk::FILE_CHOOSER_ACTION_OPEN);
  dialog.set_transient_for(*this);

  dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
  dialog.add_button("OK", Gtk::RESPONSE_OK);

  if (dialog.run() == Gtk::RESPONSE_OK)
  {
    db.load(dialog.get_filename());
  }
}

void MainWindow::on_save_database_menu_item_clicked()
{
}

void MainWindow::on_clear_database_menu_item_clicked()
{
}

void MainWindow::on_source_menu_changed(STREAM_SOURCE source)
{

}

/*

  db.load("db/db.xml");
  image_place.set(Gdk::Pixbuf::create_from_data(db.books.front()->getImage().data, Gdk::COLORSPACE_RGB, false, 8, db.books.front()->getImage().cols, db.books.front()->getImage().rows, db.books.front()->getImage().step));
  image_place.queue_draw();
  */