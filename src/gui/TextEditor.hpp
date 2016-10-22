#ifndef TEXTEDITOR_HPP_
#  define TEXTEDITOR_HPP_

#  include <gtkmm.h>
#  include "Forth.hpp"

class TextDocument;

// *************************************************************************************************
//
// *************************************************************************************************
class FindWindow : public Gtk::Window
{
public:
  FindWindow(TextDocument* ancestor);
  ~FindWindow()
  {
  }

protected:
  void find(Glib::ustring const& text, Gtk::TextBuffer::iterator& iter);

  Gtk::Window win;
  Gtk::VBox vbox;
  Gtk::HBox hbox;
  Gtk::Entry entry;
  Gtk::Button search;
  Gtk::Button next;
  TextDocument* m_ancestor;

private:
  void findNext();//Gtk::Entry* entry);//Glib::ustring const& text);
  void findFirst();//Gtk::Entry* entry);//Glib::ustring text);//Glib::ustring const& text);
};

// *************************************************************************************************
//
// *************************************************************************************************
class TextDocument : public Gtk::ScrolledWindow
{
public:
  TextDocument();
  ~TextDocument();
  bool isModified() const;
  bool questionSave();
  bool winSaveAs();

  bool save();
  bool saveAs(std::string const& filename);
  bool load(std::string const& filename);
  void clear();
  void find();

protected:
  friend class TextEditor;
  friend class FindWindow;

  void findWindowClose();

  Gtk::TextView m_textview;
  std::string m_filename;
  std::string m_title;
  FindWindow* m_findwindow;
};

// *************************************************************************************************
//
// *************************************************************************************************
class CloseLabel : public Gtk::HBox//Gtk::Widget
{
public:
  CloseLabel(std::string const& text);

protected:
  void on_close_button_clicked();
  Gtk::HBox  hbox;
  Gtk::Label tab_label;
  Gtk::Button close_button;
  Gtk::Image image;
};

// *************************************************************************************************
//
// *************************************************************************************************
class TextEditor
{
public:
  TextEditor();
  ~TextEditor(); // check all TextDocument is modifed

  void addEmptyTab();
  void addFileTab();
  void addFileTab(std::string const& filename);
  void saveCurrentTab();
  void saveCurrentTabAs();
  TextDocument* document();
  Glib::ustring text();
  void clear();

  Gtk::Notebook m_notebook;

protected:

  TextDocument* document(const uint32_t i);
  const std::string title();
  void title(std::string const& title);
};

// *************************************************************************************************
//
// *************************************************************************************************
//A bouger ailleurs
class ForthEditor : public TextEditor
{
public:
  ForthEditor()
  {
  }
  bool execForth(Forth& forth);
};

#endif /* TEXTEDITOR_HPP_ */
