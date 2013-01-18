#pragma once
#include <gtkmm.h>

#include <string>

class BookDataDialog :  public Gtk::Dialog
{
public:
  BookDataDialog(void);
  ~BookDataDialog(void);

  std::string getTitle();
  std::string getAuthor();
  std::string getISBN();
private:
  Gtk::Label title_label, author_label, isbn_label;
  Gtk::Entry title_entry, author_entry, isbn_entry;
  Gtk::HBox title_box, author_box, isbn_box;
  Gtk::VBox main_box;
};

