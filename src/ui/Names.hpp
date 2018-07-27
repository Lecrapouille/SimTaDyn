#ifndef NAMESPACES_HPP
#define NAMESPACES_HPP

namespace simtadyn
{
  //! \brief Toolbar for the map editor and a toolbar for the forth editor.
  //enum ToolBarNames { MapToolbar, ForthToolbar, MaxToolBarNames = ForthToolbar };
  //! \brief Map editor menus.
  enum MapMenuNames { MapMenu, MaxMapMenuNames = MapMenu };
  //! \brief Forth editor menus: Forth, text edition and Forth pluggins.
  enum ForthMenuNames { ForthMenu, TextMenu, PlugginsMenu, MaxForthMenuNames = PlugginsMenu };
  //! \brief Help menu.
  enum GeneralMenuNames { HelpMenu, MaxGeneralMenuNames = HelpMenu };
  //! \brief Notebook tab names for the Forth editor (text editor):
  //! results of Forth script, history of old forth commands, Forth dictionary, Simtadyn errors.
  enum ForthTabNames { ForthResTab, ForthHistoryTab, ForthDicoTab, ForthMsgTab, MaxForthTabNames = ForthMsgTab };
}

#endif
