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

#ifndef DIALOGS_HPP
#  define DIALOGS_HPP

#  include "Gtkmm.tpp"
#  include "Logger.hpp"
#  include "Exception.hpp"
#  include "Config.hpp"
#  include "PathManager.hpp"

// **************************************************************
//! \brief Facade class for Gtk::AboutDialog.
// **************************************************************
class AboutDialog: public Gtk::AboutDialog
{
public:

  AboutDialog()
  {
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
        set_logo(Gdk::Pixbuf::create_from_file(PathManager::expand("icons/SimTaDyn-logo.jpg")));
      }
    catch (...)
      {
        LOGW("SimTaDyn logo not found");
      }

    signal_response().connect([&](const int&) { hide(); });
  }
};

// *************************************************************************************************
//! \brief Facade class for displaying Exception on a GTK::MessageDialog
// *************************************************************************************************
class ExceptionDialog
{
public:

  ExceptionDialog(Gtk::Window& win)
    : m_win(win)
  {}

  ~ExceptionDialog() {}

  void show(Exception const& e, std::string const& reason)
  {
    std::string msg(reason);
    if (!reason.empty())
      msg += ". Reason: " + e.message();

    LOGC("%s - %s", e.what(), msg.c_str());

    Gtk::MessageDialog dialog(m_win, e.what(), false, Gtk::MESSAGE_WARNING);
    dialog.set_secondary_text(msg);
    dialog.run();
  }

  void show(std::string const& what, std::string const& message, std::string const& reason)
  {
    std::string msg(message);
    if (!reason.empty())
      msg += ". Reason: " + reason;

    LOGC("%s - %s", what.c_str(), msg.c_str());

    Gtk::MessageDialog dialog(m_win, what, false, Gtk::MESSAGE_WARNING);
    dialog.set_secondary_text(msg);
    dialog.run();
  }

private:

  Gtk::Window& m_win;
};

#endif // DIALOGS_HPP
