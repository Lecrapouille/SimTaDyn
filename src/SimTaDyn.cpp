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

// FIXME: this is a temporary example
void SimTaDynContext::init(cli::Parser& parser)
{
  LOGI("Parsing project options");
  PathManager::instance().add(parser.get<std::string>("p"));
  LOGI("%s", PathManager::instance().toString().c_str());

  SimForth& forth = SimForth::instance();
  forth.boot();

  // FIXME TEMPORARY CODE
#if 0
  // FIXME: simple example to move in unit tests
  SimTaDynSheet* sheet = new SimTaDynSheet("Sheet0"); // Ok leak but just for example
  assert(sheet->name().compare("Sheet0") == 0);

  CellNode& n0 = sheet->addNode("1 1 +");
  LOGI("CellNode: %p %s %u", &n0, n0.name().c_str(), n0.id());
  CellNode& n1 = sheet->addNode("2 N0 +");
  LOGI("CellNode: %p %s %u", &n1, n1.name().c_str(), n1.id());
  n0.addNeighbor(n1);

  sheet->parse(forth);
  std::pair<bool, std::string> res = sheet->evaluate(forth);
  forth.ok(res);
  if (res.first)
    {
      //sheet->displayResult();
      std::cout << n0.rawValue() << std::endl;
      std::cout << n1.rawValue() << std::endl;
    }
#endif
}

static void configure_options(cli::Parser& parser)
{
  parser.set_optional<std::string>("p", "path", "", "Add pathes for searching datum. Use ':' for separate pathes");
}

// Init Gtkmm and SimTaDyn contexts. Be careful not to create a GTK+
// button before the context creation of GTK libraries.
int main(int argc, char** argv)
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
  cli::Parser parser(argc, argv);
  configure_options(parser);
  parser.run_and_exit_if_error();

  LOGI("Init GTK");
  auto app = Gtk::Application::create();
  Gsv::init();

  LOGI("Init SimTaDyn");
  auto SimTaDyn = std::make_unique<SimTaDynContext>();
  SimTaDyn->init(parser);

  LOGI("Start SimTaDyn main loop");
  app->run(SimTaDyn->window());

  return 0;
}
