#include "BookDataDialog.h"


BookDataDialog::BookDataDialog(void) :
  title_label("Title:"),
  author_label("Author:"),
  isbn_label("ISBN:")
{
  title_box.pack_start(title_label);
  title_box.pack_end(title_entry);

  author_box.pack_start(author_label);
  author_box.pack_end(author_entry);
  
  isbn_box.pack_start(isbn_label);
  isbn_box.pack_end(isbn_entry);

  main_box.pack_start(title_box);
  main_box.pack_start(author_box);
  main_box.pack_start(isbn_box);

  get_vbox()->pack_start(main_box);
  
  get_vbox()->show();
  get_vbox()->show_all_children(true);

  add_button("Add to database", Gtk::RESPONSE_OK);
}


BookDataDialog::~BookDataDialog(void)
{
}

std::string BookDataDialog::getTitle()
{
  return title_entry.get_text();
}

std::string BookDataDialog::getAuthor()
{
  return author_entry.get_text();
}

std::string BookDataDialog::getISBN()
{
  return isbn_entry.get_text();
}
