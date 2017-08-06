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

#ifndef CONFIG_HPP_
#  define CONFIG_HPP_

#  include "Singleton.hpp"
#  include "version.h"
#  include <string>
#  include <glibmm.h>

class Config: public Singleton<Config>
{
  friend class Singleton<Config>;

public:

  //! \brief
  inline std::string data_path(const std::string& path) const
  {
    return m_data_path + path;
  }

  //! \brief
  inline std::string data_path(const char *path) const
  {
    return m_data_path + path;
  }

  inline const char *char_data_path() const
  {
    return m_data_path.c_str();
  }

  std::string m_data_path;

private:

  Config()
    : m_data_path(Glib::get_home_dir() + std::string("/.SimTaDyn/data/"))
  {
  }
};

namespace config
{
  //! \brief
  const char project_name[] = "SimTaDyn";
  //! \brief Major version of SimTaDyn
  const uint32_t major_version = SIMTADYN_MAJOR_VERSION;
  //! \brief Minor version of SimTaDyn
  const uint32_t minor_version = SIMTADYN_MINOR_VERSION;
  //! \brief
  const char log_path[] = "SimTaDyn.log";
  //! \brief
  const char tmp_log_path[] = "/tmp/SimTaDyn.log";
  //! \brief Number of elements by pool in containers
  //! used for storing nodes and arcs in a graph
  const uint32_t graph_container_nb_elements = 8U;
}

#endif /* CONFIG_HPP_ */
