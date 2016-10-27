#ifndef TEXTEDITOR_HPP_
#  define TEXTEDITOR_HPP_

#  include <gtkmm.h>
#  include "Forth.hpp"

class TextDocument;
class FindWindow;
class TextEditor;

// *************************************************************************************************
//
// *************************************************************************************************
class FindWindow : public Gtk::Window
{
public:
  FindWindow(Gtk::TextView* document);
  void document(Gtk::TextView* document);
  void findNext();
  void findFirst();

protected:
  void find(Glib::ustring const& text, Gtk::TextBuffer::iterator& iter);
  //void clearMessage();

  Gtk::Window m_win;
  Gtk::VBox m_vbox;
  Gtk::HBox m_hbox;
  Gtk::Label m_label;
  Gtk::Label m_status;
  Gtk::Entry m_entry;
  Gtk::Button m_search;
  Gtk::Button m_next;
  Gtk::TextView* m_document;
};

// *************************************************************************************************
//
// *************************************************************************************************
class CloseLabel : public Gtk::HBox
{
public:
  CloseLabel(std::string const& text);
  void label(std::string const& text);
  Glib::ustring label();
  void link(Gtk::Notebook *notebook, Gtk::Widget *widget);

protected:
  void onClicked();

  Gtk::HBox  m_hbox;
  Gtk::Label m_label;
  Gtk::Button m_button;
  Gtk::Image m_image;
  Gtk::Notebook *m_notebook;
  Gtk::Widget *m_widget;
};

// *************************************************************************************************
// TBD: undo/redo
// *************************************************************************************************
class TextDocument : public Gtk::ScrolledWindow
{
public:
  TextDocument();
  ~TextDocument();
  void clear();
  bool isModified() const;
  void onChanged();
  bool save();
  bool saveAs(std::string const& filename);
  bool load(std::string const& filename, bool clear = true);

protected:
  friend class TextEditor;

  Gtk::TextView m_textview;
  CloseLabel m_button;
  std::string m_filename;
};

// *************************************************************************************************
//
// *************************************************************************************************
class TextEditor
{
public:
  TextEditor();
  ~TextEditor();
  void newDocument();
  void newEmptyDocument();
  void newTemplatedDocument(std::string const& text);
  void saveCurrentDocument();
  void saveAsCurrentDocument();
  Glib::ustring text();
  void clear();
  void find();

  Gtk::Notebook m_notebook;
  FindWindow m_findwindow;

protected:
  TextDocument* document();
  TextDocument* document(const uint32_t i);
  bool dialogSave(TextDocument *doc);
  bool saveAs(TextDocument *doc);
  void newLoadedDocument(std::string const& filename);
  void onPageSwitched(GtkNotebookPage* page, guint page_num);

  int m_nb_nonames;
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
