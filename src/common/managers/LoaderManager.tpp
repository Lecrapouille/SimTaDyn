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

#ifndef LOADER_MANAGER_TPP_
#  define LOADER_MANAGER_TPP_

// ***********************************************************************************************
// Inspired by: http://loulou.developpez.com/tutoriels/moteur3d/
// document: "Part 4: Gestion des ressources" and its code "YesEngine"
// but this current code is different from the original code.
// ***********************************************************************************************

#  include "Singleton.tpp"
#  include "ILoader.tpp"
#  include "PathManager.hpp"
#  include <map>
#  include <memory>

template <class R>
using LoaderContainer = std::map<std::string, std::unique_ptr<ILoader<R>>>;

// ***********************************************************************************************
//! \brief Class holding a std::map. This will be needed for multiple
//! inheritance with the LoaderManager. Template I is for the resource
//! identifier (int, enum, string) and R for the resource.
// ***********************************************************************************************
template <class R>
struct LoaderHolder
{
  LoaderContainer<R> m_loaders;
};

// **************************************************************
//! \brief Define a class managing a set of ILoader<R> where R for
//! a resource class. ILoader loads/save a resource to the
//! ResourceManager class.
//!
//! To avoid adding a long list of inheritance we include
//! Utilities/GenHierarchies.h from the YesEngine which will contain
//! the list of resources.
// **************************************************************
class LoaderManager
  : public Singleton<LoaderManager>,
    public CScatteredHierarchy<ResourceList, LoaderHolder>
{
private:

  //------------------------------------------------------------------
  //! \brief Mandatory by the Signleton design.
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
  template <class L, class R>
  void registerLoader(std::string const& extensions)
  {
    std::stringstream ss(extensions);
    std::string ext;

    while (std::getline(ss, ext, ':'))
      {
        // Ignore empty string as extension
        if (!ext.empty())
          {
            LOGI("Register file '%s' extension to loaderManager", ext.c_str());
            LoaderHolder<R>::m_loaders[ext] = std::make_unique<L>();
          }
      }
  }

  //------------------------------------------------------------------
  //! \brief Load a template R resource from a file.
  //!
  //! Look for a ILoader<R> according to file extension and load the
  //! resource. Throw a LoaderManagerException if the file does not exist or
  //! if an error occured during the loading of the resource.
  //------------------------------------------------------------------
  template <class R>
  void loadFromFile(std::string const& filename, R& resource) const
  {
    LOGI("Loading file '%s'", filename.c_str());
    std::pair<std::string, bool> full_path = PathManager::find(filename);
    if (!full_path.second)
      {
        // Note: probably useless because if the file does not exist
        // the loader should throw an exception but here we throw
        // a different message.
        std::string msg("The file '" + filename +
                        "' cannot be found in the given search path '" +
                        PathManager::toString() + "'");
        throw LoaderManagerException(msg);
      }
    try
      {
        ILoader<R>& loader = getLoader<R>(File::extension(full_path.first));
        loader.loadFromFile(full_path.first, resource);
        LOGI("Sucessfuly loaded the file '%s'", filename.c_str());
      }
    catch (LoaderException const &e)
      {
        LOGF("Failed loading the file '%s'. Reason is '%s'",
             filename.c_str(), e.what());
        e.rethrow();
      }
    catch (LoaderManagerException const &e)
      {
        LOGF("Failed loading the file '%s'. Reason is '%s'",
             filename.c_str(), e.what());
        throw LoaderException(e.message());
      }
    catch (...)
      {
        throw LoaderException("loadFromFile");
      }
  }

  //------------------------------------------------------------------
  //! \brief Save a template R resource inside a file.
  //!
  //! Look for a ILoader<R> according to file extension and load the
  //! resource. Throw a LoaderManagerException if the file does not exist or
  //! if an error occured during the loading of the resource.
  //------------------------------------------------------------------
  template <class R>
  void saveToFile(R const& resource, std::string const& filename) const
  {
    LOGI("Saving file '%s'", filename.c_str());
    try
      {
        ILoader<R> const& loader = getLoader<R>(File::extension(filename));
        loader.saveToFile(resource, filename);
        LOGI("Sucessfuly saved the file '%s'", filename.c_str());
      }
    catch (LoaderException const &e)
      {
        LOGF("Failed loading the file '%s'. Reason is '%s'",
             filename.c_str(), e.what());
        e.rethrow();
      }
    catch (LoaderManagerException const &e)
      {
        LOGF("Failed saving the file '%s'", filename.c_str());
        throw LoaderException(e.what());
      }
    catch (...)
      {
        throw LoaderException("loadFromFile");
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
  LoaderContainer<R> const& loaders() const
  {
    return LoaderHolder<R>::m_loaders;
  }

private:

  virtual void registerAllLoaders();

  //------------------------------------------------------------------
  //! \brief Look in the hash table if the file extension is known and
  //! return the associated ILoader<R>.
  //------------------------------------------------------------------
  template <class R>
  ILoader<R>& getLoader(std::string const& extension) const
  {
    auto const& it = LoaderHolder<R>::m_loaders.find(extension);
    if ((it == LoaderHolder<R>::m_loaders.end()) || (nullptr == it->second))
      {
        std::string msg("No loader found taking into account this "
                        "extension file '" + extension + "'");
        LOGF("%s", msg.c_str());
        throw LoaderManagerException(msg);
      }
    LOGD("Found loader");
    return *(it->second);
  }
};

#endif /* LOADER_MANAGER_TPP_ */
