#include "ForthEditor.hpp"
#include "SimTaDynContext.hpp"

// *************************************************************************************************
//
// *************************************************************************************************
ForthEditor::ForthEditor()
  : m_cout(std::cout, m_result.get_buffer()),
    m_cerr(std::cerr, m_result.get_buffer())
{
  // Menus '_Forth Scripts'
  {
    m_menuitem[1].set_label("Forth");
    m_menuitem[1].set_submenu(m_menu[1]);

    //
    m_submenu[0].set_label("New Script");
    m_image[0].set_from_icon_name("document-new", Gtk::ICON_SIZE_MENU);
    m_submenu[0].set_image(m_image[0]);
    m_submenu[0].signal_activate().connect(sigc::mem_fun(*this, &ForthEditor::empty));
    m_menu[1].append(m_submenu[0]);

    //
    m_submenu[1].set_label("New Templated Script");
    m_image[1].set_from_icon_name("text-x-generic-template", Gtk::ICON_SIZE_MENU);
    m_submenu[1].set_image(m_image[1]);
    m_submenu[1].signal_activate().connect(sigc::mem_fun(*this, &ForthEditor::templated));
    m_menu[1].append(m_submenu[1]);

    //
    m_submenu[2].set_label("Interactive");
    m_image[2].set_from_icon_name("utilities-terminal", Gtk::ICON_SIZE_MENU);
    m_submenu[2].set_image(m_image[2]);
    m_submenu[2].signal_activate().connect(sigc::mem_fun(*this, &ForthEditor::empty));// TODO
    m_menu[1].append(m_submenu[2]);

    //
    m_menu[1].append(m_menuseparator[0]);

    //
    m_submenu[3].set_label("Open Script");
    m_image[3].set_from_icon_name("document-open", Gtk::ICON_SIZE_MENU);
    m_submenu[3].set_image(m_image[3]);
    m_submenu[3].signal_activate().connect(sigc::mem_fun0(*this, &TextEditor::open));
    m_menu[1].append(m_submenu[3]);

    //
    m_submenu[4].set_label("Save Script");
    m_image[4].set_from_icon_name("document-save", Gtk::ICON_SIZE_MENU);
    m_submenu[4].set_image(m_image[4]);
    m_submenu[4].signal_activate().connect(sigc::mem_fun(*this, &ForthEditor::save));
    m_menu[1].append(m_submenu[4]);

    //
    m_submenu[5].set_label("Save As Script");
    m_image[5].set_from_icon_name("document-save-as", Gtk::ICON_SIZE_MENU);
    m_submenu[5].set_image(m_image[5]);
    m_submenu[5].signal_activate().connect(sigc::mem_fun0(*this, &ForthEditor::saveAs));
    m_menu[1].append(m_submenu[5]);

    //
    m_menu[1].append(m_menuseparator[1]);

    //
    m_submenu[9].set_label("Load Dictionary");
    m_image[9].set_from_icon_name("document-open", Gtk::ICON_SIZE_MENU);
    m_submenu[9].set_image(m_image[9]);
    m_submenu[9].signal_activate().connect(sigc::mem_fun(*this, &ForthEditor::loadDictionary));
    m_menu[1].append(m_submenu[9]);

    //
    m_submenu[10].set_label("Save As Dictionary");
    m_image[10].set_from_icon_name("document-save-as", Gtk::ICON_SIZE_MENU);
    m_submenu[10].set_image(m_image[10]);
    m_submenu[10].signal_activate().connect(sigc::mem_fun(*this, &ForthEditor::dumpDictionary));
    m_menu[1].append(m_submenu[10]);
  }

  // Forth dictionary display
  {
    m_ref_tree_model = Gtk::ListStore::create(m_columns);
    m_dico.set_model(m_ref_tree_model);

    m_dico.append_column("Word", m_columns.m_word);
    m_dico.append_column("Token", m_columns.m_token);

    // Test
    Gtk::TreeModel::Row row = *(m_ref_tree_model->append());
    row[m_columns.m_word] = "Billy";
    row[m_columns.m_token] = 42;

    row = *(m_ref_tree_model->append());
    row[m_columns.m_word] = "Foo";
    row[m_columns.m_token] = 43;
  }

  // Forth text view for storing results, debug, historic
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
void ForthEditor::dumpDictionary()
{
  Gtk::FileChooserDialog dialog("Choose a binary file to save Forth dictionary",
                                Gtk::FILE_CHOOSER_ACTION_SAVE);
  dialog.set_transient_for((Gtk::Window&) (*m_notebook.get_toplevel()));

  // Add response buttons the the dialog:
  dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
  dialog.add_button(Gtk::Stock::SAVE_AS, Gtk::RESPONSE_OK);

  // Add filters, so that only certain file types can be selected:
  auto filter_forth = Gtk::FileFilter::create();
  filter_forth->set_name("Forth dictionary files");
  filter_forth->add_pattern("*.simdico");
  dialog.add_filter(filter_forth);

  auto filter_any = Gtk::FileFilter::create();
  filter_any->set_name("Any files");
  filter_any->add_pattern("*");
  dialog.add_filter(filter_any);

  int result = dialog.run();
  if (Gtk::RESPONSE_OK == result)
    {
      SimTaDynContext& simtadyn = SimTaDynContext::getInstance();
      simtadyn.m_forth.dictionary().dump(dialog.get_filename());
      // FIXME return not taken into account
    }
}

// *************************************************************************************************
//
// *************************************************************************************************
void ForthEditor::loadDictionary()
{
  Gtk::FileChooserDialog dialog("Choose a binary file to save Forth dictionary",
                                Gtk::FILE_CHOOSER_ACTION_OPEN);
  dialog.set_transient_for((Gtk::Window&) (*m_notebook.get_toplevel()));

  // Add response buttons the the dialog:
  dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
  dialog.add_button(Gtk::Stock::OPEN, Gtk::RESPONSE_OK);

  // Add filters, so that only certain file types can be selected:
  auto filter_forth = Gtk::FileFilter::create();
  filter_forth->set_name("Forth dictionary files");
  filter_forth->add_pattern("*.simdico");
  dialog.add_filter(filter_forth);

  auto filter_any = Gtk::FileFilter::create();
  filter_any->set_name("Any files");
  filter_any->add_pattern("*");
  dialog.add_filter(filter_any);

  int result = dialog.run();
  if (Gtk::RESPONSE_OK == result)
    {
      SimTaDynContext& simtadyn = SimTaDynContext::getInstance();
      simtadyn.m_forth.dictionary().load(dialog.get_filename());
      // FIXME return not taken into account
    }
}

// *************************************************************************************************
//
// *************************************************************************************************
std::string ForthEditor::elapsedTime()
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

  // Clear the old text in the "Result" tab of the notebook
  Glib::RefPtr<Gtk::TextBuffer> buf = m_result.get_buffer();
  buf->erase(buf->begin(), buf->end());

  // Exec the Forth script and  measure the execution time
  auto t0 = Time::now();
  res = simtadyn.m_forth.interpreteString(script, document()->filename());
  auto t1 = Time::now();

  // Flush the std::cout in the textview
  m_cout.flush();
  m_cerr.flush();

  if (res.first)
    {
      m_elapsed_time = std::chrono::duration_cast<ns>(t1 - t0);
      m_statusbar.push(elapsedTime());

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

      // Show the faulty document
      TextEditor::open1(simtadyn.m_forth.m_streams_stack[simtadyn.m_forth.m_err_stream].name());

      // Show res (redirect sdout to gui)
      simtadyn.m_forth.ok(res);
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
