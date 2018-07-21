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
// along with GNU Emacs.  If not, see <http://www.gnu.org/licenses/>.
//=====================================================================

#ifndef ILOADER_HPP_
#  define ILOADER_HPP_

#  include "LoaderException.hpp"
#  include "Logger.hpp"
#  include <memory>
#  include <map>
#  include <vector>

// **************************************************************
//! \brief Interface for loading and saving data from/to a file.
//! Template R is for IResource but not necessary.
// **************************************************************
template <class R>
class ILoader
{
public:

  //------------------------------------------------------------------
  //! \brief Dummy virtual destructor for inheritance.
  //------------------------------------------------------------------
  virtual ~ILoader() {}

  //------------------------------------------------------------------
  //! \brief Interface for read datum from a file. The derived class
  //! implementing this method shall return the address of the T-typed
  //! class holding these datum. Calling this method will produce an
  //! exception.
  //------------------------------------------------------------------
  virtual void loadFromFile(std::string const& filename, std::shared_ptr<R> &/*resource*/)
  {
    std::string msg("Found no loader supporting this kind of file '"
                    + filename);
    LOGF("%s", msg.c_str());
    throw LoaderException(msg);
  }

  //------------------------------------------------------------------
  //! \brief Interface for storing datum in a file. The derived class
  //! implementing this method shall pass the T-typed class holding
  //! these datum. Calling this method will produce an exception.
  //------------------------------------------------------------------
  virtual void saveToFile(std::shared_ptr<R> const /*resource*/, std::string const& filename)
  {
    std::string msg("Found no loader supporting this kind of file '"
                    + filename);
    LOGF("%s", msg.c_str());
    throw LoaderException(msg);
  }

  //------------------------------------------------------------------
  //! \brief Return the reference of the text describing the goal of
  //! the loader (for example needed by Gtk::FileFilter).
  //------------------------------------------------------------------
  inline std::string const &description() const
  {
    return m_description;
  }

protected:

  //------------------------------------------------------------------
  //! \brief Made this class abstract (cannot be instanciated).
  //! \param description Short text describing the goal of the loader.
  //------------------------------------------------------------------
  ILoader(std::string const &description)
  : m_description(description)
  {
  }

  //------------------------------------------------------------------
  //! \brief Short description of the goal of the loader.
  //------------------------------------------------------------------
  std::string m_description;
};

// **************************************************************
//! \brief Same goal than 'typedef'.
// **************************************************************
template <class R>
using LoaderContainer = std::map<std::string, std::shared_ptr<ILoader<R>>>;

// **************************************************************
//! \brief Hold loaders. Needed for meta-programming
// **************************************************************
template <class R>
struct LoaderHolder
{
  //! \brief Hash table of loaders. Ordered by file extension.
  LoaderContainer<R> m_loaders;
};

#endif
