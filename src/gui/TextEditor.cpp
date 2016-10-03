#include "TextEditor.hpp"

// *************************************************************************************************
//
// *************************************************************************************************
TextDocument::TextDocument()
  : m_filename("")
{
  Gtk::ScrolledWindow::add(m_textview);
  Gtk::ScrolledWindow::set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
}

// *************************************************************************************************
//
// *************************************************************************************************
TextDocument::~TextDocument()
{
  #warning "Jamais appelle"
  std::cout << "DEstructor TextDocument::~TextDocument()\n";
  TextDocument::questionSave();
}

// *************************************************************************************************
// Return if the document has been changed and need to be saved
// *************************************************************************************************
bool TextDocument::isModified() const
{
  return m_textview.get_buffer()->get_modified();
}

// *************************************************************************************************
// Check if the document need to be save. If the doc needs to be saved, a popup is created
// to prevent the user and depending on the user choice save or not the document.
// Return a bool if the document has been correctly saved.
// *************************************************************************************************
bool TextDocument::questionSave()
{
  if (TextDocument::isModified())
    {
      Gtk::MessageDialog dialog("Buffer Modified. Do you want to save it now ?", false,
                                Gtk::MESSAGE_QUESTION, Gtk::BUTTONS_YES_NO);

      int result = dialog.run();
      if (Gtk::RESPONSE_YES == result)
        {
          if (0 == m_filename.compare("")) // FIXME || bouton SAVE_AS
            {
              return TextDocument::winSaveAs();
            }
          else
            {
              return TextDocument::save();
            }
        }
    }

  return !TextDocument::isModified();
}

// *************************************************************************************************
//
// *************************************************************************************************
bool TextDocument::winSaveAs()
{
  Gtk::FileChooserDialog dialog("Please choose a file to save as", Gtk::FILE_CHOOSER_ACTION_OPEN);
  //dialog.set_transient_for(*this);

  // Add response buttons the the dialog:
  dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
  dialog.add_button(Gtk::Stock::OPEN, Gtk::RESPONSE_OK);

  // Add filters, so that only certain file types can be selected:
  Gtk::FileFilter filter_forth;
  filter_forth.set_name("Forth files");
  filter_forth.add_pattern("*.fs");
  filter_forth.add_pattern("*.fth");
  filter_forth.add_pattern("*.4th");
  filter_forth.add_pattern("*.forth");
  dialog.add_filter(filter_forth);

  Gtk::FileFilter filter_text;
  filter_text.set_name("Text files");
  filter_text.add_mime_type("text/plain");
  dialog.add_filter(filter_text);

  Gtk::FileFilter filter_any;
  filter_any.set_name("Any files");
  filter_any.add_pattern("*");
  dialog.add_filter(filter_any);

  int result = dialog.run();
  if (Gtk::RESPONSE_OK == result)
    {
      return saveAs(dialog.get_filename());
    }
  return false;
}

// *************************************************************************************************
// Save the document defined by the private field m_filename
// Return a bool if the document has been correctly saved.
// *************************************************************************************************
bool TextDocument::save()
{
  if (TextDocument::isModified())
    {
      std::ofstream outfile;

      outfile.open(m_filename, std::fstream::out);
      if (outfile)
        {
          outfile << m_textview.get_buffer()->get_text();
          outfile.close();
          m_textview.get_buffer()->set_modified(false);
        }
    }
  return !TextDocument::isModified();
}

// *************************************************************************************************
// Save as. Same idea than save but use the filename given as param as new file
// *************************************************************************************************
bool TextDocument::saveAs(std::string const& filename)
{
  m_filename = filename;
  m_title = filename.substr(filename.find_last_of("/") + 1);
  return TextDocument::save();
}

// *************************************************************************************************
//
// *************************************************************************************************
bool TextDocument::load(std::string const& filename)
{
  std::ifstream infile;
  std::string line, base_name;

  if (TextDocument::isModified())
    {
      TextDocument::questionSave();
    }

  infile.open(filename, std::fstream::in);
  if (!infile)
    return false;

  while (std::getline(infile, line))
    {
      line = line + '\n';
      m_textview.get_buffer()->insert(m_textview.get_buffer()->end(), line);
    }
  infile.close();

  m_filename = filename;
  m_title = filename.substr(filename.find_last_of("/") + 1);
  m_textview.get_buffer()->set_modified(false);

  return true;
}

// *************************************************************************************************
//
// *************************************************************************************************
TextEditor::TextEditor()
{
  m_notebook.set_scrollable();
}

// *************************************************************************************************
//
// *************************************************************************************************
TextEditor::~TextEditor()
{
  std::cout << "DEstructor TextEditor::~TextEditor()\n";
}

// *************************************************************************************************
//
// *************************************************************************************************
TextDocument* TextEditor::document()
{
  int page = m_notebook.get_current_page();
  Gtk::Widget *widget = m_notebook.get_nth_page(page);
  if (NULL == widget)
    {
      return nullptr;
    }
  return dynamic_cast<TextDocument*>(widget);
}
// *************************************************************************************************
//
// *************************************************************************************************
TextDocument* TextEditor::document(const uint32_t i)
{
  Gtk::Widget *widget = m_notebook.get_nth_page(i);
  if (NULL == widget)
    {
      return nullptr;
    }
  return dynamic_cast<TextDocument*>(widget);
}


// *************************************************************************************************
//
// *************************************************************************************************
const std::string TextEditor::title()
{
  return m_notebook.get_tab_label_text(*(m_notebook.get_nth_page(m_notebook.get_current_page())));
}

// *************************************************************************************************
//
// *************************************************************************************************
void TextEditor::title(std::string const& title)
{
  m_notebook.set_tab_label_text(*(m_notebook.get_nth_page(m_notebook.get_current_page())), title);
}

// *************************************************************************************************
//
// *************************************************************************************************
void TextEditor::addEmptyTab()
{
  TextDocument *doc = new TextDocument();

  m_notebook.append_page(*doc, "New Forth Script");
  document()->m_textview.get_buffer()->set_text("1 1 + .");

  m_notebook.show_all();
  m_notebook.set_current_page(-1);
}

// *************************************************************************************************
//
// *************************************************************************************************
void TextEditor::addFileTab(std::string const& filename)
{
  TextDocument *doc = new TextDocument();
  doc->load(filename);

  m_notebook.append_page(*doc, doc->m_title);

  m_notebook.show_all();
  m_notebook.set_current_page(-1);
}

// *************************************************************************************************
//
// *************************************************************************************************
void TextEditor::addFileTab()
{
  Gtk::FileChooserDialog dialog("Please choose a file to open", Gtk::FILE_CHOOSER_ACTION_OPEN);
  //dialog.set_transient_for(*this);

  // Add response buttons the the dialog:
  dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
  dialog.add_button(Gtk::Stock::OPEN, Gtk::RESPONSE_OK);

  // Add filters, so that only certain file types can be selected:
  Gtk::FileFilter filter_text;
  filter_text.set_name("Text files");
  filter_text.add_mime_type("text/plain");
  dialog.add_filter(filter_text);

  Gtk::FileFilter filter_forth;
  filter_forth.set_name("Forth files");
  filter_forth.add_pattern("*.fs");
  filter_forth.add_pattern("*.fth");
  filter_forth.add_pattern("*.4th");
  filter_forth.add_pattern("*.forth");
  dialog.add_filter(filter_forth);

  Gtk::FileFilter filter_any;
  filter_any.set_name("Any files");
  filter_any.add_pattern("*");
  dialog.add_filter(filter_any);

  int result = dialog.run();
  if (Gtk::RESPONSE_OK == result)
    {
      // Already opened ? Switch the page
      for (int k = 0; k < m_notebook.get_n_pages(); ++k)
        {
          if (0 == document(k)->m_filename.compare(dialog.get_filename()))
            {
              std::cout << "Already opened\n"; // TODO statusbar
              m_notebook.set_current_page(k);
              return ;
            }
        }

      addFileTab(dialog.get_filename());
    }
}

// *************************************************************************************************
//
// *************************************************************************************************
void TextEditor::saveCurrentTab()
{
  if (0 == document()->m_filename.compare(""))
    {
      document()->winSaveAs();
    }
  else
    {
      document()->save();
    }
}

// *************************************************************************************************
//
// *************************************************************************************************
void TextEditor::saveCurrentTabAs()
{
  document()->winSaveAs();
}
