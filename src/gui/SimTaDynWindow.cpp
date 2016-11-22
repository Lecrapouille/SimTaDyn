#include "SimTaDynWindow.hpp"
#include "SimTaDynContext.hpp"

// *************************************************************************************************
// When terminating the SimTaDyn application
// *************************************************************************************************
bool SimTaDynWindow::quitting()
{
  std::cout << "SimTaDynWindow said: 'bye bye'" << std::endl;
  return false;
}

// *************************************************************************************************
// SimTaDyn main window
// *************************************************************************************************
SimTaDynWindow::SimTaDynWindow(const std::string& title)
  : Gtk::Window(),
    m_nb_forth_buttons(0),
    m_nb_map_buttons(0)
{
  set_title(title);
  set_default_size(1024, 800);
  set_position(Gtk::WIN_POS_CENTER);

  // Menus:
  // * _Map: Import/export/save/load/... geographic maps.
  // * _Forth: Import/export/save/load/... Forth scripts
  // * _Plugins: TBD: Let the user to add an menu calling it's on fprth scripts.
  // * _Help: TBD: add About/help/interactive tutorials
  {
    m_menubar.items().push_back(Gtk::Menu_Helpers::MenuElem("_Map", m_menu[0]));
    m_menubar.items().push_back(Gtk::Menu_Helpers::MenuElem("_Forth", m_fortheditor.m_menu));
    m_menubar.items().push_back(Gtk::Menu_Helpers::MenuElem("_Plugins", m_menu[2]));
    m_menubar.items().push_back(Gtk::Menu_Helpers::MenuElem("_Help", m_menu[3]));
  }

  // Submenus '_Map'
  {
  }

  // Map toolbar:
  {
    //m_toolbar[MapToolbar].append(m_toolbutton[0]);
    //m_toolbar[MapToolbar].append(m_separator[0]);
  }

  // Forth toolbar
  {
    m_toolbutton[0].set_label("New");
    m_toolbutton[0].set_stock_id(Gtk::Stock::NEW);
    m_toolbar[ForthToolbar].append(m_toolbutton[0], sigc::mem_fun(m_fortheditor, &ForthEditor::newDocument));

    m_toolbutton[1].set_label("Exec");
    m_toolbutton[1].set_stock_id(Gtk::Stock::EXECUTE);
    m_toolbar[ForthToolbar].append(m_toolbutton[1], sigc::mem_fun(m_fortheditor, &ForthEditor::exec));
    m_toolbar[ForthToolbar].append(m_separator[1]);
    m_nb_forth_buttons += 2U;
  }

  // Split horizontaly the main window
  {
    add(m_hpaned[0]);
    m_hpaned[0].set_position(800);
    m_hpaned[0].pack1(m_drawing_area);
    m_hpaned[0].pack2(m_hbox[0]);
  }

  // Left side of the main window: OpenGL area
  {
    // FIXME ajouter l'editeur de proprietes des cellules sous la map
    add_events(Gdk::KEY_RELEASE_MASK);
    signal_key_press_event().connect_notify(sigc::mem_fun(*this, &SimTaDynWindow::onKeyPressed));
    signal_key_release_event().connect_notify(sigc::mem_fun(*this, &SimTaDynWindow::onKeyReleased));
    //signal_scroll_event().connect(sigc::mem_fun(*this, &SimTaDynWindow::on_scroll_event));
  }

  // Rigth side of the main window: Forth editor
  {
    //
    m_vbox[0].pack_start(m_menubar, Gtk::PACK_SHRINK);
    m_vbox[0].pack_start(m_fortheditor.m_notebook, Gtk::PACK_EXPAND_WIDGET);

    //
    m_vbox[1].pack_start(m_toolbar[ForthToolbar], Gtk::PACK_SHRINK);
    m_vbox[1].pack_start(m_fortheditor.m_statusbar, Gtk::PACK_SHRINK);
    m_vbox[1].pack_start(m_fortheditor.m_res_notebooks, Gtk::PACK_EXPAND_WIDGET);

    m_hbox[0].pack_start(m_toolbar[MapToolbar], Gtk::PACK_SHRINK);
    m_hbox[0].pack_start(m_vpaned[0]);
    m_vpaned[0].pack1(m_vbox[0]);
    m_vpaned[0].pack2(m_vbox[1]);
    m_vpaned[0].set_position(350);
  }

  // When terminating the SimTaDyn application
  Gtk::Main::signal_quit().connect(sigc::mem_fun(this, &SimTaDynWindow::quitting));

  show_all_children();
}

// **************************************************************
//
// **************************************************************
void SimTaDynWindow::addForthButon(const Gtk::BuiltinStockID icon,
                                   const std::string &script) //const Cell16 ForthToken)

{
  if (m_nb_forth_buttons < 32)
    {
      // FIXME: si pile vide ou pas le bon nombre d'elements alors fenetre popup qui demande les param
      m_toolbutton[m_nb_forth_buttons].set_label(script);
      m_toolbutton[m_nb_forth_buttons].set_stock_id(icon);
      // FIXME: ajouter le postip avec la definiton du mot "WORD ( n1 n2 -- n3 n4 )"
      m_toolbutton[m_nb_forth_buttons].set_tooltip_text("COUCOU");
      // FIXME ne pas autoriser a compiler
      m_toolbar[ForthToolbar].append(m_toolbutton[m_nb_forth_buttons],
                                     sigc::bind<const std::string>(sigc::mem_fun(m_fortheditor, &ForthEditor::exec1), script));
      m_toolbar[ForthToolbar].show_all_children();
      ++m_nb_forth_buttons;
    }
  else
    {
      Gtk::MessageDialog dialog(*this, "FIXME: Max buttons reached");
      dialog.run();
    }
}

// **************************************************************
//
// **************************************************************
void SimTaDynWindow::addForthMenu(const Gtk::BuiltinStockID icon,
                                  const std::string &word)
{
  //m_menuimage[5].set(Gtk::Stock::FIND, Gtk::ICON_SIZE_MENU);
  //menulist.push_back(Gtk::Menu_Helpers::ImageMenuElem("_Find",
  // m_menuimage[5], sigc::mem_fun(*this, &SimTaDynWindow::find)));
}

#if 0 // FIXME
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
        m_drawing_area.zoomFitPage(simtadyn.m_graph);
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
