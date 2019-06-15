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

#ifndef PATH_HPP_
#  define PATH_HPP_

#  include <list>
#  include <string>
#  include <vector>

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

  //! \brief Reset the path state. Directories are
  //! separated by ':'. Example: "/foo/bar:/usr/lib/".
  void reset(std::string const& path);

  //! \brief Erase the path.
  void clear();

  //! \brief Erase the given directory from the path.
  void remove(std::string const& path);

  //! \brief Save temporary the path of currently loading SimTaDynMap
  //! file. The goal of the stack is to avoid to SimForth to have path
  //! conflict with two loaded SimTaDynMap files.
  void push(std::string const& path)
  {
    m_stack_path.push_back(path);
  }

  //! \brief once loaded the path is no longer needed.
  void pop()
  {
    if (!m_stack_path.empty())
      {
        m_stack_path.pop_back();
      }
  }

  //! \brief Get the top of the stack
  std::string top() const
  {
    return m_stack_path.back();
  }

  //! \brief Find if a file exists in the search path. Note that you
  //! have to check again the existence of this file when opening it
  //! (with functions such as iofstream, fopen, open ...). Indeed the
  //! file may have been suppress since this method have bee called.
  //! \return the full path (if found) and the existence of this path.
  std::pair<std::string, bool> find(std::string const& filename) const;

  //! \brief Return the full path for the file (if found) else
  //! return itself.
  std::string expand(std::string const& filename) const;

  //! \brief Return the path as string.
  std::string const& toString() const;

protected:

  void update();

  void split(std::string const& path);

protected:

  //! \brief Path separator when several pathes are given as a single
  //! string.
  const char m_delimiter = ':';
  //! \brief the list of pathes.
  std::list<std::string> m_search_paths;
  //! \brief the list of pathes converted as a string. Pathes are
  //! separated by the m_delimiter char.
  std::string m_string_path;
  //! \brief Stack of temporary pathes. A temporary path is pushed
  //! when loading a SimTaDyn file: this allows to traverse its
  //! resources (a SimTaDyn file is a zip file containing directories
  //! and files). A stack is usefull when loading a SimTaDyn file that
  //! loads another SimTaDyn file.)
  std::vector<std::string> m_stack_path;
};

#endif
