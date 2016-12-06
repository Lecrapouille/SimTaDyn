#include "SimTaDynWindow.hpp"
#include "SimTaDynContext.hpp"

// *************************************************************************************************
// SimTaDyn main window
// *************************************************************************************************
SimTaDynWindow::SimTaDynWindow(const std::string& title)
  : Gtk::Window(),
    m_nb_forth_buttons(0),
    m_nb_map_buttons(0)
{
  // Main window
  set_title(title);
  set_default_size(1024, 800);
  set_position(Gtk::WIN_POS_CENTER);

  // Drawing area
  m_drawing_area.set_hexpand(true);
  m_drawing_area.set_vexpand(true);
  m_drawing_area.set_auto_render(true);
  //m_drawing_area.set_halign(Gtk::ALIGN_FILL);
  //m_drawing_area.set_valign(Gtk::ALIGN_FILL);

  // Desired OpenGL context version 3.3
  m_drawing_area.set_required_version(3, 3);

  // Connect gl area signals
  m_drawing_area.signal_realize().connect(sigc::mem_fun(*this, &SimTaDynWindow::onRealize));
  m_drawing_area.signal_unrealize().connect(sigc::mem_fun(*this, &SimTaDynWindow::onUnrealize), false);
  m_drawing_area.signal_render().connect(sigc::mem_fun(*this, &SimTaDynWindow::onRender));

  // Menus:
  // * _Map: Import/export/save/load/... geographic maps.
  // * _Forth: Import/export/save/load/... Forth scripts
  // * _Plugins: TBD: Let the user to add an menu calling it's on fprth scripts.
  // * _Help: TBD: add About/help/interactive tutorials
  {
    m_menu[0].set_label("Map"); m_menubar.append(m_menu[0]);
    //m_fortheditor.m_menu.set_label("Forth"); m_menubar.append(m_fortheditor.m_menu]);
    m_menu[2].set_label("Plugins"); m_menubar.append(m_menu[2]);
    m_menu[3].set_label("Help"); m_menubar.append(m_menu[3]);
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

    m_renderer.start();
    m_renderer.clearScreen();
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
    m_renderer.end();
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
    m_renderer.render();

    return true;
  }
  catch (const Gdk::GLError& gle)
  {
    std::cerr << "An error occurred in the render callback of the GLArea" << std::endl;
    std::cerr << gle.domain() << "-" << gle.code() << "-" << gle.what() << std::endl;
    return false;
  }
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
  (void) icon;
  (void) word;
  //m_menuimage[5].set(Gtk::Stock::FIND, Gtk::ICON_SIZE_MENU);
  //menulist.push_back(Gtk::Menu_Helpers::ImageMenuElem("_Find",
  // m_menuimage[5], sigc::mem_fun(*this, &SimTaDynWindow::find)));
}
