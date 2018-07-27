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

#include "Path.hpp"
#include "Logger.hpp"
#include "File.hpp"

Path::Path()
{
}

Path::Path(std::string const& path)
{
  split(path);
}

Path::~Path()
{
}

//! \brief add a directory in the path
void Path::add(std::string const& path)
{
  LOGI("Path::add '%s'", path.c_str());
  split(path);
}

//! \brief add a directory in the path
void Path::init(std::string const& path)
{
  LOGI("Path::clear()");
  m_paths.clear();
  split(path);
}

void Path::clear()
{
  m_paths.clear();
  m_path.clear();
}

void Path::remove(std::string const& path)
{
  LOGI("Path::remove '%s'", path.c_str());
  m_paths.remove(path);
  update();
}

std::pair<std::string, bool> Path::find(std::string const& filename) const
{
  if (File::exist(filename))
    return std::make_pair(filename, true);

  for (auto it: m_paths)
    {
      std::string file(it + filename);
      if (File::exist(file))
        return std::make_pair(file, true);
    }

  // Not found
  return std::make_pair(std::string(), false);
}

std::string Path::expand(std::string const& filename) const
{
  for (auto it: m_paths)
    {
      std::string file(it + filename);
      if (File::exist(file))
        return file;
    }

  return filename;
}

std::string const &Path::toString() const
{
  return m_path;
}

void Path::update()
{
  m_path.clear();
  for (auto it: m_paths)
    {
      m_path += it;
      m_path.pop_back(); // Remove the '/' char
      m_path += m_delimiter;
    }
}

void Path::split(std::string const& path)
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
