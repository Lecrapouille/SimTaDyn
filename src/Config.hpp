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

#  include "version.h"
#  include "File.hpp"

namespace config
{
  //! \brief
  static const std::string project_name("SimTaDyn");
  //! \brief Major version of SimTaDyn
  static const uint32_t major_version(SIMTADYN_MAJOR_VERSION);
  //! \brief Minor version of SimTaDyn
  static const uint32_t minor_version(SIMTADYN_MINOR_VERSION);
  //! \brief Pathes where default SimTaDyn resources have been installed
  //! (when called  by the shell command: sudo make install).
  static const std::string data_path(SIMTADYN_DATA_PATH);
  //! \brief Location for storing temporary files
  static const std::string tmp_path(File::generateTempFileName(SIMTADYN_TEMP_DIR, "/"));
  //! \brief Give a name to the default project log file.
  static const std::string log_name(project_name + ".log");
  //! \brief Define the full path for SimTaDyn project.
  static const std::string log_path(tmp_path + log_name);
  //! \brief Number of elements by pool in containers
  //! used for storing nodes and arcs in a graph
  static const uint32_t graph_container_nb_elements(8U);
}

#endif /* CONFIG_HPP_ */
