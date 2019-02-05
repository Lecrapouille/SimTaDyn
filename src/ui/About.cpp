//=====================================================================
// SimTaDyn: A GIS in a spreadsheet.
// Copyright 2019 Quentin Quadrat <lecrapouille@gmail.com>
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

#include "About.hpp"
#include "PathManager.hpp"
#include "Logger.hpp"

About::About()
{
  // About window
  set_program_name(config::project_name);
  set_version(std::to_string(config::major_version) + '.' + std::to_string(config::minor_version));
  set_copyright("Copyright ©2004, 2017-2019 Quentin Quadrat");
  set_comments("A GIS in a spreadsheet");
  set_license_type(Gtk::LICENSE_GPL_3_0);
  set_wrap_license(false);
  set_website("https://github.com/Lecrapouille/SimTaDyn");
  set_website_label("Visit " + config::project_name + " github site");
  set_authors({"Quentin Quadrat <lecrapouille@gmail.com>"});

  try
    {
      set_logo(Gdk::Pixbuf::create_from_file(PathManager::instance().expand("icons/SimTaDyn-logo.jpg")));
    }
  catch (...)
    {
      LOGW("SimTaDyn logo not found");
    }

  signal_response().connect([&](const int&) { hide(); });
}
