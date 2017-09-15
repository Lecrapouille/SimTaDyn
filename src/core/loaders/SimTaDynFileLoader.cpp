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

#include "SimTaDynFileLoader.hpp"
#include "File.hpp"
#include "zipper/unzipper.h"
#include "zipper/zipper.h"
#include <cstdio>

std::string SimTaDynFileLoader::generateTempDirName() const
{
  char buffer_time[32];
  std::string path(config::tmp_path);
  time_t current_time = time(nullptr);

  strftime(buffer_time, sizeof (buffer_time), "%Y-%m-%d/", localtime(&current_time));
  path += buffer_time;

  strftime(buffer_time, sizeof (buffer_time), "%Hh-%Mm-%Ss", localtime(&current_time));
  path += buffer_time;

  return path;
}

// FIXME password
bool SimTaDynFileLoader::unzip(std::string const &zip_file)
{
  zipper::Unzipper unzipper(zip_file);

  m_base_dir = generateTempDirName();
  if (false == File::mkdir(m_base_dir))
    {
      LOGE("Failed unzipping '%s' could not create the temporary dir", m_base_dir.c_str());
      return false;
    }
  if (false == unzipper.extract(m_base_dir))
    {
      LOGE("Failed unzipping '%s' could not create the temporary dir", m_base_dir.c_str());
      return false;
    }

  return true;
}

void SimTaDynFileLoader::loadFromFile(std::string const& filename, SimTaDynFile* &current_project)
{
  bool dummy_project = (nullptr == current_project);

  LOGI("Loading the SimTaDynFile '%s' in an %s",
       filename.c_str(), (dummy_project ? "dummy map" : "already opened map"));

  if (false == unzip(filename))
    {
      LoaderException e("Failed unzipping file '" + filename + "'");
      throw e;
    }

  if (dummy_project)
    {
      current_project = new SimTaDynFile();
    }
  current_project->m_name = File::baseName(filename);
  current_project->m_zip_path = filename;
  current_project->m_base_dir = m_base_dir;
  current_project->m_full_path = current_project->m_base_dir + '/' + current_project->m_name;

  /* Path::instance().add(base_dir);
  forth = Forth::instance();
  forth.backup(save);
  bool res = forth.read(Path::instance().get("Index.fth"));
  if (false == res)
    {
      // unload all maps and resources
      forth.backup(restore);
    }
  */
}

// FIXME password
bool SimTaDynFileLoader::zip(SimTaDynFile const& project, std::string const& filename)
{
  if (filename == project.m_zip_path)
    {
      // FIXME pas le plus safe si zipper.add echoue
      std::remove(filename.c_str());
    }

  zipper::Zipper zipper(filename);
  bool ret = zipper.add(project.m_full_path);
  if (false == ret)
    {
      LOGE("Failed zipping the dir '%s'", project.m_full_path.c_str());
    }

  return ret;
}

void SimTaDynFileLoader::saveToFile(SimTaDynFile const& project, std::string const& filename)
{
  if (false == zip(project, filename))
    {
      LoaderException e("Failed zipping file '" + filename + "'");
      throw e;
    }
}
