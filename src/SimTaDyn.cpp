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

#include "SimTaDyn.hpp"
#include "Config.hpp"
#include "PathManager.hpp"
#include <cstdlib>

SimTaDyn::SimTaDyn(int argc, char** argv)
  : m_parser(argc, argv)
{
  termcolor::enable();
  std::cout << "Welcome to SimTaDyn version "
            << config::major_version
            << '.'
            << config::minor_version
            << ' '
            << (config::Debug == config::mode ? "Debug" : "Release")
            << " mode "
            << std::endl
            << "git: " << config::git_branch << " " << config::git_sha1
            << std::endl;

  // Call it before Logger constructor
  if (!File::mkdir(config::tmp_path))
    {
      std::cerr << "Failed creating the temporary directory '"
                << config::tmp_path << "'" << std::endl;
    }

  LOGI("Init option parser");
  configureOptions();
  m_parser.run_and_exit_if_error();

  LOGI("Init GTK");
  application() = Gtk::Application::create();
  Gsv::init();

  // On startup create all main windows
  createForthEditorWindow();
  application()->signal_startup().connect([&]{
      createMapEditorWindow();
      init();
    });
}

SimTaDyn::~SimTaDyn()
{
}

int SimTaDyn::run()
{
  WindowContainer& win = windows();
  return (0_z != win.size())
    ? application()->run(*(win[0].get()))
    : EXIT_FAILURE;
}

void SimTaDyn::configureOptions()
{
  m_parser.set_optional<std::string>("p", "path", "", "Add pathes for searching datum. Use ':' for separate pathes");
}

void SimTaDyn::init()
{
  LOGI("Parsing project options");
  PathManager::instance().add(m_parser.get<std::string>("p"));
  LOGI("%s", PathManager::instance().toString().c_str());

  forth().boot();
}
