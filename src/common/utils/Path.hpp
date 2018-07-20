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

#ifndef PATH_HPP_
#  define PATH_HPP_

#  include <list>
#  include <string>

class Path
{
public:

  //! \brief Empty constructor.
  Path();

  //! \brief Constructor. init with a given path. Directories are
  //! separated by ':'. Example: "/foo/bar:/usr/lib/".
  Path(std::string const& path);

  //! \brief Destructor.
  ~Path();

  //! \brief Append a new path. Directories are
  //! separated by ':'. Example: "/foo/bar:/usr/lib/".
  void add(std::string const& path);

  //! \brief Replace the path. Directories are
  //! separated by ':'. Example: "/foo/bar:/usr/lib/".
  void init(std::string const& path);

  //! \brief Erase the path.
  void clear();

  //! \brief Erase the given directory from the path.
  void remove(std::string const& path);

  //! \brief Find if a file exists in the path.
  //! \return the full path (if found) and the existence of this path.
  std::pair<std::string, bool> find(std::string const& filename) const;

  //! \brief Return the full path for the file (if found) else
  //! return itself.
  std::string expand(std::string const& filename) const;

  //! \brief Return the path as string.
  std::string const &toString() const;

protected:

  void update();

  void split(std::string const& path);

  const char m_delimiter = ':';
  std::list<std::string> m_paths;
  std::string m_path;
};

#endif
