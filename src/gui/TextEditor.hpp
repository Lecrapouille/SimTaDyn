#ifndef TEXTEDITOR_HPP_
#  define TEXTEDITOR_HPP_

#  include <gtkmm.h>
#  include "simtypes.hpp"

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

protected:
  friend class TextEditor;

  Gtk::TextView m_textview;
  std::string m_filename;
  std::string m_title;
};

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

  Gtk::Notebook m_notebook;

protected:

  TextDocument* document(const uint32_t i);
  const std::string title();
  void title(std::string const& title);
};

#endif /* TEXTEDITOR_HPP_ */
