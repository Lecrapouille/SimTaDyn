// -*- c++ -*- Coloration Syntaxique pour Emacs
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

#ifndef ILOADER_HPP_
#  define ILOADER_HPP_

#  include "ManagerException.hpp"
#  include "Logger.hpp"
#  include <memory>
#  include <map>
#  include <vector>

// *************************************************************************************************
//! \brief Interface for loading/importing and saving/exporting data from/to a file on hard disk.
//! Template R is for the resource.
// *************************************************************************************************
template <class R>
class ILoader
{
public:

  //------------------------------------------------------------------
  //! \brief Dummy virtual destructor for derived class.
  //------------------------------------------------------------------
  virtual ~ILoader() {}

  //------------------------------------------------------------------
  //! \brief Interface for reading data from a file on the hard
  //! file. The default behavior of this method is to trig an 'not
  //! implemented' exception. The derived class con override this
  //! method and implement the 'importing' algorithm.
  //! \throw LoaderException.
  //------------------------------------------------------------------
  virtual void loadFromFile(std::string const& filename, R& /*resource*/)
  {
    std::string msg("Found no loader supporting this kind of file '" + filename);
    LOGF("%s", msg.c_str());
    throw LoaderException(msg);
  }

  //------------------------------------------------------------------
  //! \brief Interface for storing data in a file on the hard
  //! file. The default behavior of this method is to trig an 'not
  //! implemented' exception. The derived class can override this
  //! method and implement the 'exporting' algorithm.
  //! \throw LoaderException.
  //------------------------------------------------------------------
  virtual void saveToFile(R const& /*resource*/, std::string const& filename)
  {
    std::string msg("Found no loader supporting this kind of file '" + filename);
    LOGF("%s", msg.c_str());
    throw LoaderException(msg);
  }

  //------------------------------------------------------------------
  //! \brief Return the reference of the text describing the goal of
  //! the loader (for example needed by Gtk::FileFilter).
  //------------------------------------------------------------------
  inline std::string const& description() const
  {
    return m_description;
  }

protected:

  //------------------------------------------------------------------
  //! \brief Make this class abstract (cannot be instanciated
  //! directly).
  //! \param description the short text describing the goal of the
  //loader.
  //------------------------------------------------------------------
  ILoader(std::string const &description)
    : m_description(description)
  {
  }

private:

  //------------------------------------------------------------------
  //! \brief Short description of the goal of the loader.
  //------------------------------------------------------------------
  std::string m_description;
};

#endif
