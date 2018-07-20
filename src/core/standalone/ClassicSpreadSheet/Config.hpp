//=====================================================================
// SimForth: A GIS in a spreadsheet.
// Copyright 2017 Quentin Quadrat <lecrapouille@gmail.com>
//
// This file is part of SimForth.
//
// SimForth is free software: you can redistribute it and/or modify it
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

#  include "Singleton.tpp"
#  include "Path.hpp"
#  include "File.hpp"
#  include "version.h"

// **************************************************************
//! \brief
// **************************************************************
class Config:
  public Path,
  public Singleton<Config>
{
private:

  //------------------------------------------------------------------
  //! \brief Mandatory by design.
  //------------------------------------------------------------------
  friend class Singleton<Config>;

  //------------------------------------------------------------------
  //! \brief Private because of Singleton.
  //------------------------------------------------------------------
  Config()
  {
    add(PROJECT_DATA_PATH);
  }

  //------------------------------------------------------------------
  //! \brief Private because of Singleton. Check if resources is still
  //! acquired which show a bug in the management of resources.
  //------------------------------------------------------------------
  ~Config() { };
};

namespace config
{
  //! \brief
  enum Mode { Debug, Release };

  //! \brief
  static const Mode mode = config::Debug;
  //! \brief Either create a new log file or smash the older log.
  static const bool separated_logs = false;
  //! \brief Used for logs and GUI.
  static const std::string project_name("SimExcel");
  //! \brief Major version of project
  static const uint32_t major_version(PROJECT_MAJOR_VERSION);
  //! \brief Minor version of project
  static const uint32_t minor_version(PROJECT_MINOR_VERSION);
  //! \brief Save the git SHA1
  static const std::string git_sha1(PROJECT_SHA1);
  //! \brief Save the git branch
  static const std::string git_branch(PROJECT_BRANCH);
  //! \brief Pathes where default project resources have been installed
  //! (when called  by the shell command: sudo make install).
  static const std::string data_path(PROJECT_DATA_PATH);
  //! \brief Location for storing temporary files
  static const std::string tmp_path(false == separated_logs ?
                                    PROJECT_TEMP_DIR :
                                    File::generateTempFileName(PROJECT_TEMP_DIR, "/"));
  //! \brief Give a name to the default project log file.
  static const std::string log_name(project_name + ".log");
  //! \brief Define the full path for the project.
  static const std::string log_path(tmp_path + log_name);
  //! \brief Number of elements by pool in containers
  //! used for storing nodes and arcs in a graph
  static const uint32_t graph_container_nb_elements(8U);
}

#endif /* CONFIG_HPP_ */
