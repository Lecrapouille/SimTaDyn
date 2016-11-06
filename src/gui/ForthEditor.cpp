#include "ForthEditor.hpp"
#include "SimTaDynContext.hpp"

// *************************************************************************************************
// Get all text in the current text editor and give it to the Forth interpreter
// Return true if the code was interpreted correctly, else return false.
// *************************************************************************************************
void ForthEditor::execForth()
{
  SimTaDynContext& simtadyn = SimTaDynContext::getInstance();
  std::pair<bool, std::string> res = simtadyn.m_forth.eatString(ForthEditor::text().raw());

  if (res.first)
    {
      m_statusbar.push("OK");

      // Paste the script Forth result in the "Result" tab of the notebook
      Glib::RefPtr<Gtk::TextBuffer> buf = m_output.get_buffer();
      buf->erase(buf->begin(), buf->end());
      buf->insert(buf->end(), res.second);

      // Copy paste the Forth script into the historic buffer
      buf = m_historic.get_buffer();
      buf->insert(buf->end(), ForthEditor::text());
      buf->insert(buf->end(), "\n\n");

      // Clear the text editor if and only if we are in an interactive mode
      //if (m_interactive)
        {
          ForthEditor::clear();
        }

      // TODO: inserer nouveau mot dans tree
    }
  else
    {
      // Text view: indiquer ligne ko
      m_statusbar.push("FAILED ! Reason: '" + res.second + "'");
    }

  //return res.first;
}
