#include "ForthEditor.hpp"
#include "SimTaDynContext.hpp"

// *************************************************************************************************
//
// *************************************************************************************************
ForthEditor::ForthEditor()
{
  // Forth text view for storing results, debug, historic, dictionary
  {
    // FIXME: mettre les text view en read-only
    m_scrolledwindow[0].add(m_result);
    m_scrolledwindow[0].set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
    m_scrolledwindow[1].add(m_historic);
    m_scrolledwindow[1].set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
    m_scrolledwindow[2].add(m_debug);
    m_scrolledwindow[2].set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
    m_scrolledwindow[3].add(m_dico);
    m_scrolledwindow[3].set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
  }

  // Forth notebook
  {
    m_res_notebooks.append_page(m_scrolledwindow[0], "Result");
    m_res_notebooks.append_page(m_scrolledwindow[1], "Historic");
    m_res_notebooks.append_page(m_scrolledwindow[2], "Debug");
    m_res_notebooks.append_page(m_scrolledwindow[3], "Dico");
  }

  // Statusbar
  {
    m_statusbar.push("Welcome to SimTaDyn !");
  }
}

// *************************************************************************************************
//
// *************************************************************************************************
ForthEditor::~ForthEditor()
{
  // TODO: save the historic buffer
}

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
      Glib::RefPtr<Gtk::TextBuffer> buf = m_result.get_buffer();
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
