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
#include "SimTaDynForth.hpp"
#include "zipper/unzipper.h"
#include "zipper/zipper.h"
#include "Config.hpp"
#include <cstdio>

// FIXME password
void SimTaDynFileLoader::unzip(std::string const &zip_file)
{
  std::string msg;

  try
    {
      zipper::Unzipper unzipper(zip_file);
      if (false == unzipper.extract(config::tmp_path))
        {
          msg = "Failed unzipping '" + zip_file
            + "': could not extract '"
            + config::tmp_path + "' from the tarball";
        }
    }
  catch (std::exception const&)
    {
      msg = "Failed unzipping '" + zip_file
        + "': could not locate this file";
    }

  if (!msg.empty())
    {
      LoaderException e(msg);
      throw e;
    }
}

// TODO: si on met a jour une carte et que ca bug au chargement on a corrompu l'ancienne carte.
void SimTaDynFileLoader::loadFromFile(std::string const& filename, SimTaDynMapPtr& current_project)
{
  //SimTaDynMapPtr current_project = SimTaDynMapManager::instance().acquire(filename);
  bool dummy_project = (nullptr == current_project); // A supprimer: dans MapEditor on creer avant la carte

  LOGI("Loading the SimTaDynMap '%s' in an %s", filename.c_str(),
       (dummy_project ? "dummy map" : "already opened map"));

  unzip(filename);
  if (dummy_project)
    {
      current_project = SimTaDynMapManager::instance().create(filename);
      if (nullptr == current_project)
        {
          LoaderException e("nullptr");
          throw e;
        }
    }

  current_project->m_name = File::baseName(filename);
  current_project->m_zip_path = filename;
  current_project->m_full_path = config::tmp_path + current_project->m_name;

  SimForth &forth = SimForth::instance();
  const auto& res = forth.interpreteFile(current_project->m_full_path + "/Index.fth");
  forth.ok(res);
  if (false == res.first)
    {
      LoaderException e(res.second);
      throw e;
    }
}

// FIXME password
bool SimTaDynFileLoader::zip(SimTaDynMapPtr const map, std::string const& filename)
{
  if (filename == map->m_zip_path)
    {
      // FIXME pas le plus safe si zipper.add echoue
      std::remove(filename.c_str());
    }

  zipper::Zipper zipper(filename);
  bool ret = zipper.add(map->m_full_path);
    if (false == ret)
    {
      // FIXME: on a perdu filename
      LOGE("Failed zipping the dir '%s'", map->m_full_path.c_str());
    }

  return ret;
}

void SimTaDynFileLoader::saveToFile(SimTaDynMapPtr const map, std::string const& filename)
{
  if (false == zip(map, filename))
    {
      LoaderException e("Failed zipping file '" + filename + "'");
      throw e;
    }
}
