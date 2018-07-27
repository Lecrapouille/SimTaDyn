//=====================================================================
// SimTaDyn: A GIS in a spreadsheet.
// Copyright 2017 Quentin Quadrat <lecrapouille@gmail.com>
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

#  include "Logger.hpp"
#  include "File.hpp"
#  include <list>

class Path
{
public:

  Path()
  {
  }

  Path(std::string const& path)
  {
    split(path);
  }

  virtual ~Path()
  {
  }

  //! \brief add a directory in the path
  void add(std::string const& path)
  {
    LOGI("Path::add '%s'", path.c_str());
    split(path);
  }

  //! \brief add a directory in the path
  void init(std::string const& path)
  {
    LOGI("Path::clear()");
    m_paths.clear();
    split(path);
  }

  void clear()
  {
    m_paths.clear();
    m_path.clear();
  }

  void remove(std::string const& path)
  {
    LOGI("Path::remove '%s'", path.c_str());
    m_paths.remove(path);
  }

  std::pair<std::string, bool> find(std::string const& filename) const
  {
    for (auto it: m_paths)
    {
      std::string file(it + filename);
      if (File::exist(file))
        return std::make_pair(file, true);
    }
    if (File::exist(filename))
      return std::make_pair(filename, true);

    // Not found
    return std::make_pair(std::string(), false);
  }

  std::string expand(std::string const& filename) const
  {
    for (auto it: m_paths)
    {
      std::string file(it + filename);
      if (File::exist(file))
        return file;
    }

    return filename;
  }

  std::string const &toString() const
  {
    return m_path;
  }

protected:

  void update()
  {
    m_path = "";
    for (auto it: m_paths)
      {
        m_path += it;
        m_path.pop_back(); // Remove the '/' char
        m_path += m_delimiter;
      }
  }

  void split(std::string const& path)
  {
    std::stringstream ss(path);
    std::string directory;

    while (std::getline(ss, directory, m_delimiter))
      {
        if (directory.empty())
          continue ;

        if ((*directory.rbegin() == '\\') || (*directory.rbegin() == '/'))
          m_paths.push_back(directory);
        else
          m_paths.push_back(directory + "/");
      }
    update();
  }

  const char m_delimiter = ':';
  std::list<std::string> m_paths;
  std::string m_path;
};

#endif
