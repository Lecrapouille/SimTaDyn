#include "TextEditor.hpp"

// *************************************************************************************************
//
// *************************************************************************************************
CloseLabel::CloseLabel(std::string const& text)
  : tab_label(text),
    close_button(),
    image(Gtk::Stock::CLOSE, Gtk::ICON_SIZE_MENU)
{
  close_button.add(image);
  close_button.set_relief(Gtk::RELIEF_NONE);
  close_button.set_focus_on_click(false);
  close_button.signal_clicked().connect(sigc::mem_fun(*this, &CloseLabel::on_close_button_clicked));
  pack_start(tab_label);
  pack_start(close_button);
  show_all();
}

void CloseLabel::on_close_button_clicked()
{
  std::cout << "on_close_button_clicked" << std::endl;
  //Gtk::NoteBook* nb = dynamic_cast<Gtk::NoteBook*>(get_ancestor());
  //nb->remove_page();
}

// *************************************************************************************************
//
// *************************************************************************************************
TextDocument::TextDocument()
  : m_filename(""),
    m_findwindow(nullptr)
{
  Gtk::ScrolledWindow::add(m_textview);
  Gtk::ScrolledWindow::set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
  //m_textview.get_buffer()->signal_changed().connect(sigc::mem_fun(this, &TextDocument::onChanged));
}

// *************************************************************************************************
// FIXME: never called
// *************************************************************************************************
TextDocument::~TextDocument()
{
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
//
// *************************************************************************************************
void FindWindow::find(Glib::ustring const& text, Gtk::TextBuffer::iterator& iter)
{
  Gtk::TextBuffer::iterator mstart, mend;

  if (iter.forward_search(text, Gtk::TEXT_SEARCH_TEXT_ONLY, mstart, mend))
    {
      m_ancestor->m_textview.get_buffer()->select_range(mstart, mend);
      m_ancestor->m_textview.get_buffer()->create_mark("last_pos", mend, false);
    }
  else
    {
      std::cout << "Not found\n";
      Gtk::MessageDialog dialog(*this, "End of search !");
      dialog.set_secondary_text("Not found");
      dialog.run();
    }
}

void FindWindow::findNext()//Gtk::Entry* entry)//Glib::ustring const& text)
{
  Gtk::TextBuffer::iterator iter;
  Glib::RefPtr<Gtk::TextBuffer::Mark> last_pos;

  last_pos = m_ancestor->m_textview.get_buffer()->get_mark("last_pos");
  if (0 == last_pos)
    {
      findFirst();
      return ;
    }
  iter = m_ancestor->m_textview.get_buffer()->get_iter_at_mark(last_pos);
  FindWindow::find(entry.get_text(), iter);
}

void FindWindow::findFirst()//Gtk::Entry* entry)//Glib::ustring const& text)
{
  Gtk::TextBuffer::iterator mstart = m_ancestor->m_textview.get_buffer()->begin();
  FindWindow::find(entry.get_text(), mstart);
}

FindWindow::FindWindow(TextDocument* ancestor)
  : search("Search"),
    next("Next"),
    m_ancestor(ancestor)
{
  hbox.pack_start(entry);
  hbox.pack_start(search);
  hbox.pack_start(next);
  vbox.pack_start(hbox);
  add(vbox);
  show_all_children();

  search.signal_clicked().connect(sigc::mem_fun(*this, &FindWindow::findFirst));
  next.signal_clicked().connect(sigc::mem_fun(*this, &FindWindow::findNext));

  //search.signal_clicked().connect(sigc::bind<Gtk::Entry*>(sigc::mem_fun(*this, &FindWindow::findFirst), &entry));
  //next.signal_clicked().connect(sigc::bind<Gtk::Entry*>(sigc::mem_fun(*this, &TextDocument::findNext), &entry));
}

void TextDocument::findWindowClose()
{
  m_findwindow = nullptr;
}

void TextDocument::find()
{
  if (nullptr != m_findwindow)
    return ;

  m_findwindow = new FindWindow(this);
  m_findwindow->signal_hide().connect(sigc::mem_fun(*this, &TextDocument::findWindowClose));

  m_findwindow->show();
}




//void TextDocument::onChanged()
//{
//}

// *************************************************************************************************
// Erase all text in the document
// *************************************************************************************************
void TextDocument::clear()
{
  m_textview.get_buffer()->erase(m_textview.get_buffer()->begin(),
                                 m_textview.get_buffer()->end());
}

// *************************************************************************************************
// Check if the document need to be saved. If the doc needs to be saved, a popup is created
// to prevent the user and depending on the user choice save or not the document.
// Return a bool if the document has been correctly saved.
// *************************************************************************************************
bool TextDocument::questionSave()
{
  if (TextDocument::isModified())
    {
      Gtk::MessageDialog dialog("The document '" + m_title + "' has been modified. Do you want to save it now ?",
                                false, Gtk::MESSAGE_QUESTION, Gtk::BUTTONS_YES_NO);
      dialog.add_button(Gtk::Stock::SAVE_AS, Gtk::RESPONSE_APPLY);

      int result = dialog.run();
      if (Gtk::RESPONSE_YES == result)
        {
          if (0 == m_filename.compare(""))
            {
              return TextDocument::winSaveAs();
            }
          else
            {
              return TextDocument::save();
            }
        }
      else if (Gtk::RESPONSE_APPLY == result)
        {
          return TextDocument::winSaveAs();
        }
    }

  return !TextDocument::isModified();
}

// *************************************************************************************************
//
// *************************************************************************************************
bool TextDocument::winSaveAs()
{
  Gtk::FileChooserDialog dialog("Please choose a file to save as", Gtk::FILE_CHOOSER_ACTION_SAVE);
  //dialog.set_transient_for(*this);

  // Add response buttons the the dialog:
  dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
  dialog.add_button(Gtk::Stock::SAVE_AS, Gtk::RESPONSE_OK);

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
      return TextDocument::saveAs(dialog.get_filename());
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
  std::cout << "DDDDDDDDDDEstructor TextEditor::~TextEditor()\n";
  document()->questionSave();
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
  //m_notebook.set_tab_label_text(*(m_notebook.get_nth_page(m_notebook.get_current_page())), title);
}

// *************************************************************************************************
//
// *************************************************************************************************
void TextEditor::addEmptyTab()
{
  TextDocument *doc = new TextDocument();
  CloseLabel *label = new CloseLabel("New Forth Script");

  m_notebook.append_page(*doc, *label);
  document()->m_textview.get_buffer()->set_text("1 1 + .");
  m_notebook.show_all();
  m_notebook.set_current_page(-1);

  document()->find();
}

// *************************************************************************************************
//
// *************************************************************************************************
void TextEditor::addFileTab(std::string const& filename)
{
  TextDocument *doc = new TextDocument();

  doc->load(filename);

  CloseLabel *label = new CloseLabel(doc->m_title);
  m_notebook.append_page(*doc, *label);

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
  TextDocument* doc = document();

  std::cout << "TextEditor::saveCurrentTab()\n";
  if (nullptr != doc)
    {
      if (0 == doc->m_filename.compare(""))
        {
          doc->winSaveAs();
          title(doc->m_title);
        }
      else
        {
          doc->save();
        }
    }
}

// *************************************************************************************************
//
// *************************************************************************************************
void TextEditor::saveCurrentTabAs()
{
  TextDocument* doc = document();

  std::cout << "TextEditor::saveCurrentTabAs()\n";
  if (nullptr != doc)
    {
      doc->winSaveAs();
      title(doc->m_title);
    }
}

// *************************************************************************************************
// Return the UTF8 string from the current text editor
// *************************************************************************************************
Glib::ustring TextEditor::text()
{
  TextDocument* doc = document();

  if (nullptr != doc)
    {
      return doc->m_textview.get_buffer()->get_text();
    }
  else
    {
      return Glib::ustring("");
    }
}

// *************************************************************************************************
// Erase all text in the current text editor
// *************************************************************************************************
void TextEditor::clear()
{
  TextDocument* doc = document();

  if (nullptr != doc)
    {
      doc->clear();
    }
}

// *************************************************************************************************
// Get all text in the current text editor and give it to the Forth interpreter
// Return true if the code was interpreted correctly, else return false.
// *************************************************************************************************
bool ForthEditor::execForth(Forth& forth)
{
  return forth.eatString(text().raw());
}
