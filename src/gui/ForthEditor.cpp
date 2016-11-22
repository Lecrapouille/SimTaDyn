#include "ForthEditor.hpp"
#include "SimTaDynContext.hpp"

// *************************************************************************************************
//
// *************************************************************************************************
ForthEditor::ForthEditor()
{
  // Submenus '_Forth'
  {
    Gtk::Menu::MenuList& menulist = m_menu.items();

    m_menuimage[3].set(Gtk::Stock::NEW, Gtk::ICON_SIZE_MENU);
    menulist.push_back(Gtk::Menu_Helpers::ImageMenuElem("_New File",
       m_menuimage[3], sigc::mem_fun(*this, &ForthEditor::newEmptyDocument)));

    m_menuimage[4].set(Gtk::Stock::NEW, Gtk::ICON_SIZE_MENU);
    menulist.push_back(Gtk::Menu_Helpers::ImageMenuElem("_New Template",
       m_menuimage[4], sigc::mem_fun(*this, &ForthEditor::newTemplatedDocument))); // TODO

    m_menuimage[6].set(Gtk::Stock::NEW, Gtk::ICON_SIZE_MENU);
    menulist.push_back(Gtk::Menu_Helpers::ImageMenuElem("_New Prompt",
       m_menuimage[6], sigc::mem_fun(*this, &ForthEditor::newEmptyDocument))); // FIXME TBD TODO

    menulist.push_back(m_menuseparator[0]);

    m_menuimage[0].set(Gtk::Stock::OPEN, Gtk::ICON_SIZE_MENU);
    menulist.push_back(Gtk::Menu_Helpers::ImageMenuElem("_Open",
       m_menuimage[0], sigc::mem_fun(*this, &ForthEditor::newDocument)));

    m_menuimage[1].set(Gtk::Stock::SAVE_AS, Gtk::ICON_SIZE_MENU);
    menulist.push_back(Gtk::Menu_Helpers::ImageMenuElem("_SaveAs",
       m_menuimage[1], sigc::mem_fun(*this, &ForthEditor::saveAsCurrentDocument)));

    m_menuimage[2].set(Gtk::Stock::SAVE, Gtk::ICON_SIZE_MENU);
    menulist.push_back(Gtk::Menu_Helpers::ImageMenuElem("_Save",
       m_menuimage[2], sigc::mem_fun(*this, &ForthEditor::saveCurrentDocument)));

    menulist.push_back(m_menuseparator[1]);

    m_menuimage[5].set(Gtk::Stock::FIND, Gtk::ICON_SIZE_MENU);
    menulist.push_back(Gtk::Menu_Helpers::ImageMenuElem("_Find",
       m_menuimage[5], sigc::mem_fun(*this, &ForthEditor::find)));
  }

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
//
// *************************************************************************************************
void ForthEditor::newTemplatedDocument()
{
  TextEditor::newTemplatedDocument(": COMPUTE-ME 1 1 + . ;\nCOMPUTE-ME");
}

// *************************************************************************************************
//
// *************************************************************************************************
std::string  ForthEditor::elapsedTime()
{
  using namespace std;
  using namespace std::chrono;
  typedef duration<int, ratio<86400>> days;
  std::chrono::nanoseconds ns = m_elapsed_time;

  stringstream ss;
  char fill = ss.fill();

  ss.fill('0');
  auto d = duration_cast<days>(ns);
  ns -= d;
  auto h = duration_cast<hours>(ns);
  ns -= h;
  auto m = duration_cast<minutes>(ns);
  ns -= m;
  auto s = duration_cast<seconds>(ns);
  ns -= s;
  auto ms = duration_cast<milliseconds>(ns);
  ns -= ms;
  auto us = duration_cast<microseconds>(ns);
  ns -= us;

  ss << setw(2) << d.count() << "d:"
     << setw(2) << h.count() << "h:"
     << setw(2) << m.count() << "m:"
     << setw(2) << s.count() << "s:"
     << setw(2) << ms.count() << "ms:"
     << setw(2) << us.count() << "us";
  ss.fill(fill);
  return ss.str();
}

// *************************************************************************************************
// Get all text in the current text editor and give it to the Forth interpreter
// Return true if the code was interpreted correctly, else return false.
// *************************************************************************************************
void ForthEditor::exec1(const std::string &script)
{
  typedef std::chrono::nanoseconds ns;
  typedef std::chrono::high_resolution_clock Time;
  std::pair<bool, std::string> res;
  SimTaDynContext& simtadyn = SimTaDynContext::getInstance();

  // Exec the Forth script and  measure the execution time
  auto t0 = Time::now();
  res = simtadyn.m_forth.eatString(script);
  auto t1 = Time::now();

  // Clear the old text in the "Result" tab of the notebook
  Glib::RefPtr<Gtk::TextBuffer> buf = m_result.get_buffer();
  buf->erase(buf->begin(), buf->end());

  if (res.first)
    {
      m_elapsed_time = std::chrono::duration_cast<ns>(t1 - t0);
      m_statusbar.push("OK ! Done in " + elapsedTime());

      // Paste the script Forth result in the "Result" tab of the notebook
      buf->insert(buf->end(), res.second);

      // Copy paste the Forth script into the historic buffer
      buf = m_historic.get_buffer();
      buf->insert(buf->end(), script);
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
      m_statusbar.push("FAILED");

      //std::pair<size_t, size_t> p = simtadyn.m_forth.READER.cursors();
      buf->insert(buf->end(), "Ambiguous condition from "
                  //+ simtadyn.m_forth.READER.file() + ':'
                  //+ p.first << ':'
                  //+ p.second << ' '
                  + res.second);
      simtadyn.m_forth.restore();
    }
}

// *************************************************************************************************
//
// *************************************************************************************************
void ForthEditor::exec()
{
  TextDocument* doc = document();

  if (nullptr != doc)
    {
      ForthEditor::exec1(ForthEditor::text().raw());
    }
  else
    {
      m_statusbar.push("Please, feed me with a Forth script !");
    }
}
