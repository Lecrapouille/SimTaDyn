#include "TextEditor.hpp"

// *************************************************************************************************
// Create a kind of dialog window for searching a string inside a text document.
// *************************************************************************************************
FindWindow::FindWindow(Gtk::TextView* document)
  : m_label("Search:"),
    m_search("First"),
    m_next("Next"),
    m_document(document)
{
  m_hbox.pack_start(m_label);
  m_hbox.pack_start(m_entry);
  m_hbox.pack_start(m_search);
  m_hbox.pack_start(m_next);
  m_vbox.pack_start(m_hbox);
  m_vbox.pack_start(m_status);
  add(m_vbox);
  set_title("SimTaForth Search");
  show_all_children();

  //m_entry.signal_activate().connect(sigc::mem_fun(*this, &FindWindow::clearMessage));
  m_search.signal_clicked().connect(sigc::mem_fun(*this, &FindWindow::findFirst));
  m_next.signal_clicked().connect(sigc::mem_fun(*this, &FindWindow::findNext));
}

// *************************************************************************************************
// When switching a notebook page, and if the search dialog is present, change the reference of the
// document to search in.
// *************************************************************************************************
void FindWindow::document(Gtk::TextView* document)
{
  m_document = document;
}

// *************************************************************************************************
// Clear the text "Found/Not found"
// *************************************************************************************************
/*void FindWindow::clearMessage()
{
  std::cout << "clearMessage\n";
  m_status.set_text("");
  }*/

// *************************************************************************************************
//
// *************************************************************************************************
void FindWindow::find(Glib::ustring const& text, Gtk::TextBuffer::iterator& iter)
{
  Gtk::TextBuffer::iterator mstart, mend;
  Glib::RefPtr<Gtk::TextBuffer::Mark> mark;

  if (iter.forward_search(text, Gtk::TEXT_SEARCH_TEXT_ONLY, mstart, mend))
    {
      m_document->get_buffer()->select_range(mstart, mend);
      mark = m_document->get_buffer()->create_mark("last_pos", mend, false);
      m_document->scroll_mark_onscreen(mark);
      m_status.set_text("Found: yes");
    }
  else
    {
      m_status.set_text("No more found");
    }
}

// *************************************************************************************************
// Search the word occurence
// *************************************************************************************************
void FindWindow::findFirst()
{
  if (nullptr != m_document)
    {
      Gtk::TextBuffer::iterator mstart = m_document->get_buffer()->begin();
      FindWindow::find(m_entry.get_text(), mstart);
    }
  else
    {
      m_status.set_text("No more found");
    }
}

// *************************************************************************************************
// Search the next occurence
// *************************************************************************************************
void FindWindow::findNext()
{
  if (nullptr != m_document)
    {
      Gtk::TextBuffer::iterator iter;
      Glib::RefPtr<Gtk::TextBuffer::Mark> last_pos;

      last_pos = m_document->get_buffer()->get_mark("last_pos");
      if (0 == last_pos)
        {
          findFirst();
          return ;
        }
      iter = m_document->get_buffer()->get_iter_at_mark(last_pos);
      FindWindow::find(m_entry.get_text(), iter);
    }
  else
    {
      m_status.set_text("Not found");
    }
}

// *************************************************************************************************
// A notebook as tabs but Gtkmm does not give tab with a closing button. This is the aim of CloseLabel
// *************************************************************************************************
CloseLabel::CloseLabel(std::string const& text)
  : m_label(text),
    m_button(),
    m_image(Gtk::Stock::CLOSE, Gtk::ICON_SIZE_MENU)
{
  m_button.add(m_image);
  m_button.set_relief(Gtk::RELIEF_NONE);
  m_button.set_focus_on_click(false);
  m_button.signal_clicked().connect(sigc::mem_fun(*this, &CloseLabel::onClicked));
  pack_start(m_label);
  pack_start(m_button);
  show_all();
}

// *************************************************************************************************
// Return the text of the button (here the filename of the document)
// *************************************************************************************************
Glib::ustring CloseLabel::label()
{
  return m_label.get_text().raw();
}

// *************************************************************************************************
// Change the text of the button
// *************************************************************************************************
void CloseLabel::label(std::string const& text)
{
  m_label.set_text(text);
}

// *************************************************************************************************
//
// *************************************************************************************************
void CloseLabel::onClicked()
{
  m_notebook->remove_page(*m_widget);
}

// *************************************************************************************************
// Kind of button.signal_clicked().connect(sigc::bind<T>(sigc::mem_fun(*this, &CloseLabel::onClicked), T));
// *************************************************************************************************
void CloseLabel::link(Gtk::Notebook *notebook, Gtk::Widget *widget)
{
  m_notebook = notebook;
  m_widget = widget;
}

// *************************************************************************************************
//
// *************************************************************************************************
TextDocument::TextDocument()
  : m_button(""),
    m_filename("")
{
  Gtk::ScrolledWindow::add(m_textview);
  Gtk::ScrolledWindow::set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);

  m_textview.get_buffer()->signal_changed().connect(sigc::mem_fun(this, &TextDocument::onChanged));
}

// *************************************************************************************************
// FIXME: never called
// *************************************************************************************************
TextDocument::~TextDocument()
{
  //TextDocument::dialogSave();
  //save();
}

// *************************************************************************************************
// Erase all text in the document
// *************************************************************************************************
void TextDocument::clear()
{
  m_textview.get_buffer()->erase(m_textview.get_buffer()->begin(),
                                 m_textview.get_buffer()->end());
}

// *************************************************************************************************
// Return if the document has been changed and need to be saved
// *************************************************************************************************
bool TextDocument::isModified() const
{
  return m_textview.get_buffer()->get_modified();
}

// *************************************************************************************************
// TODO: for undo/redo actions
// *************************************************************************************************
void TextDocument::onChanged()
{
  std::cout << "Text changed\n";
  // ajouter un "*" au titre
}

// *************************************************************************************************
// Save the document defined by the private field m_filename
// Return a bool if the document has been correctly saved.
// *************************************************************************************************
bool TextDocument::save()
{
  bool res = true;

  if (TextDocument::isModified())
    {
      std::ofstream outfile;

      // FIXME BUG si  m_filename == ""
      outfile.open(m_filename, std::fstream::out);
      if (outfile)
        {
          outfile << m_textview.get_buffer()->get_text();
          outfile.close();
          m_textview.get_buffer()->set_modified(false);
        }
      else
        {
          res = false;
        }
    }
  return res;
}

// *************************************************************************************************
// Save as. Same idea than save but use the filename given as param as new file
// *************************************************************************************************
bool TextDocument::saveAs(std::string const& filename)
{
  std::string title = filename.substr(filename.find_last_of("/") + 1);
  m_button.label(title);
  m_filename = filename;
  return TextDocument::save();
}

// *************************************************************************************************
// Open a filename and get its content. If append is false, replace the old content by the content
// of the file. Note: we do not popup a dialog to ask if needed saving (TBD: bool save_before_otrunc)
// *************************************************************************************************
bool TextDocument::load(std::string const& filename, bool clear)
{
  std::ifstream infile;
  std::string line, base_name;

  if (clear)
    {
      TextDocument::clear();
      m_filename = filename;
      std::string title = filename.substr(filename.find_last_of("/") + 1);
      m_button.label(title);
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

  if (clear)
    {
      m_textview.get_buffer()->set_modified(false);
    }

  return true;
}

// *************************************************************************************************
//
// *************************************************************************************************
TextEditor::TextEditor()
  : m_findwindow(nullptr),
    m_nb_nonames(0)
{
  m_notebook.set_scrollable();
  m_notebook.signal_switch_page().connect(sigc::mem_fun(*this, &TextEditor::onPageSwitched));
}

// *************************************************************************************************
//
// *************************************************************************************************
TextEditor::~TextEditor()
{
  for (int k = 0; k < m_notebook.get_n_pages(); ++k)
    {
      m_notebook.set_current_page(k);
      TextDocument *doc = TextEditor::document();
      if (nullptr != doc)
        {
          if (doc->isModified())
            dialogSave(doc);
        }
    }
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
  if (nullptr == widget)
    {
      return nullptr;
    }
  return dynamic_cast<TextDocument*>(widget);
}

// *************************************************************************************************
// Check if the document need to be saved. If the doc needs to be saved, a popup is created
// to prevent the user and depending on the user choice save or not the document.
// Return a bool if the document has been correctly saved.
// *************************************************************************************************
bool TextEditor::dialogSave(TextDocument *doc)
{
  Gtk::MessageDialog dialog("The document '" + doc->m_button.label() + "' has been modified. Do you want to save it now ?",
                            false, Gtk::MESSAGE_QUESTION, Gtk::BUTTONS_YES_NO);
  dialog.add_button(Gtk::Stock::SAVE_AS, Gtk::RESPONSE_APPLY);

  int result = dialog.run();
  if (Gtk::RESPONSE_YES == result)
    {
      if (0 == doc->m_filename.compare(""))
        {
          return TextEditor::saveAs(doc);
        }
      else
        {
          return doc->save();
        }
    }
  else if (Gtk::RESPONSE_APPLY == result)
    {
      return TextEditor::saveAs(doc);
    }
  else // other button
    {
      return !doc->isModified();
    }
}

// *************************************************************************************************
//
// *************************************************************************************************
bool TextEditor::saveAs(TextDocument *doc)
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
      bool ret = doc->saveAs(dialog.get_filename());
      if (ret)
        {
          // m_nb_nonames--;
        }
      return ret;
    }
  return false;
}

// *************************************************************************************************
//
// *************************************************************************************************
void TextEditor::newDocument()
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

      newLoadedDocument(dialog.get_filename());
    }
}

// *************************************************************************************************
//
// *************************************************************************************************
void TextEditor::onPageSwitched(GtkNotebookPage* page, guint page_num)
{
  (void) page;
  m_findwindow.document(&(TextEditor::document(page_num)->m_textview));
}

// *************************************************************************************************
//
// *************************************************************************************************
void TextEditor::newEmptyDocument()
{
  TextDocument *doc = new TextDocument();

  m_nb_nonames++;
  doc->m_button.label("New Forth Script " + std::to_string(m_nb_nonames));
  doc->m_button.link(&m_notebook, doc);

  m_notebook.append_page(*doc, doc->m_button);
  m_notebook.show_all();
  m_notebook.set_current_page(-1);
}

// *************************************************************************************************
//
// *************************************************************************************************
void TextEditor::newTemplatedDocument(std::string const& text)
{
  TextDocument *doc = new TextDocument();

  m_nb_nonames++;
  doc->m_button.label("New Forth Script " + std::to_string(m_nb_nonames));
  doc->m_textview.get_buffer()->set_text(text);
  doc->m_button.link(&m_notebook, doc);

  m_notebook.append_page(*doc, doc->m_button);
  m_notebook.show_all();
  m_notebook.set_current_page(-1);
}

// *************************************************************************************************
//
// *************************************************************************************************
void TextEditor::newLoadedDocument(std::string const& filename)
{
  TextDocument *doc = new TextDocument();

  doc->load(filename);
  doc->m_button.link(&m_notebook, doc);

  m_notebook.append_page(*doc, doc->m_button);
  m_notebook.show_all();
  m_notebook.set_current_page(-1);
}
// *************************************************************************************************
//
// *************************************************************************************************
void TextEditor::saveCurrentDocument()
{
  TextDocument* doc = document();

  if (nullptr != doc)
    {
      if (0 == doc->m_filename.compare(""))
        {
          TextEditor::saveAs(doc);
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
void TextEditor::saveAsCurrentDocument()
{
  TextDocument* doc = document();

  if (nullptr != doc)
    {
      TextEditor::saveAs(doc);
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
// Launch the search window
// *************************************************************************************************
void TextEditor::find()
{
  m_findwindow.show();
}
