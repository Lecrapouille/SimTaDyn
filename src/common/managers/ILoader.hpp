// -*- c++ -*- Coloration Syntaxique pour Emacs
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

#ifndef ILOADER_HPP_
#  define ILOADER_HPP_

#  include "LoaderException.hpp"
#  include "Logger.hpp"
#  include <memory>
#  include <map>
#  include <vector>

// ***********************************************************************************************
//! \brief Define callbacks on loader events
// ***********************************************************************************************
template <class T>
class ILoaderListener
{
public:

  ILoaderListener() { };
  virtual ~ILoaderListener() { };
  //------------------------------------------------------------------
  //! \brief Callback when a failure occured during the loading of file
  //! \param filename the full path of the file.
  //! \param msg the error message returned by the loader.
  //------------------------------------------------------------------
  virtual void onLoadFailure(std::string const& filename, std::string const& msg) = 0;
  //------------------------------------------------------------------
  //! \brief CCallback when a file has been successfully loaded.
  //! \param filename the full path of the file.
  //------------------------------------------------------------------
  virtual void onLoadSucess(std::string const& filename) = 0;
  //------------------------------------------------------------------
  //! \brief Callback when a failure occured during the saving of file
  //! \param filename the full path of the file.
  //! \param msg the error message returned by the loader.
  //------------------------------------------------------------------
  virtual void onSaveFailure(std::string const& filename, std::string const& msg) = 0;
  //------------------------------------------------------------------
  //! \brief Callback when a file has been successfully saved.
  //! \param filename the full path of the file.
  //------------------------------------------------------------------
  virtual void onSaveSucess(std::string const& filename) = 0;
};

// **************************************************************
//! \brief Interface for loading and saving data from/to a file.
// **************************************************************
template <class T>
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
  virtual void loadFromFile(std::string const& filename, T* &/*object*/)
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
  virtual void saveToFile(T const& /*object*/, std::string const& filename)
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
template <class T>
using LoaderContainer = std::map<std::string, std::shared_ptr<ILoader<T>>>;

// **************************************************************
//! \brief Hold loaders. Needed for meta-programming
// **************************************************************
template <class T>
struct LoaderHolder
{
  //! \brief Hash table of loaders
  LoaderContainer<T> m_loaders;
  //! \brief List of observers attached to loader events.
  std::vector<ILoaderListener<T>*> m_listeners;
};

#endif
