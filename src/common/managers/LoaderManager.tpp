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

#ifndef LOADER_MANAGER_TPP_
#  define LOADER_MANAGER_TPP_

#  include "ILoader.tpp"
#  include "PathManager.hpp"
#  include "Logger.hpp"

// Inspired by: http://loulou.developpez.com/tutoriels/moteur3d/
// document: "Part 4: Gestion des ressources" and its code "YesEngine"
// but this current code is different from the original code.

// **************************************************************
//! \brief Define a class managing a list of ILoader<R>. R for
//! Resource class.
//!
//! ResourceList is a list of ILoader<R>. The exact content of this
//! list is postponed and need to include Utilities/GenHierarchies.h
//! from the YesEngine. This avoid writing long lines of inheritance
//! for each loaders we want to include.
//!
//! LoaderHolder is just a class containing each class of ResourceList.
// **************************************************************
class LoaderManager
  : public Singleton<LoaderManager>,
    public CScatteredHierarchy<ResourceList, LoaderHolder>
{
private:

  //------------------------------------------------------------------
  //! \brief Mandatory by design.
  //------------------------------------------------------------------
  friend class Singleton<LoaderManager>;

  //------------------------------------------------------------------
  //! \brief Private because of Singleton.
  //------------------------------------------------------------------
  LoaderManager()
  {
    registerAllLoaders();
  }

  //------------------------------------------------------------------
  //! \brief Private because of Singleton.
  //------------------------------------------------------------------
  virtual ~LoaderManager()
  {
  }

public:

  //------------------------------------------------------------------
  //! \brief Register a loader and associate it to one or several file
  //! extensions (extensions in the list are separated by the ':' char).
  //------------------------------------------------------------------
  template <class R>
  void registerLoader(std::shared_ptr<ILoader<R>> loader, std::string const& extensions)
  {
    std::stringstream ss(extensions);
    std::string ext;
    while (std::getline(ss, ext, ':'))
      {
        // Ignore empty string as extension
        if (!ext.empty())
          {
            LOGI("Register file '%s' extension to loader %p",
                 ext.c_str(), loader.get());
            LoaderHolder<R>::m_loaders[ext] = loader;
          }
      }
  }

  //------------------------------------------------------------------
  //! \brief Load a template R resource from a file.
  //!
  //! Look for a ILoader<R> according to file extension and load the
  //! resource. Throw a LoaderException if the file does not exist or
  //! if an error occured during the loading of the resource.
  //------------------------------------------------------------------
  template <class R>
  void loadFromFile(std::string const& filename, std::shared_ptr<R>& resource) const
  {
    LOGI("Loading file '%s'", filename.c_str());
    std::pair<std::string, bool> full_path = PathManager::instance().find(filename);
    if (!full_path.second)
      {
        // Note: probably useless because if the file does not exist
        // the loader should throw an exception but here we throw
        // a different message.
        std::string msg("The file '" + filename +
                        "' cannot be found in the given search path '" +
                        PathManager::instance().toString() + "'");
        throw LoaderException(msg);
      }
    try
      {
        ILoader<R>& loader = getLoader<R>(File::extension(full_path.first));
        loader.loadFromFile(full_path.first, resource);
        if (nullptr == resource)
          {
            std::string msg("Get a nullptr resource after loading the file '"
                            + filename + "'");
            throw LoaderException(msg);
          }
        else
          {
            LOGI("Sucessfuly loaded the file '%s'", filename.c_str());
          }
      }
    catch (LoaderException const &e)
      {
        LOGF("Failed loading the file '%s'. Reason is '%s'",
             filename.c_str(), e.what());
        e.rethrow();
      }
  }

  //------------------------------------------------------------------
  //! \brief Save a template R resource inside a file.
  //!
  //! Look for a ILoader<R> according to file extension and load the
  //! resource. Throw a LoaderException if the file does not exist or
  //! if an error occured during the loading of the resource.
  //------------------------------------------------------------------
  template <class R>
  void saveToFile(std::shared_ptr<R> const resource, std::string const& filename) const
  {
    LOGI("Saving file '%s'", filename.c_str());
    try
      {
        ILoader<R>& loader = getLoader<R>(File::extension(filename));
        loader.saveToFile(resource, filename);
        LOGI("Sucessfuly saved the file '%s'", filename.c_str());
      }
    catch (LoaderException const &e)
      {
        LOGF("Failed saving the file '%s'", filename.c_str());
        throw e;
      }
  }

  //------------------------------------------------------------------
  //! \brief Check if a ILoader<R> exists according to file extension.
  //------------------------------------------------------------------
  template <class R>
  inline bool hasLoader(std::string const& extension) const
  {
    return LoaderHolder<R>::m_loaders.find(extension)
      != LoaderHolder<R>::m_loaders.end();
  }

  //------------------------------------------------------------------
  //! \brief Return the list of loaders.
  //------------------------------------------------------------------
  template <class R>
  LoaderContainer<R> const &loaders() const
  {
    return LoaderHolder<R>::m_loaders;
  }

protected:

  virtual void registerAllLoaders();

  //------------------------------------------------------------------
  //! \brief Look in the hash table if the file extension is known and
  //! return the associated ILoader<R>.
  //------------------------------------------------------------------
  template <class R>
  inline ILoader<R>& getLoader(std::string const& extension) const
  {
    auto it = LoaderHolder<R>::m_loaders.find(extension);
    if (!((it != LoaderHolder<R>::m_loaders.end()) && it->second))
      {
        std::string msg("No loader found taking into account this extension file '."
                        + extension + "'");
        LOGF("%s", msg.c_str());
        throw LoaderException(msg);
      }
    return *(it->second);
  }
};

#endif /* LOADER_MANAGER_TPP_ */
