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

#ifndef SIMTADYN_CONTEXT_HPP
#  define SIMTADYN_CONTEXT_HPP

#  include "Path.hpp"
#  include "NonCopyable.hpp"
#  include "MapEditorWindow.hpp"
#  include "ForthEditorWindow.hpp"
#  include "CmdParser/cmdparser.hpp"

// **************************************************************
//! \brief
// **************************************************************
class SimTaDyn : private NonCopyable
{
public:

  //------------------------------------------------------------------
  //! \brief Private because of Singleton.
  //------------------------------------------------------------------
  SimTaDyn(int argc, char** argv);

  //------------------------------------------------------------------
  //! \brief Private because of Singleton. Check if resources is still
  //! acquired which show a bug in the management of resources.
  //------------------------------------------------------------------
  ~SimTaDyn();

  //------------------------------------------------------------------
  //! \brief
  //------------------------------------------------------------------
  int run();

  //------------------------------------------------------------------
  //! \brief
  //------------------------------------------------------------------
  inline static void createMapEditorWindow()
  {
    createWindow<MapEditorWindow>();
  }

  //------------------------------------------------------------------
  //! \brief
  //------------------------------------------------------------------
  inline static void createForthEditorWindow()
  {
    createWindow<ForthEditorWindow>();
  }

  inline static SimForth& forth()
  {
    return m_forth;
  }

  inline static Glib::RefPtr<Gtk::Application>& application()
  {
    return m_application;
  }

private:

  //------------------------------------------------------------------
  //! \brief
  //------------------------------------------------------------------
  template<class W> static void createWindow()
  {
    std::unique_ptr<W> win = std::make_unique<W>();
    if (0_z != m_windows.size())
      {
        m_application->add_window(*(win.get()));
      }
    m_windows.push_back(std::move(win));
  }

  //------------------------------------------------------------------
  //! \brief
  //------------------------------------------------------------------
  void configureOptions();

  //------------------------------------------------------------------
  //! \brief
  //! \param parser A command line parser (same job than getoption)
  //------------------------------------------------------------------
  void init();

private:

  cli::Parser                                     m_parser;
  static SimForth                                 m_forth;
  static Glib::RefPtr<Gtk::Application>           m_application;
  static std::vector<std::unique_ptr<MainWindow>> m_windows;
};

#endif // SIMTADYN_CONTEXT_HPP
