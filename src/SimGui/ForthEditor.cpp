#include "ForthEditor.hpp"
#include "SimTaDynContext.hpp"

// *************************************************************************************************
//
// *************************************************************************************************
ForthEditor::ForthEditor()
{
  // Menus '_Forth'
  {
    m_menuitem.set_label("Forth");
    m_menuitem.set_submenu(m_menu);

    //
    m_submenu[0].set_label("New Forth document");
    m_image[0].set_from_icon_name("document-new", Gtk::ICON_SIZE_MENU);
    m_submenu[0].set_image(m_image[0]);
    m_submenu[0].signal_activate().connect(sigc::mem_fun(*this, &ForthEditor::empty));
    m_menu.append(m_submenu[0]);

    //
    m_submenu[1].set_label("New Forth templated document");
    m_image[1].set_from_icon_name("text-x-generic-template", Gtk::ICON_SIZE_MENU);
    m_submenu[1].set_image(m_image[1]);
    m_submenu[1].signal_activate().connect(sigc::mem_fun(*this, &ForthEditor::templated));
    m_menu.append(m_submenu[1]);

    //
    m_submenu[2].set_label("Interactive Forth");
    m_image[2].set_from_icon_name("utilities-terminal", Gtk::ICON_SIZE_MENU);
    m_submenu[2].set_image(m_image[2]);
    m_submenu[2].signal_activate().connect(sigc::mem_fun(*this, &ForthEditor::empty));// TODO
    m_menu.append(m_submenu[2]);

    //
    m_menu.append(m_menuseparator[0]);

    //
    m_submenu[3].set_label("Open Forth document");
    m_image[3].set_from_icon_name("document-open", Gtk::ICON_SIZE_MENU);
    m_submenu[3].set_image(m_image[3]);
    m_submenu[3].signal_activate().connect(sigc::mem_fun(*this, &ForthEditor::open));
    m_menu.append(m_submenu[3]);

    //
    m_submenu[4].set_label("Save");
    m_image[4].set_from_icon_name("document-save", Gtk::ICON_SIZE_MENU);
    m_submenu[4].set_image(m_image[4]);
    m_submenu[4].signal_activate().connect(sigc::mem_fun(*this, &ForthEditor::save));
    m_menu.append(m_submenu[4]);

    //
    m_submenu[5].set_label("Save As");
    m_image[5].set_from_icon_name("document-save-as", Gtk::ICON_SIZE_MENU);
    m_submenu[5].set_image(m_image[5]);
    m_submenu[5].signal_activate().connect(sigc::mem_fun0(*this, &ForthEditor::saveAs));
    m_menu.append(m_submenu[5]);

    //
    m_menu.append(m_menuseparator[1]);

    //
    m_submenu[6].set_label("Find");
    m_image[6].set_from_icon_name("edit-find", Gtk::ICON_SIZE_MENU);
    m_submenu[6].set_image(m_image[6]);
    m_submenu[6].signal_activate().connect(sigc::mem_fun(*this, &ForthEditor::find));
    m_menu.append(m_submenu[6]);
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
void ForthEditor::empty()
{
  TextEditor::empty("New Forth script");
}

// *************************************************************************************************
//
// *************************************************************************************************
void ForthEditor::templated()
{
  ForthEditor::empty();
  TextEditor::document()->buffer()->set_text(": COMPUTE-ME 1 1 + . ;\nCOMPUTE-ME");
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
