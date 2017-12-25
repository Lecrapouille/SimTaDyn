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

#include "SimTaDyn.hpp"
#include "Config.hpp"

void SimTaDynContext::init()
{
  LOGI("Init SimTaDynContext");

  SimTaDynSheet sheet("Sheet0");
  SimForth& forth = SimForth::instance();
  forth.boot();
  assert(sheet.name().compare("Sheet0") == 0);

  CellNode& n0 = sheet.addNode("1 1 +");
  LOGI("CellNode: %p %s %u", &n0, n0.name().c_str(), n0.id());
  CellNode& n1 = sheet.addNode("2 N0 +");
  LOGI("CellNode: %p %s %u", &n1, n1.name().c_str(), n1.id());
  n0.addNeighbor(n1);

  sheet.parse(forth);
  std::pair<bool, std::string> res = sheet.evaluate(forth);
  forth.ok(res);
  if (res.first)
    {
      //sheet.displayResult();
      std::cout << n0.value() << std::endl;
      std::cout << n1.value() << std::endl;
    }
}

// Init Gtkmm and SimTaDyn contexts. Be careful not to create a GTK+
// button before the context creation of GTK libraries.
int main(int argc, char** argv)
{
  std::cout << "Welcome to SimTaDyn version "
            << config::major_version
            << '.'
            << config::minor_version
            << ' '
            << (config::Debug == config::mode ? "Debug" : "Release")
            << " mode"
            << std::endl;

  // Call it before Logger constructor
  if (!File::mkdir(config::tmp_path))
    {
      std::cerr << "Failed creating the temporary directory '"
                << config::tmp_path << "'" << std::endl;
    }

  LOGI("Init GTK");
  const Gtk::Main kit(argc, argv);
  Gsv::init();

  auto SimTaDyn = std::unique_ptr<SimTaDynContext>(new SimTaDynContext);
  if (nullptr == SimTaDyn)
    {
      LOGE("Failed creating the SimTaDynContext GUI. Aborting");
      exit(1);
    }

  SimTaDyn->init();
  kit.run(SimTaDyn->window());

  return 0;
}
