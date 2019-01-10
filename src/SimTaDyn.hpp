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

#ifndef SIMTADYN_CONTEXT_HPP_
#  define SIMTADYN_CONTEXT_HPP_

#  include "Path.hpp"
#  include "SimTaDynWindow.hpp"
#  include "CmdParser/cmdparser.hpp"

// **************************************************************
//! \brief
// **************************************************************
class SimTaDyn
  : public Singleton<SimTaDyn>
{
private:

  friend class Singleton<SimTaDyn>;

  //------------------------------------------------------------------
  //! \brief Private because of Singleton.
  //------------------------------------------------------------------
  SimTaDyn()
    : m_forth_editor(m_forth),
      m_map_editor(m_forth),
      m_window(m_forth_editor, m_map_editor)
  {}

  //------------------------------------------------------------------
  //! \brief Private because of Singleton. Check if resources is still
  //! acquired which show a bug in the management of resources.
  //------------------------------------------------------------------
  ~SimTaDyn()
  {}

public:

  //------------------------------------------------------------------
  //! \brief
  //! \param parser A command line parser (same job than getoption)
  //------------------------------------------------------------------
  void init(cli::Parser& parser);

  //------------------------------------------------------------------
  //! \brief Return the reference of the main UI window.
  //------------------------------------------------------------------
  inline SimTaDynWindow& mainWindow()
  {
    return m_window;
  }

  inline SimForth& forth()
  {
    return m_forth;
  }

  inline MapEditor& mapEditor()
  {
    return m_map_editor;
  }

  inline ForthEditor& forthEditor()
  {
    return m_forth_editor;
  }

  inline GLDrawingArea& drawingArea()
  {
    return m_map_editor.m_drawing_area;
  }

private:

  SimForth            m_forth;
  ForthEditor         m_forth_editor;
  MapEditor           m_map_editor;
  SimTaDynWindow      m_window;
};

#endif /* SIMTADYN_CONTEXT_HPP_ */
