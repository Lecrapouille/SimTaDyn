#ifndef FORTHEDITOR_HPP_
#  define FORTHEDITOR_HPP_

#  include "TextEditor.hpp"
#  include "Forth.hpp"

// *************************************************************************************************
//
// *************************************************************************************************
//A bouger ailleurs
class ForthEditor : public TextEditor
{
public:
  ForthEditor(Gtk::TextView& output, Gtk::Statusbar& msg, Gtk::TextView& historic, bool interactive = false)
    : m_output(output),
      m_statusbar(msg),
      m_historic(historic),
      m_interactive(interactive)
  {
  }

  ~ForthEditor()
  {
    // TODO: save the historic buffer
  }

  void execForth();

protected:
  Gtk::TextView& m_output;
  Gtk::Statusbar&  m_statusbar;
  Gtk::TextView& m_historic;
  bool m_interactive;
};

#endif /* FORTHEDITOR_HPP_ */
