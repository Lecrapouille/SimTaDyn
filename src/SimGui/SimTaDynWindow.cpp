#include "SimTaDynWindow.hpp"
#include "MapEditor.hpp"
#include "ForthEditor.hpp"
#include "Config.hpp"

// *************************************************************************************************
// SimTaDyn main window
// *************************************************************************************************
SimTaDynWindow::SimTaDynWindow()
  : Gtk::Window()
{
  // Main window
  {
    set_title(Config::instance().m_title);
    set_default_size(1400, 800);
    set_position(Gtk::WIN_POS_CENTER);
    setTitleIcon();
  }

  // Drawing area
  {
    m_drawing_area.set_hexpand(true);
    m_drawing_area.set_vexpand(true);
    m_drawing_area.set_auto_render(true);
    //m_drawing_area.set_halign(Gtk::ALIGN_FILL);
    //m_drawing_area.set_valign(Gtk::ALIGN_FILL);

    // Connect drawing area signals
    m_drawing_area.signal_realize().connect(sigc::mem_fun(*this, &SimTaDynWindow::onRealize));
    m_drawing_area.signal_unrealize().connect(sigc::mem_fun(*this, &SimTaDynWindow::onUnrealize), false);
    m_drawing_area.signal_render().connect(sigc::mem_fun(*this, &SimTaDynWindow::onRender));

    add_events(Gdk::KEY_RELEASE_MASK);
    signal_key_press_event().connect_notify(sigc::mem_fun(*this, &SimTaDynWindow::onKeyPressed));
    signal_key_release_event().connect_notify(sigc::mem_fun(*this, &SimTaDynWindow::onKeyReleased));
  }

  // Menus:
  // * _Map: Import/export/save/load/... geographic maps.
  // * _Forth: Import/export/save/load/... Forth scripts
  // * _Text: search, replace, goto line
  // * _Plugins: TBD: Let the user to add an menu calling it's on fprth scripts.
  // * _Help: TBD: add About/help/interactive tutorials
  {
    // Menu '_Map'
    m_menubar.append(MapEditor::instance().m_menuitem[simtadyn::MapMenu]);

    // Menu '_Forth'
    m_menubar.append(ForthEditor::instance().m_menuitem[simtadyn::ForthMenu]);
    m_menubar.append(ForthEditor::instance().m_menuitem[simtadyn::TextMenu]);
    m_menubar.append(ForthEditor::instance().m_menuitem[simtadyn::PlugginsMenu]);
    ForthEditor::instance().addPluggin("text-x-generic-template", "41 1 + . CR", "test");


    // Menu '_Help'
    // TODO: submenus Tuto, Help, Example, About
    m_menuitem[simtadyn::HelpMenu].set_label("_Help");
    m_menuitem[simtadyn::HelpMenu].set_use_underline(true);
    m_menubar.append(m_menuitem[simtadyn::HelpMenu]);
    m_menuitem[simtadyn::HelpMenu].set_submenu(m_menu[simtadyn::HelpMenu]);
  }

  // Split verticaly the main window
  {
    add(m_hpaned[0]);
    m_hpaned[0].set_position(800);
    m_hpaned[0].pack1(m_drawing_area);
    m_hpaned[0].pack2(m_hbox[0]); // Forth editor
  }

  // Rigth side of the main window: Forth editor
  // From top to bottom: menubar, notebook, toolbar, statusbat, notebook
  {
    //
    m_vbox[0].pack_start(m_menubar, Gtk::PACK_SHRINK);
    m_vbox[0].pack_start(ForthEditor::instance().m_notebook, Gtk::PACK_EXPAND_WIDGET);

    //
    m_vbox[1].pack_start(ForthEditor::instance().m_toolbar, Gtk::PACK_SHRINK);
    m_vbox[1].pack_start(ForthEditor::instance().m_statusbar, Gtk::PACK_SHRINK);
    m_vbox[1].pack_start(ForthEditor::instance().m_hpaned, Gtk::PACK_EXPAND_WIDGET);

    //
    m_hbox[0].pack_start(MapEditor::instance().m_toolbar, Gtk::PACK_SHRINK);
    m_hbox[0].pack_start(m_vpaned[0]);

    //
    m_vpaned[0].pack1(m_vbox[0]);
    m_vpaned[0].pack2(m_vbox[1]);
    m_vpaned[0].set_position(350);
  }

  // When terminating the SimTaDyn application
  //Gtk::Main::signal_quit().connect(sigc::mem_fun(this, &SimTaDynWindow::onQuit));
  show_all_children();
}

void SimTaDynWindow::setTitleIcon()
{
  std::string path = Config::instance().data_path("icons/SimTaDyn.png");

  if (File::exist(path))
    {
      set_icon_from_file(path);
    }
  else
    {
      LOGW("SimTaDynWindow: Icon '%s' does not exist\n", path.c_str());
    }
}

void SimTaDynWindow::onKeyPressed(GdkEventKey* evenement)
{
  switch (evenement->keyval)
    {
    case GDK_KEY_Escape:
      break;
    case GDK_KEY_F1:
      {
        //SimTaDynContext& simtadyn = SimTaDynContext::getInstance();
        //m_drawing_area.zoomFitPage(simtadyn.m_graph);
      }
      break;
    case GDK_KEY_Page_Up:
      m_drawing_area.keyPressed(GLDrawingArea::Forward);
      break;
    case GDK_KEY_Page_Down:
      m_drawing_area.keyPressed(GLDrawingArea::Backward);
      break;
    case GDK_KEY_Up:
      m_drawing_area.keyPressed(GLDrawingArea::Up);
      break;
    case GDK_KEY_Down:
      m_drawing_area.keyPressed(GLDrawingArea::Down);
      break;
    case GDK_KEY_Right:
      m_drawing_area.keyPressed(GLDrawingArea::Right);
      break;
    case GDK_KEY_Left:
      m_drawing_area.keyPressed(GLDrawingArea::Left);
      break;
    default:
      break;
    }
  ForthEditor::instance().autoCompleteWord(evenement->keyval);
}

void SimTaDynWindow::onKeyReleased(GdkEventKey* evenement)
{
  switch (evenement->keyval)
    {
    case GDK_KEY_Page_Up:
      m_drawing_area.keyReleased(GLDrawingArea::Forward);
      break;
    case GDK_KEY_Page_Down:
      m_drawing_area.keyReleased(GLDrawingArea::Backward);
      break;
    case GDK_KEY_Up:
      m_drawing_area.keyReleased(GLDrawingArea::Up);
      break;
    case GDK_KEY_Down:
      m_drawing_area.keyReleased(GLDrawingArea::Down);
      break;
    case GDK_KEY_Right:
      m_drawing_area.keyReleased(GLDrawingArea::Right);
      break;
    case GDK_KEY_Left:
      m_drawing_area.keyReleased(GLDrawingArea::Left);
      break;
    default:
      break;
    }
}
