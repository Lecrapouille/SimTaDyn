#include "SimTaDynWindow.hpp"
#include "SimTaDynContext.hpp"

// *************************************************************************************************
// SimTaDyn main window
// *************************************************************************************************
SimTaDynWindow::SimTaDynWindow(const std::string& title)
  : Gtk::Window(),
    m_nb_opened_documents(0)
{
  std::cout << "SimTaDynWindow::SimTaDynWindow\n";
  set_title(title);
  set_default_size(1000, 600);
  set_position(Gtk::WIN_POS_CENTER);

  // Text editors
  uint32_t i = m_nb_opened_documents;
  m_nb_opened_documents++;
  m_scrolledwindow[i].add(m_textview[i]);
  m_scrolledwindow[i].set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
  m_textview[i].get_buffer()->set_text(": DOUBLE DUP + ;\n"
                                       ": NUM 42 ;\n"
                                       ": QUADRUPLE DOUBLE DOUBLE DUP . NUM . ;\n"
                                       "1 2 QUADRUPLE + ." );

  // Text editors in notebook
  m_notebook[0].set_scrollable();
  m_notebook[0].append_page(m_scrolledwindow[i], "Forth Editor");

  // Menus
  m_menubar.items().push_back(Gtk::Menu_Helpers::MenuElem("_Map", m_menu[0]));
  m_menubar.items().push_back(Gtk::Menu_Helpers::MenuElem("_Forth", m_menu[1]));
  m_menubar.items().push_back(Gtk::Menu_Helpers::MenuElem("_Help", m_menu[2]));

  // Submenus 'Forth'
  Gtk::Menu::MenuList& menulist = m_menu[1].items();
  m_menuimage[3].set(Gtk::Stock::NEW, Gtk::ICON_SIZE_MENU);
  menulist.push_back(Gtk::Menu_Helpers::ImageMenuElem("_New",
     m_menuimage[3], sigc::mem_fun(*this, &SimTaDynWindow::fileNew)));
  m_menuimage[0].set(Gtk::Stock::OPEN, Gtk::ICON_SIZE_MENU);
  menulist.push_back(Gtk::Menu_Helpers::ImageMenuElem("_Open",
     m_menuimage[0], sigc::mem_fun(*this, &SimTaDynWindow::fileOpen)));
  m_menuimage[1].set(Gtk::Stock::SAVE_AS, Gtk::ICON_SIZE_MENU);
  menulist.push_back(Gtk::Menu_Helpers::ImageMenuElem("_SaveAs",
     m_menuimage[1], sigc::mem_fun(*this, &SimTaDynWindow::fileSaveAs)));
  m_menuimage[2].set(Gtk::Stock::SAVE, Gtk::ICON_SIZE_MENU);
  menulist.push_back(Gtk::Menu_Helpers::ImageMenuElem("_Save",
     m_menuimage[2], sigc::mem_fun(*this, &SimTaDynWindow::fileSave)));

  m_vbox[0].pack_start(m_menubar, Gtk::PACK_SHRINK);
  m_vbox[0].pack_start(m_notebook[0]);

  // Horizontal split
  add(m_hpaned[0]);
  m_hpaned[0].set_position(600);
  m_hpaned[0].pack1(m_drawing_area);
  m_hpaned[0].pack2(m_vbox[0]);

  add_events(Gdk::KEY_RELEASE_MASK);
  signal_key_press_event().connect_notify(sigc::mem_fun(*this, &SimTaDynWindow::onKeyPressed));
  signal_key_release_event().connect_notify(sigc::mem_fun(*this, &SimTaDynWindow::onKeyReleased));
  //signal_scroll_event().connect(sigc::mem_fun(*this, &SimTaDynWindow::on_scroll_event));

  show_all_children();
}

// *************************************************************************************************
//
// *************************************************************************************************
void SimTaDynWindow::fileNew()
{
  uint32_t i = m_nb_opened_documents;

  if (16 == m_nb_opened_documents)
    {
      std::cout << "Too many notebook pages opened\n"; // TODO statusbar
      return ;
    }

  m_scrolledwindow[i].add(m_textview[i]);
  m_scrolledwindow[i].set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
  m_notebook[0].append_page(m_scrolledwindow[i], "New forth script");
  m_notebook[0].show_all();
  m_notebook[0].set_current_page(i);
  m_nb_opened_documents++;
}

// *************************************************************************************************
//
// *************************************************************************************************
void SimTaDynWindow::fileSave()
{
  std::pair<Gtk::TextView*, std::string> tv = getCurrentTextView();

  if (NULL != tv.first)
    {
      std::ofstream outfile;

      outfile.open(tv.second, std::fstream::out);
      if (outfile)
        {
          outfile << tv.first->get_buffer()->get_text();
        }
      outfile.close();
    }
}

// *************************************************************************************************
//
// *************************************************************************************************
void SimTaDynWindow::fileSaveAs()
{
  Gtk::FileChooserDialog dialog("Please choose a file to save as", Gtk::FILE_CHOOSER_ACTION_OPEN);
  dialog.set_transient_for(*this);

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

  // Handle the response:
  switch (result)
    {
    case Gtk::RESPONSE_OK:
      {
        // First change the notebook label with the new filename
        Glib::ustring filename(dialog.get_filename());
        m_notebook[0].set_tab_label_text(*(m_notebook[0].get_nth_page(m_notebook[0].get_current_page())),
                                         filename);
        fileSave();
      }
      break;

    case Gtk::RESPONSE_CANCEL:
      break;

    default:
      break;
    }
}

// *************************************************************************************************
//
// *************************************************************************************************
void SimTaDynWindow::fileOpen()
{
  if (16 == m_nb_opened_documents)
    {
      std::cout << "Too many notebook pages opened\n"; // TODO statusbar
      return ;
    }

  std::ifstream infile;
  std::string line, path, base_name;

  Gtk::FileChooserDialog dialog("Please choose a file to open", Gtk::FILE_CHOOSER_ACTION_OPEN);
  dialog.set_transient_for(*this);

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

  // Handle the response:
  switch (result)
    {
    case Gtk::RESPONSE_OK:
      {
        uint32_t i = m_nb_opened_documents;

        // Get the base name
        path = dialog.get_filename();
        base_name = path;//path.substr(path.find_last_of("/") + 1);

        // Already opened ? Switch the page
        for (uint32_t k = 0; k < m_nb_opened_documents; ++k)
          {
            if (0 == base_name.compare(m_notebook[0].get_tab_label_text(*(m_notebook[0].get_nth_page(k)))))
              {
                std::cout << "Already opened\n"; // TODO statusbar
                m_notebook[0].set_current_page(1);
                return ;
              }
          }

        // Open a new notebook page/text editor
        m_scrolledwindow[i].add(m_textview[i]);
        m_scrolledwindow[i].set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
        m_notebook[0].append_page(m_scrolledwindow[i], base_name);
        m_notebook[0].show_all();
        m_notebook[0].set_current_page(i);
        m_nb_opened_documents++;

        // Read the file and store its content into the new notebook page/text editor
        infile.open(dialog.get_filename(), std::fstream::in);
        if (infile)
          {
            while (std::getline(infile, line))
              {
                line = line + '\n';
                m_textview[i].get_buffer()->insert(m_textview[i].get_buffer()->end(), line);
              }
            infile.close();
          }
      }
      break;

    case Gtk::RESPONSE_CANCEL:
      break;

    default:
      break;
    }
}

// *************************************************************************************************
// Get the Forth opened document (text view) from the notebook
// *************************************************************************************************
std::pair<Gtk::TextView*, std::string> SimTaDynWindow::getCurrentTextView()
{
  int page = m_notebook[0].get_current_page();
  Gtk::Widget *widget = m_notebook[0].get_nth_page(page);
  if (NULL == widget)
    {
      return std::make_pair(nullptr, std::string());
    }
  Gtk::ScrolledWindow *sw = dynamic_cast<Gtk::ScrolledWindow*>(widget);
  Gtk::TextView *tv = dynamic_cast<Gtk::TextView*>(sw->get_child());
  return std::make_pair(tv, m_notebook[0].get_tab_label_text(*widget));
}

#if 0
// Mouse button pressed : process mouse button event
bool SimTaDynWindow::on_button_press_event(GdkEventButton *event)
{
  // Check if the event is a left button click.
  if (1 == event->button)
    {
      // Memorize pointer position
      lastXMouse = event->x;
      lastYMouse = event->y;
      // Start moving the view
      moveFlag = true;
      // Event has been handled
      return true;
    }

#if 0
  // Check if the event is a right button click.
  if (3 == event->button)
    {
      // Memorize mouse coordinates
      lastXMouse = event->x;
      lastYMouse = event->y;
      // Display the popup menu
      m_Menu_Popup.popup(event->button, event->time);
      // The event has been handled.
      return true;
    }
#endif

  // Event has not been handled
  return false;
}

// Mouse button released : process mouse button event
bool GlDrawingArea::on_button_release_event(GdkEventButton *event)
{
  // Check if it is the left button
  if ((1 == event->button) && moveFlag)
    {
      // End of motion
      moveFlag = false;
      // Update image focus
      imgFocusX -= (event->x - lastXMouse) / scale;
      imgFocusY -= (event->y - lastYMouse) / scale;
      return true;
    }
  // Event has been handled
  return false;
}

// Mouse pointer moved : process event
bool GlDrawingArea::on_motion_notify_event(GdkEventMotion*event)
{
  // If the left button is pressed, move the view
  if (moveFlag)
    {
      // Get mouse coordinates
      int XMouse = event->x;
      int YMouse = event->y;

      // Update image focus
      imgFocusX -= (XMouse - lastXMouse) / scale;
      imgFocusY -= (YMouse - lastYMouse) / scale;

      // Memorize new position of the pointer
      lastXMouse = XMouse;
      lastYMouse = YMouse;

      // Update view
      camera.moveAt(imgFocusX, imgFocusY);
      applyViewport();
      return true;
    }
  // Event has been handled
  return false;
}
#endif

void SimTaDynWindow::onKeyPressed(GdkEventKey* evenement)
{
  switch (evenement->keyval)
    {
    case GDK_KEY_Escape:
      break;
    case GDK_KEY_F1:
      {
        SimTaDynContext& simtadyn = SimTaDynContext::getInstance();
        m_drawing_area.zoomFitPage(simtadyn.graph);
      }
      break;
    case GDK_KEY_Page_Up:
      m_drawing_area.keyPressed(GlDrawingArea::Forward);
      break;
    case GDK_KEY_Page_Down:
      m_drawing_area.keyPressed(GlDrawingArea::Backward);
      break;
    case GDK_KEY_Up:
      m_drawing_area.keyPressed(GlDrawingArea::Up);
      break;
    case GDK_KEY_Down:
      m_drawing_area.keyPressed(GlDrawingArea::Down);
      break;
    case GDK_KEY_Right:
      m_drawing_area.keyPressed(GlDrawingArea::Right);
      break;
    case GDK_KEY_Left:
      m_drawing_area.keyPressed(GlDrawingArea::Left);
      break;
    default:
      break;
    }
}

void SimTaDynWindow::onKeyReleased(GdkEventKey* evenement)
{
  switch (evenement->keyval)
    {
    case GDK_KEY_Page_Up:
      m_drawing_area.keyReleased(GlDrawingArea::Forward);
      break;
    case GDK_KEY_Page_Down:
      m_drawing_area.keyReleased(GlDrawingArea::Backward);
      break;
    case GDK_KEY_Up:
      m_drawing_area.keyReleased(GlDrawingArea::Up);
      break;
    case GDK_KEY_Down:
      m_drawing_area.keyReleased(GlDrawingArea::Down);
      break;
    case GDK_KEY_Right:
      m_drawing_area.keyReleased(GlDrawingArea::Right);
      break;
    case GDK_KEY_Left:
      m_drawing_area.keyReleased(GlDrawingArea::Left);
      break;
    default:
      break;
    }
}
