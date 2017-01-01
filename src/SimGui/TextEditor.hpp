#ifndef TEXTEDITOR_HPP_
#  define TEXTEDITOR_HPP_

#  include <gtkmm.h>
#  include <gtksourceviewmm.h>
#  include "simtypes.hpp"

class TextDocument;
class FindWindow;
class ReplaceWindow;
class GotoLineWindow;
class TextEditor;

// *************************************************************************************************
//
// *************************************************************************************************
class GotoLineWindow : public Gtk::Window
{
public:
  GotoLineWindow(Gsv::View* document);
  void document(Gsv::View* document);
  void gotoLine();
  inline void title(std::string const& text)
  {
    set_title("Go to Line in " + text);
  }

  Gtk::VBox m_vbox;
  Gtk::HBox m_hbox;
  Gtk::Label m_label;
  Gtk::Entry m_entry;
  Gtk::Button m_button;
  Gsv::View* m_document;
};

// *************************************************************************************************
//
// *************************************************************************************************
class Find : public Gtk::Window
{
public:
  Find(Gsv::View* document);
  void document(Gsv::View* document);
  void findNext();
  void findFirst();

protected:
  void find(Glib::ustring const& text, Gtk::TextBuffer::iterator& iter);
  Gsv::View* m_document;
  Gtk::Entry m_entry;
  Gtk::Label m_status;
  bool m_found;
  Gtk::TextBuffer::iterator m_start, m_end;
};

// *************************************************************************************************
//
// *************************************************************************************************
class FindWindow : public Find
{
public:
  FindWindow(Gsv::View* document);
  inline void title(std::string const& text)
  {
    set_title("Find in " + text);
  }

protected:
  Gtk::VBox m_vbox;
  Gtk::HBox m_hbox;
  Gtk::Label m_label;
  Gtk::Button m_next;
};

// *************************************************************************************************
//
// *************************************************************************************************
class ReplaceWindow : public Find
{
public:
  ReplaceWindow(Gsv::View* document);
  void replace();
  void replaceAll();
  void find();
  inline void title(std::string const& text)
  {
    set_title("Find and Replace in " + text);
  }

protected:
  Gtk::VBox m_vbox;
  Gtk::HBox m_hbox;
  Gtk::Label m_label;
  Gtk::Label m_label2;
  Gtk::Entry m_entry2;
  Gtk::Button m_search;
  Gtk::Button m_replace;
  Gtk::Button m_all;
};

// *************************************************************************************************
//
// *************************************************************************************************
class CloseLabel : public Gtk::Box
{
public:
  CloseLabel(std::string const& text);
  //! Kind of button.signal_clicked().connect(sigc::bind<T>(sigc::mem_fun(*this, &CloseLabel::onClicked), T));
  inline void link(Gtk::Notebook *notebook, Gtk::Widget *widget)
  {
    m_notebook = notebook;
    m_widget = widget;
  }
  //! Return the text of the button (here the filename of the
  //! document).
  inline const std::string& title() const
  {
    return m_title;
  }
  //! Change the text of the button
  inline void title(std::string const& title)
  {
    m_title = title;
    m_label.set_text(m_title);
  }
  void asterisk(const bool asterisk);
  inline bool asterisk() const
  {
    return m_asterisk;
  }

protected:
  inline void onClicked()
  {
    m_notebook->remove_page(*m_widget);
  }

  // Use the middle button to close the document
  bool onButtonPressEvent(GdkEventButton* event)
  {
    bool res = (GDK_BUTTON_MIDDLE == event->button);
    if (res) onClicked();
    return res;
  }

  Gtk::Label m_label;
  Gtk::Button m_button;
  Gtk::Image m_image;
  Gtk::Notebook *m_notebook;
  Gtk::Widget *m_widget;
  bool m_asterisk;
  std::string m_title;
};

// *************************************************************************************************
// TBD: undo/redo
// *************************************************************************************************
class TextDocument : public Gtk::ScrolledWindow
{
public:
  TextDocument(Glib::RefPtr<Gsv::Language> language);
  void clear();
  bool isModified() const;
  void onChanged();
  bool save();
  bool saveAs(std::string const& filename);
  bool load(std::string const& filename, bool clear = true);
  void cursorAt(const uint32_t line, const uint32_t index);
  virtual void autoCompleteWord(__attribute__((unused)) int keyval)
  {
  }
  inline Glib::RefPtr<Gsv::Buffer> buffer()
  {
    return m_buffer;
  }
  inline void title(std::string const& text)
  {
    m_button.title(text);
  }
  inline Glib::ustring title()
  {
    return m_button.title();
  }
  inline void filename(std::string const& filename)
  {
    m_filename = filename;
  }
  inline const std::string& filename() const
  {
    return m_filename;
  }

protected:
  friend class TextEditor;

  Gsv::View m_textview;
  Glib::RefPtr<Gsv::Buffer> m_buffer;
  CloseLabel m_button;
  std::string m_filename;
  // FIXME: mode interactif ==> ne pas sauvegarder
};

// *************************************************************************************************
//
// *************************************************************************************************
class TextEditor
{
public:
  TextEditor();
  ~TextEditor();
  void open();
  void open1(std::string const& filename); // FIXME ugly name: temporary
  void empty(std::string const& title = "New document");
  void save();
  void saveAs();
  void saveAll();
  Glib::ustring text();
  void clear();
  void find();
  void replace();
  void gotoLine();

  Gtk::Notebook m_notebook;
  FindWindow m_findwindow;
  ReplaceWindow m_replacewindow;
  GotoLineWindow m_gotolinewindow;

  Gtk::MenuItem       m_menuitem[2];
  Gtk::Menu           m_menu[2];
  Gtk::ImageMenuItem  m_submenu[16];
  Gtk::Image          m_image[16];
  Gtk::SeparatorMenuItem m_menuseparator[4];

protected:
  inline virtual TextDocument* create()
  {
    return new TextDocument(m_language);
  }
  TextDocument* document();
  TextDocument* document(const uint32_t i);
  bool dialogSave(TextDocument *doc);
  bool saveAs(TextDocument *doc);
  void newLoadedDocument(std::string const& filename);
  void onPageSwitched(Gtk::Widget* page, guint page_num);

  int m_nb_nonames;
  Glib::RefPtr<Gsv::LanguageManager> m_language_manager;
  Glib::RefPtr<Gsv::Language> m_language;
};

#endif /* TEXTEDITOR_HPP_ */
