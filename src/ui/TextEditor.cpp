//=====================================================================
// SimTaDyn: A GIS in a spreadsheet.
// Copyright 2018 Quentin Quadrat <lecrapouille@gmail.com>
//
// This file is part of SimTaDyn.
//
// SimTaDyn is free software: you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with SimTaDyn.  If not, see <http://www.gnu.org/licenses/>.
//=====================================================================

#include "TextEditor.hpp"
#include <gtkmm/cssprovider.h>
#include <ctype.h>

GotoLineWindow::GotoLineWindow(Gsv::View* document)
  : m_label("Line number:"),
    m_button("Go to line"),
    m_document(document)
{
  m_hbox.pack_start(m_label);
  m_hbox.pack_start(m_entry);
  m_hbox.pack_start(m_button);
  m_vbox.pack_start(m_hbox);
  add(m_vbox);
  set_title("Go to line");

  m_button.signal_clicked().connect(sigc::mem_fun(*this, &GotoLineWindow::gotoLine));

  show_all_children();
}

void GotoLineWindow::gotoLine()
{
  if (nullptr != m_document)
    {
      // Allow only numbers to be entered
      Glib::ustring text = m_entry.get_text();
      for (uint32_t i = 0; i < text.length(); ++i)
        {
          if (Glib::Unicode::isdigit(text[i]) == false)
            return ;
          // FIXME: display error
        }

      // Go to line
      int line = std::stoi(text.c_str());
      Glib::RefPtr<Gtk::TextBuffer> buf = m_document->get_buffer();
      Gtk::TextBuffer::iterator iter = buf->get_iter_at_line(line);
      m_document->scroll_to(iter);
      // FIXME: highligth the line
    }
}

// *************************************************************************************************
// When switching a notebook page, and if the search dialog is present, change the reference of the
// document to search in.
// *************************************************************************************************
void GotoLineWindow::document(Gsv::View* document)
{
  m_document = document;
}

// *************************************************************************************************
//
// *************************************************************************************************
Find::Find(Gsv::View* document)
  : m_document(document),
    m_found(false)
{
}

// *************************************************************************************************
// When switching a notebook page, and if the search dialog is present, change the reference of the
// document to search in.
// *************************************************************************************************
void Find::document(Gsv::View* document)
{
  m_document = document;
  m_found = false;
}

// *************************************************************************************************
//
// *************************************************************************************************
void Find::find(Glib::ustring const& text, Gtk::TextBuffer::iterator& iter)
{
  Glib::RefPtr<Gtk::TextBuffer::Mark> mark;

  m_found = iter.forward_search(text, Gtk::TEXT_SEARCH_TEXT_ONLY, m_start, m_end);
  if (m_found)
    {
      m_document->get_buffer()->select_range(m_start, m_end);
      mark = m_document->get_buffer()->create_mark("last_pos", m_end, false);
      m_document->scroll_to(mark);
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
void Find::findFirst()
{
  if (nullptr != m_document)
    {
      m_start = m_document->get_buffer()->begin();
      Find::find(m_entry.get_text(), m_start);
    }
  else
    {
      m_status.set_text("No more found");
      m_found = false;
    }
}

// *************************************************************************************************
// Search the next occurence
// *************************************************************************************************
void Find::findNext()
{
  if (nullptr != m_document)
    {
      Gtk::TextBuffer::iterator iter;
      Glib::RefPtr<Gtk::TextBuffer::Mark> last_pos;

      last_pos = m_document->get_buffer()->get_mark("last_pos");
      if (!last_pos)
        {
          findFirst();
          return ;
        }
      iter = m_document->get_buffer()->get_iter_at_mark(last_pos);
      Find::find(m_entry.get_text(), iter);
    }
  else
    {
      m_status.set_text("Not found");
      m_found = false;
    }
}

// *************************************************************************************************
// Create a kind of dialog window for searching a string inside a text document.
// *************************************************************************************************
FindWindow::FindWindow(Gsv::View* document)
  : Find(document),
    m_label("Find"),
    m_next("Next")
{
  m_hbox.pack_start(m_label);
  m_hbox.pack_start(m_entry);
  m_hbox.pack_start(m_next);
  m_vbox.pack_start(m_hbox);
  m_vbox.pack_start(m_status);
  add(m_vbox);

  m_next.signal_clicked().connect(sigc::mem_fun(*this, &Find::findNext));

  show_all_children();
}

// *************************************************************************************************
//
// *************************************************************************************************
ReplaceWindow::ReplaceWindow(Gsv::View* document)
  : Find(document),
    m_label("Replace"),
    m_label2("by"),
    m_search("Find"),
    m_replace("Replace"),
    m_all("Replace All")
{
  m_hbox.pack_start(m_label);
  m_hbox.pack_start(m_entry);
  m_hbox.pack_start(m_label2);
  m_hbox.pack_start(m_entry2);
  m_hbox.pack_start(m_search);
  m_hbox.pack_start(m_replace);
  m_hbox.pack_start(m_all);
  m_vbox.pack_start(m_hbox);
  m_vbox.pack_start(m_status);
  add(m_vbox);

  m_search.signal_clicked().connect(sigc::mem_fun0(*this, &ReplaceWindow::find));
  m_replace.signal_clicked().connect(sigc::mem_fun(*this, &ReplaceWindow::replace));
  m_all.signal_clicked().connect(sigc::mem_fun(*this, &ReplaceWindow::replaceAll));

  show_all_children();
}

// *************************************************************************************************
//
// *************************************************************************************************
void ReplaceWindow::find()
{
  Find::findNext();
  if (!m_found)
    {
      Find::findFirst();
    }
}

// *************************************************************************************************
//
// *************************************************************************************************
void ReplaceWindow::replace()
{
  if (!m_found)
    {
      ReplaceWindow::find();
    }
  if (m_found)
    {
      Gtk::TextBuffer::iterator i;
      i = m_document->get_buffer()->erase(m_start, m_end);
      m_document->get_buffer()->insert(i, m_entry2.get_text());
      Find::findNext();
    }
}

// *************************************************************************************************
//
// *************************************************************************************************
void ReplaceWindow::replaceAll()
{
  do {
    ReplaceWindow::replace();
  } while (m_found);
}

// *************************************************************************************************
// A notebook as tabs but Gtkmm does not give tab with a closing button. This is the aim of CloseLabel
// *************************************************************************************************
CloseLabel::CloseLabel(std::string const& text)
  : Box(Gtk::ORIENTATION_HORIZONTAL),
    m_label(text),
    m_button(),
    m_image(Gtk::Stock::CLOSE, Gtk::ICON_SIZE_MENU),
    m_editor(nullptr),
    m_widget(nullptr),
    m_asterisk(false)
{
  set_can_focus(false);
  m_label.set_can_focus(false);
  // m_button.set_image_from_icon_name("window-close-symbolic");
  m_button.add(m_image);
  m_button.set_can_focus(false);
  m_button.set_relief(Gtk::ReliefStyle::RELIEF_NONE);
  m_button.signal_clicked().connect(sigc::mem_fun(*this, &CloseLabel::onClicked));
  m_button.signal_button_press_event().connect(sigc::mem_fun(*this, &CloseLabel::onButtonPressEvent));

  pack_start(m_label, Gtk::PACK_SHRINK);
  pack_end(m_button, Gtk::PACK_SHRINK);
  show_all();
}

// *************************************************************************************************
//
// *************************************************************************************************
void CloseLabel::asterisk(const bool asterisk)
{
  if (m_asterisk != asterisk)
    {
      m_asterisk = asterisk;
      if (m_asterisk)
        {
          m_label.set_text("** " + m_title);
        }
      else
        {
          m_label.set_text(m_title);
        }
    }
}

// *************************************************************************************************
//
// *************************************************************************************************
void CloseLabel::close()
{
  if (nullptr == m_editor)
    return ;
  if (nullptr == m_widget)
    return ;

  if (m_asterisk)
    {
      int page = m_editor->page_num(*m_widget);
      Gtk::Widget *widget = m_editor->get_nth_page(page);
      if (NULL != widget)
        {
          TextDocument* doc = dynamic_cast<TextDocument*>(widget);
          if (nullptr == doc)
            return ;
          if (!m_editor->dialogSave(doc))
            return ;
        }
    }
  m_editor->remove_page(*m_widget);
}

// *************************************************************************************************
//
// *************************************************************************************************
TextDocument::TextDocument(Glib::RefPtr<Gsv::Language> language)
  : Gtk::ScrolledWindow(),
    m_button(""), // FIXME a passer en param
    m_filename("")
{
  LOGI("Creating TextDocument");

  Gtk::ScrolledWindow::add(m_textview);
  Gtk::ScrolledWindow::set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
  // Create the text buffer with syntax coloration
  m_buffer = Gsv::Buffer::create(language);
  m_buffer->set_highlight_syntax(true);
  m_textview.set_source_buffer(m_buffer);
  // Fonts size
  m_textview.override_font(Pango::FontDescription("mono 12"));
  // Behavior/Display of the text view
  gtk_source_view_set_background_pattern(m_textview.gobj(), GTK_SOURCE_BACKGROUND_PATTERN_TYPE_GRID);
  m_textview.set_show_line_numbers(true);
  m_textview.set_show_right_margin(true);
  m_textview.set_highlight_current_line(true);
  m_textview.set_tab_width(1U);
  m_textview.set_indent_width(1U);
  m_textview.set_insert_spaces_instead_of_tabs(true);
  m_textview.set_auto_indent(true);
  m_buffer->signal_changed().connect(sigc::mem_fun(this, &TextDocument::onChanged));
}

// *************************************************************************************************
//
// *************************************************************************************************
TextDocument::~TextDocument()
{
  LOGI("Destroying TextDocument");
}

// *******************************²******************************************************************
// Erase all text in the document
// *************************************************************************************************
void TextDocument::clear()
{
  m_buffer->erase(m_buffer->begin(), m_buffer->end());
}

// *************************************************************************************************
// Return if the document has been changed and need to be saved
// *************************************************************************************************
bool TextDocument::isModified() const
{
  return m_buffer->get_modified();
}

// *************************************************************************************************
// Slot.
// *************************************************************************************************
void TextDocument::onChanged()
{
  // FIXM: if (!read_only)
  m_button.asterisk(true);
}

// *************************************************************************************************
//
// *************************************************************************************************
/*void TextDocument::cursorAt(const uint32_t line, const uint32_t index)
{
  int l = std::min((int) line, m_buffer->get_line_count() - 1);
  Gtk::TextIter iter = m_textview.get_iter_at_line_end(l);
  index = std::min(index, iter.get_line_index());
  m_buffer->place_cursor(m_buffer->get_iter_at_line_index(l, index));
  }*/

// *************************************************************************************************
// Save the document defined by the private field m_filename
// Return a bool if the document has been correctly saved.
// *************************************************************************************************
bool TextDocument::save()
{
  LOGI("TextDocument saving '%s'", m_filename.c_str());
  bool res = true;

  if (TextDocument::isModified())
    {
      std::ofstream outfile;

      // FIXME BUG si  m_filename == ""
      outfile.open(m_filename, std::fstream::out);
      if (outfile)
        {
          outfile << m_buffer->get_text();
          outfile.close();
          modified(false);
          m_button.set_tooltip_text(m_filename);
        }
      else
        {
          std::string why = strerror(errno);
          LOGF("could not save the file '%s' reason was '%s'",
               m_filename.c_str(), why.c_str());
          Gtk::MessageDialog dialog((Gtk::Window&) (*m_textview.get_toplevel()),
                                    "Could not save '" + m_filename + "'",
                                    false, Gtk::MESSAGE_WARNING);
          dialog.set_secondary_text("Reason was: " + why);
          dialog.run();
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
  LOGI("TextDocument saving as '%s'", m_filename.c_str());
  std::string title = filename.substr(filename.find_last_of("/") + 1);
  m_button.title(title);
  m_filename = filename;
  return TextDocument::save();
}

// *************************************************************************************************
//
// *************************************************************************************************
bool TextDocument::close()
{
  bool res = true;

  if (isModified())
    {
      res = save();
    }
  if (res)
    {
      m_button.close();
    }
  return res;
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
      m_button.title(title);
      m_button.set_tooltip_text(filename);
    }

  infile.open(filename, std::fstream::in);
  if (!infile)
    {
      std::string why = strerror(errno);
      LOGF("could not open the file '%s' reason was '%s'",
           filename.c_str(), why.c_str());
      Gtk::MessageDialog dialog((Gtk::Window&) (*m_textview.get_toplevel()),
                                "Could not load '" + filename + "'",
                                false, Gtk::MESSAGE_WARNING);
      dialog.set_secondary_text("Reason was: " + why);
      dialog.run();
      return false;
    }

  while (std::getline(infile, line))
    {
      line = line + '\n';
      m_buffer->begin_not_undoable_action();
      m_buffer->insert(m_buffer->end(), line);
      m_buffer->end_not_undoable_action();
    }
  infile.close();

  if (clear)
    {
      modified(false);
    }

  return true;
}

// *************************************************************************************************
//
// *************************************************************************************************
TextEditor::TextEditor()
  : m_findwindow(nullptr),
    m_replacewindow(nullptr),
    m_gotolinewindow(nullptr),
    m_nb_nonames(0)
{
  LOGI("Creating TextEditor");

#if 0
  // TODO A remplacer par populatePopoverMenu()

  // Menus '_Documents'
  {
    m_menuitem[simtadyn::TextMenu].set_label("Text _Editor");
    m_menuitem[simtadyn::TextMenu].set_use_underline(true);
    m_menuitem[simtadyn::TextMenu].set_submenu(m_menu[simtadyn::TextMenu]);

    //
    m_submenu[6].set_label("Find");
    m_image[6].set_from_icon_name("edit-find", Gtk::ICON_SIZE_MENU);
    m_submenu[6].set_image(m_image[6]);
    m_submenu[6].signal_activate().connect(sigc::mem_fun(*this, &TextEditor::find));
    m_menu[simtadyn::TextMenu].append(m_submenu[6]);

    //
    m_submenu[7].set_label("Replace");
    m_image[7].set_from_icon_name("edit-find-replace", Gtk::ICON_SIZE_MENU);
    m_submenu[7].set_image(m_image[7]);
    m_submenu[7].signal_activate().connect(sigc::mem_fun(*this, &TextEditor::replace));
    m_menu[simtadyn::TextMenu].append(m_submenu[7]);

    //
    m_submenu[8].set_label("Go to Line");
    m_image[8].set_from_icon_name("go-bottom", Gtk::ICON_SIZE_MENU);
    m_submenu[8].set_image(m_image[8]);
    m_submenu[8].signal_activate().connect(sigc::mem_fun(*this, &TextEditor::gotoLine));
    m_menu[simtadyn::TextMenu].append(m_submenu[8]);
  }
#endif

  set_scrollable();
  signal_switch_page().connect(sigc::mem_fun(*this, &TextEditor::onPageSwitched));

  // Default Syntax coloration is Forth
  m_language_manager = Gsv::LanguageManager::get_default();
  m_language = m_language_manager->get_language("forth");
  if (!m_language)
    {
      std::cerr << "[WARNING] TextEditor::TextEditor: No syntax highlighted found for Forth" << std::endl;
    }

  // Change the look. Inspiration from juCi++ project (https://github.com/cppit/jucipp)
  auto provider = Gtk::CssProvider::create();
#if GTK_VERSION_GE(3, 20)
  provider->load_from_data("tab {border-radius: 5px 5px 0 0; padding: 0 4px; margin: 0;}");
#else
  provider->load_from_data(".notebook {-GtkNotebook-tab-overlap: 0px;} tab {border-radius: 5px 5px 0 0; padding: 4px 4px;}");
#endif
  get_style_context()->add_provider(provider, GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
}

// *************************************************************************************************
// FIXME:: le seul endroit ou appeller les sauvegardes
// *************************************************************************************************
TextEditor::~TextEditor()
{
  LOGI("Destroying TextEditor");
  TextEditor::closeAll();
}

// *************************************************************************************************
//
// *************************************************************************************************
bool TextEditor::saveAll()
{
  bool all_saved = true;

  for (int k = 0; k < get_n_pages(); ++k)
    {
      set_current_page(k);
      TextDocument *doc = TextEditor::document();
      if ((nullptr != doc) && (doc->isModified()))
        {
          all_saved &= dialogSave(doc);
        }
    }

  return all_saved;
}

// *************************************************************************************************
//
// *************************************************************************************************
bool TextEditor::close()
{
  bool res = false;
  TextDocument *doc = TextEditor::document();

  if ((nullptr != doc) && (doc->isModified()))
    {
      res = dialogSave(doc, true);
    }
  return res;
}

// *************************************************************************************************
//
// *************************************************************************************************
bool TextEditor::closeAll()
{
  bool all_closed = true;

  for (int k = 0; k < get_n_pages(); ++k)
    {
      set_current_page(k);
      TextDocument *doc = TextEditor::document();
      if ((nullptr != doc) && (doc->isModified()))
        {
          all_closed &= dialogSave(doc, true);
        }
    }
  return all_closed;
}

// *************************************************************************************************
//
// *************************************************************************************************
TextDocument* TextEditor::document()
{
  int page = get_current_page();
  Gtk::Widget *widget = get_nth_page(page);
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
  Gtk::Widget *widget = get_nth_page(static_cast<int>(i));
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
bool TextEditor::dialogSave(TextDocument *doc, const bool closing)
{
  // FIXME: faire apparaitre avant de tuer la fenetre principale sinon le dialog peut etre cache par d'autres fentres
  Gtk::MessageDialog dialog((Gtk::Window&) (*get_toplevel()),
                            "The document '" + doc->m_button.title() +
                            "' has been modified. Do you want to save it now ?",
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
      if (closing)
        {
          doc->modified(false);
          return true;
        }
      return !doc->isModified();
    }
}

// *************************************************************************************************
//
// *************************************************************************************************
bool TextEditor::saveAs(TextDocument *doc)
{
  Gtk::FileChooserDialog dialog("Please choose a file to save as", Gtk::FILE_CHOOSER_ACTION_SAVE);
  dialog.set_transient_for((Gtk::Window&) (*get_toplevel()));

  // Set to the SimTaDyn path while no longer the GTK team strategy.
  dialog.set_current_folder(config::data_path);

  // Add response buttons the the dialog:
  dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
  dialog.add_button(Gtk::Stock::SAVE_AS, Gtk::RESPONSE_OK);

  // Add filters, so that only certain file types can be selected:
  auto filter_forth = Gtk::FileFilter::create();
  filter_forth->set_name("Forth files");
  filter_forth->add_pattern("*.fs");
  filter_forth->add_pattern("*.fth");
  filter_forth->add_pattern("*.4th");
  filter_forth->add_pattern("*.forth");
  dialog.add_filter(filter_forth);

  auto filter_text = Gtk::FileFilter::create();
  filter_text->set_name("Text files");
  filter_text->add_mime_type("text/plain");
  dialog.add_filter(filter_text);

  auto filter_any = Gtk::FileFilter::create();
  filter_any->set_name("Any files");
  filter_any->add_pattern("*");
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
bool TextEditor::open()
{
  Gtk::FileChooserDialog dialog("Please choose a file to open", Gtk::FILE_CHOOSER_ACTION_OPEN);
  dialog.set_transient_for((Gtk::Window&) (*get_toplevel()));

  // Set to the SimTaDyn path while no longer the GTK team strategy.
  dialog.set_current_folder(config::data_path);

  // Add response buttons the the dialog:
  dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
  dialog.add_button(Gtk::Stock::OPEN, Gtk::RESPONSE_OK);

  // Add filters, so that only certain file types can be selected:
  auto filter_text = Gtk::FileFilter::create();
  filter_text->set_name("Text files");
  filter_text->add_mime_type("text/plain");
  dialog.add_filter(filter_text);

  auto filter_forth = Gtk::FileFilter::create();
  filter_forth->set_name("Forth files");
  filter_forth->add_pattern("*.fs");
  filter_forth->add_pattern("*.fth");
  filter_forth->add_pattern("*.4th");
  filter_forth->add_pattern("*.forth");
  dialog.add_filter(filter_forth);

  auto filter_any = Gtk::FileFilter::create();
  filter_any->set_name("Any files");
  filter_any->add_pattern("*");
  dialog.add_filter(filter_any);

  int result = dialog.run();
  if (Gtk::RESPONSE_OK == result)
    {
      return TextEditor::open(dialog.get_filename());
    }
  return false;
}

// *************************************************************************************************
//
// *************************************************************************************************
bool TextEditor::open(std::string const& filename)
{
  int pages = get_n_pages();
  if (pages < 0) return false;

  // Already opened ? Switch the page
  for (int k = 0; k < pages; ++k)
    {
      if (0 == document(static_cast<uint32_t>(k))->m_filename.compare(filename))
        {
          //std::cout << "'" << filename << "' already opened\n"; // TODO statusbar
          set_current_page(k);
          return true;
        }
    }
  return load(filename);
}

// *************************************************************************************************
// When switching page on the notebook, reaffect windows find, replace to the switched document
// *************************************************************************************************
void TextEditor::onPageSwitched(Gtk::Widget* page, guint page_num)
{
  (void) page;
  m_findwindow.document(&(TextEditor::document(page_num)->m_textview));
  m_findwindow.title(TextEditor::document(page_num)->title());
  m_replacewindow.document(&(TextEditor::document(page_num)->m_textview));
  m_replacewindow.title(TextEditor::document(page_num)->title());
  m_gotolinewindow.document(&(TextEditor::document(page_num)->m_textview));
  m_gotolinewindow.title(TextEditor::document(page_num)->title());
}

// *************************************************************************************************
//
// *************************************************************************************************
void TextEditor::empty(std::string const& title)
{
  TextDocument *doc = create();

  ++m_nb_nonames;
  doc->m_button.title(title + ' ' + std::to_string(m_nb_nonames));
  doc->m_button.link(this, doc);

  append_page(*doc, doc->m_button);
  show_all();
  set_current_page(-1);
}

// *************************************************************************************************
//
// *************************************************************************************************
TextDocument *TextEditor::tab(std::string const& title)
{
  int32_t tmp = get_n_pages();
  if (tmp < 0) return nullptr;

  uint32_t pages = static_cast<uint32_t>(tmp);
  for (uint32_t k = 0; k < pages; ++k)
    {
      // TBD: compare title ou filename ou les deux ?
      if (0 == document(k)->title().compare(title))
        {
          // Found
          return document(k);
        }
    }

  // Not found
  return nullptr;
}

// *************************************************************************************************
//
// *************************************************************************************************
TextDocument *TextEditor::addTab(std::string const& title)
{
  TextDocument *doc = tab(title);
  if (nullptr == doc)
    {
      doc = addTab();
    }
  doc->title(title);
  //doc->filename(title);
  return doc;
}

// *************************************************************************************************
//
// *************************************************************************************************
TextDocument *TextEditor::addTab()
{
  TextDocument *doc = create();
  append_page(*doc, doc->m_button);
  show_all();
  set_current_page(-1);
  doc->m_button.link(this, doc);
  return doc;
}

// *************************************************************************************************
//
// *************************************************************************************************
bool TextEditor::load(std::string const& filename)
{
  TextDocument *doc = addTab(filename);
  //assert(nullptr != doc);
  // FIXME: mettre en gris le fond si le document est en read-only
  return doc->load(filename);
}

// *************************************************************************************************
//
// *************************************************************************************************
void TextEditor::save()
{
  TextDocument* doc = document();

  if (nullptr != doc)
    {
      if (0 == doc->m_filename.compare("")) // FIXME || read-only(file)
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
void TextEditor::saveAs()
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
      return doc->utext();
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

// *************************************************************************************************
// Launch the find and replace window
// *************************************************************************************************
void TextEditor::replace()
{
  m_replacewindow.show();
}

// *************************************************************************************************
// Launch the go to line window
// *************************************************************************************************
void TextEditor::gotoLine()
{
  m_gotolinewindow.show();
}
