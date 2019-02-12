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

#ifndef MAP_EDITOR_COMMANDS_HPP
#  define MAP_EDITOR_COMMANDS_HPP

#  include "MapEditorWindow.hpp"

class MapEditorWindow;

// *************************************************************************************************
//! \brief
// *************************************************************************************************
class IMapEditorCommand
{
public:
  virtual ~IMapEditorCommand() {}
  virtual void execute(MapEditorWindow& editor) = 0;
};

// *************************************************************************************************
//! \brief
// *************************************************************************************************
class MoveCameraCommand: public IMapEditorCommand
{
public:

  MoveCameraCommand(CameraDirection const direction)
    : m_direction(direction)
  {}

  virtual void execute(MapEditorWindow& editor) override;

private:

  CameraDirection m_direction;
};

// *************************************************************************************************
//! \brief
// *************************************************************************************************
class SplitScreenCommand: public IMapEditorCommand
{
public:

  SplitScreenCommand(Gtk::Orientation const orientation)
    : m_oritentation(orientation)
  {}

  virtual void execute(MapEditorWindow& editor) override;

private:

  Gtk::Orientation const m_oritentation;
};

#endif // MAP_EDITOR_COMMANDS_HPP
