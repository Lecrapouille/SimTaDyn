#ifndef FORTHEDITOR_HPP_
#  define FORTHEDITOR_HPP_

#  include "TextEditor.hpp"
#  include "Forth.hpp"

// *************************************************************************************************
//
// *************************************************************************************************
class ForthEditor : public TextEditor
{
public:

  ForthEditor();
  ~ForthEditor();
  void execForth();

  Gtk::Notebook       m_res_notebooks; // FIXME: attention collision de noms TextEditor::m_notebook
  Gtk::ScrolledWindow m_scrolledwindow[4];
  Gtk::TextView       m_result;
  Gtk::TextView       m_historic;
  Gtk::TextView       m_debug;
  Gtk::TextView       m_dico;
  Gtk::Statusbar      m_statusbar;
  //SimForth* m_forth; // FIXME: utile si on gere plusieurs cartes
};

#endif /* FORTHEDITOR_HPP_ */
