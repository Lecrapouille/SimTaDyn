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

#ifndef PATHMANAGER_HPP
#  define PATHMANAGER_HPP

#  include "MyLogger/Path.hpp"

// **************************************************************
//! \brief PathManager wraps the class Path and acts like a singleton.
// **************************************************************
class PathManager
{
public:

  //------------------------------------------------------------------
  //! \brief
  //------------------------------------------------------------------
  PathManager()
  {}

  //------------------------------------------------------------------
  //! \brief
  //------------------------------------------------------------------
  ~PathManager()
  {}

  //------------------------------------------------------------------
  //! \brief
  //------------------------------------------------------------------
  static Path& path()
  {
    static Path path(PROJECT_DATA_PATH);
    return path;
  }

  //! \brief Append a new path. Directories are
  //! separated by ':'. Example: "/foo/bar:/usr/lib/".
  static void add(std::string const& p)
  {
    path().add(p);
  }

  //! \brief Reset the path state. Directories are
  //! separated by ':'. Example: "/foo/bar:/usr/lib/".
  static void reset(std::string const& p)
  {
    path().reset(p);
  }

  //! \brief Erase the path.
  static void clear()
  {
    path().clear();
  }

  //! \brief Erase the given directory from the path.
  static void remove(std::string const& p)
  {
    path().remove(p);
  }

  //! \brief Save temporary the path of currently loading SimTaDynMap
  //! file. The goal of the stack is to avoid to SimForth to have path
  //! conflict with two loaded SimTaDynMap files.
  static void push(std::string const& p)
  {
    path().push(p);
  }

  //! \brief once loaded the path is no longer needed.
  static void pop()
  {
    path().pop();
  }

  //! \brief Get the top of the stack
  static std::string top()
  {
    return path().top();
  }

  //! \brief Find if a file exists in the search path. Note that you
  //! have to check again the existence of this file when opening it
  //! (with functions such as iofstream, fopen, open ...). Indeed the
  //! file may have been suppress since this method have bee called.
  //! \return the full path (if found) and the existence of this path.
  static std::pair<std::string, bool> find(std::string const& filename)
  {
    return path().find(filename);
  }

  //! \brief Return the full path for the file (if found) else
  //! return itself.
  static std::string expand(std::string const& filename)
  {
    return path().expand(filename);
  }

  //! \brief Return the path as string.
  static std::string const& toString()
  {
    return path().toString();
  }
};

#endif // PATHMANAGER_HPP
