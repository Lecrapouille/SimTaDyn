#include "SimTaDynWindow.hpp"
#include "SimTaDynContext.hpp"

// *************************************************************************************************
// SimTaDyn main window
// *************************************************************************************************
SimTaDynWindow::SimTaDynWindow(const std::string& title)
  : Gtk::Window(),
    m_nb_plugins(0)
{
  // Main window
  {
    set_title(title);
    set_default_size(1400, 800);
    set_position(Gtk::WIN_POS_CENTER);
  }

  // Drawing area
  {
    // Desired OpenGL context version 3.3
    m_drawing_area.set_required_version(3, 3);

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
  // * _Plugins: TBD: Let the user to add an menu calling it's on fprth scripts.
  // * _Help: TBD: add About/help/interactive tutorials
  {
    // Menu '_Map'
    m_menubar.append(m_mapeditor.m_menuitem[0]);

    // Menu '_Forth'
    m_menubar.append(m_fortheditor.m_menuitem[1]);
    m_menubar.append(m_fortheditor.m_menuitem[0]);

    // Menu '_Plugins'
    m_menuitem[PlugginsMenu].set_label("_Plugins");
    m_menuitem[PlugginsMenu].set_use_underline(true);
    m_menubar.append(m_menuitem[PlugginsMenu]);
    m_menuitem[PlugginsMenu].set_submenu(m_menu[PlugginsMenu]);
    addPluggin("text-x-generic-template", "41 1 + . CR", "test");

    // Menu '_Help'
    // TODO: submenus Tuto, Help, Example, About
    m_menuitem[HelpMenu].set_label("_Help");
    m_menuitem[HelpMenu].set_use_underline(true);
    m_menubar.append(m_menuitem[HelpMenu]);
    m_menuitem[HelpMenu].set_submenu(m_menu[HelpMenu]);
  }

  // Map toolbar (vertical)
  {
    m_toolbar[MapToolbar].set_property("orientation", Gtk::ORIENTATION_VERTICAL);
    m_toolbar[MapToolbar].set_property("toolbar-style", Gtk::TOOLBAR_ICONS);

    {
      Gtk::ToolButton *button = Gtk::manage(new Gtk::ToolButton());
      button->set_label("New");
      button->set_stock_id(Gtk::Stock::NEW);
      button->set_tooltip_text("New Forth document");
      m_toolbar[MapToolbar].append(*button, sigc::mem_fun(m_fortheditor, &ForthEditor::empty));
    }

    {
      Gtk::ToolButton *button = Gtk::manage(new Gtk::ToolButton());
      button->set_label("Open");
      button->set_stock_id(Gtk::Stock::NEW);
      button->set_tooltip_text("Open Forth document");
      m_toolbar[MapToolbar].append(*button, sigc::mem_fun(m_fortheditor, &ForthEditor::open));
    }
  }

  // Forth toolbar (horizontal)
  {
    {
      Gtk::ToolButton *button = Gtk::manage(new Gtk::ToolButton());
      button->set_label("Exec");
      button->set_stock_id(Gtk::Stock::EXECUTE);
      button->set_tooltip_text("Run Forth script");
      m_toolbar[ForthToolbar].append(*button, sigc::mem_fun(m_fortheditor, &ForthEditor::exec));
    }
    m_toolbar[ForthToolbar].append(m_separator[1]);
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
    m_vbox[0].pack_start(m_fortheditor.m_notebook, Gtk::PACK_EXPAND_WIDGET);

    //
    m_vbox[1].pack_start(m_toolbar[ForthToolbar], Gtk::PACK_SHRINK);
    m_vbox[1].pack_start(m_fortheditor.m_statusbar, Gtk::PACK_SHRINK);
    m_vbox[1].pack_start(m_fortheditor.m_hpaned, Gtk::PACK_EXPAND_WIDGET);

    //
    m_hbox[0].pack_start(m_toolbar[MapToolbar], Gtk::PACK_SHRINK);
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

// *************************************************************************************************
// Initialize the OpenGL context
// *************************************************************************************************
void SimTaDynWindow::onRealize()
{
  m_drawing_area.make_current();
  try
    {
      // GLArea only support Core profile.
      glewExperimental = true;
      if (GLEW_OK != glewInit())
        {
          std::cerr << "[FAILED] when initializing GLEW (Maybe the OpenGL context does not exist)." << std::endl;
        }

      m_drawing_area.throw_if_error();

      m_drawing_area.start();
      m_drawing_area.clearScreen();
    }
  catch (const Gdk::GLError& gle)
    {
      std::cerr << "[FAILED] An error occured making the context current during realize:" << std::endl;
      std::cerr << gle.domain() << "-" << gle.code() << "-" << gle.what() << std::endl;
    }
}

// *************************************************************************************************
// Clean up
// *************************************************************************************************
void SimTaDynWindow::onUnrealize()
{
  m_drawing_area.make_current();
  try
    {
      m_drawing_area.throw_if_error();
      m_drawing_area.end();
    }
  catch (const Gdk::GLError& gle)
    {
      std::cerr << "[FAILED] An error occured making the context current during unrealize" << std::endl;
      std::cerr << gle.domain() << "-" << gle.code() << "-" << gle.what() << std::endl;
    }
}

// *************************************************************************************************
// Draw the scene
// *************************************************************************************************
bool SimTaDynWindow::onRender(const Glib::RefPtr<Gdk::GLContext>& /* context */)
{
  try
    {
      m_drawing_area.throw_if_error();
      m_drawing_area.render();

      return true;
    }
  catch (const Gdk::GLError& gle)
    {
      std::cerr << "An error occurred in the render callback of the GLArea" << std::endl;
      std::cerr << gle.domain() << "-" << gle.code() << "-" << gle.what() << std::endl;
      return false;
    }
}

// FIXME const Cell16 ForthToken)
// **************************************************************
// FIXME: si pile vide ou pas le bon nombre d'elements alors fenetre popup qui demande les param
// FIXME: ajouter le postip avec la definiton du mot "WORD ( n1 n2 -- n3 n4 )"
// FIXME ne pas autoriser a compiler
// **************************************************************
Gtk::ToolButton *SimTaDynWindow::addForthButon(enum ToolBarNames toolbar,
                                               const Gtk::BuiltinStockID icon,
                                               const std::string &script,
                                               const std::string &help)

{
  Gtk::ToolButton *button = Gtk::manage(new Gtk::ToolButton());

  if (nullptr != button)
    {
      button->set_label(script);
      button->set_stock_id(icon);
      button->set_tooltip_text(help);
      // FIXME: exec1 est trop violent: ca efface le script en cours d'edition
      m_toolbar[toolbar].append(*button,
                                sigc::bind<const std::string>(sigc::mem_fun(m_fortheditor, &ForthEditor::exec1), script));
      m_toolbar[toolbar].show_all_children();
    }
  else
    {
      Gtk::MessageDialog dialog(*this, "Failed creating a Forth button");
      dialog.run();
    }
  return button;
}

// **************************************************************
// Interface
// **************************************************************
Gtk::ToolButton *SimTaDynWindow::addForthScriptButon(const Gtk::BuiltinStockID icon,
                                                     const std::string &script,
                                                     const std::string &help)
{
  return addForthButon(ForthToolbar, icon, script, help);
}

// **************************************************************
// Interface
// **************************************************************
Gtk::ToolButton *SimTaDynWindow::addMapScriptButon(const Gtk::BuiltinStockID icon,
                                                   const std::string &script,
                                                   const std::string &help)
{
  return addForthButon(MapToolbar, icon, script, help);
}

// **************************************************************
//
// **************************************************************
uint32_t SimTaDynWindow::addPluggin(const Glib::ustring& icon_name,
                                    const std::string &script,
                                    const std::string &help)
{
  m_plugins_submenu[m_nb_plugins].set_label(help);
  m_plugins_image[m_nb_plugins].set_from_icon_name(icon_name, Gtk::ICON_SIZE_MENU);
  m_plugins_submenu[m_nb_plugins].set_image(m_plugins_image[m_nb_plugins]);
  m_plugins_submenu[m_nb_plugins].signal_activate().connect(sigc::bind<const std::string>(sigc::mem_fun(m_fortheditor, &ForthEditor::exec1), script));
  m_menu[PlugginsMenu].append(m_plugins_submenu[m_nb_plugins]);
  ++m_nb_plugins;
  return m_nb_plugins - 1U;
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
  m_fortheditor.autoCompleteWord(evenement->keyval);
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
