#include "SimForth.hpp"
#include "SimTaDynContext.hpp"

// **************************************************************
//
// **************************************************************
SimForth::SimForth()
{
  i = 0; // FIXME: prototype
}

// **************************************************************
//
// **************************************************************
void SimForth::addForthMenu(const Gtk::BuiltinStockID icon,
                            const std::string &word)
{
  //m_menuimage[5].set(Gtk::Stock::FIND, Gtk::ICON_SIZE_MENU);
  //menulist.push_back(Gtk::Menu_Helpers::ImageMenuElem("_Find",
  // m_menuimage[5], sigc::mem_fun(*this, &SimTaDynWindow::find)));
}

// **************************************************************
// A placer dans SimTaDynWindow
// **************************************************************
void SimForth::addForthButon(const Gtk::BuiltinStockID icon,
                             const std::string &word)
//const Cell16 ForthToken)

{
  SimTaDynContext& simtadyn = SimTaDynContext::getInstance();
  SimTaDynWindow* win = simtadyn.m_window;

  // FIXME: ajouter le postip avec la definiton du mot "WORD ( n1 n2 -- n3 n4 )"
  // FIXME: si pile vide ou pas le bon nombre d'elements alors fenetre popup qui demande les param
  win->m_toolbutton[2 + i].set_label(word); // FIXME: remplacer tableau par vector
  win->m_toolbutton[2 + i].set_stock_id(icon);
  win->m_toolbar[1].append(win->m_toolbutton[2 + i],
    sigc::bind<const std::string>(sigc::mem_fun(*this, &SimForth::interprete), word)); // FIXME: interprete pas bon: eatString oui // FIXME ne pas autoriser a compiler
    //sigc::bind<const Cell16>(sigc::mem_fun(*this, &SimForth::execToken),
    // ForthToken));
  win->m_toolbar[1].show_all_children();
}
