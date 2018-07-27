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
// along with GNU Emacs.  If not, see <http://www.gnu.org/licenses/>.
//=====================================================================

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
