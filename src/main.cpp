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

// Init Gtkmm and SimTaDyn contexts. Be careful not to create a GTK+
// button before the context creation of GTK libraries.
int main(int argc, char** argv)
{
  std::cout << "Welcome to SimTaDyn version "
            << config::major_version
            << '.'
            << config::minor_version
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
