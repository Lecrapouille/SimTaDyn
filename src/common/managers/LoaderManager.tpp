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

#  include "ILoader.hpp"
#  include "Config.hpp"
#  include "Logger.hpp"

// Inspired by: http://loulou.developpez.com/tutoriels/moteur3d/
// document: "Part 4: Gestion des ressources" and its code "YesEngine"
// but this current code is different than the original code.
using namespace Yes;

// **************************************************************
//! \brief Define a class managing a list of ILoader<T>.
//!
//! ResourceList is a list of ILoader<T>. The exact content of this
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
  LoaderManager() { };

  //------------------------------------------------------------------
  //! \brief Private because of Singleton.
  //------------------------------------------------------------------
  virtual ~LoaderManager() { };

public:

  //------------------------------------------------------------------
  //! \brief Register a loader and associate it to one or several file
  //! extensions (extensions in the list are separated by the ':' char).
  //------------------------------------------------------------------
  template <class T>
  void registerLoader(ILoader<T>* loader, std::string const& extensions)
  {
    std::stringstream ss(extensions);
    std::string ext;
    while (std::getline(ss, ext, ':'))
      {
        // Ignore empty string as extension
        if (!ext.empty())
          {
            LOGI("Register file '%s' extension to loader %p", ext.c_str(), loader);
            std::shared_ptr<ILoader<T>> ptr(loader);
            LoaderHolder<T>::m_loaders[ext] = ptr;
          }
      }
  }

  //------------------------------------------------------------------
  //! \brief Load a T-typed resource from a file.
  //!
  //! Look for a ILoader<T> according to file extension and load the
  //! resource. Throw a LoaderException if the file does not exist or
  //! if an error occured during the loading of the resource.
  //------------------------------------------------------------------
  template <class T>
  void loadFromFile(std::string const& filename, T* &object) const
  {
    LOGI("Loading file '%s'", filename.c_str());
    std::pair<std::string, bool> full_path = Config::instance().find(filename);
    if (!full_path.second)
      {
        // Note: probably useless because if the file does not exist
        // the loader should throw an exception but here we throw
        // a different message.
        std::string msg("The file '" + filename +
                        "' cannot be found in the given search path '" +
                        Config::instance().toString() + "'");
        notifyLoadFailure<T>(filename, msg);
        throw LoaderException(msg);
      }
    try
      {
        ILoader<T>& loader = getLoader<T>(File::extension(full_path.first));
        loader.loadFromFile(full_path.first, object);
        LOGI("Sucessfuly loaded the file '%s'", filename.c_str());
        notifyLoadSucess<T>(full_path.first);
      }
    catch (LoaderException const &e)
      {
        LOGF("Failed loading the file '%s'", filename.c_str());
        notifyLoadFailure<T>(full_path.first, e.what());
        e.rethrow();
      }
  }

  //------------------------------------------------------------------
  //! \brief Save a T-typed resource inside a file.
  //!
  //! Look for a ILoader<T> according to file extension and load the
  //! resource. Throw a LoaderException if the file does not exist or
  //! if an error occured during the loading of the resource.
  //------------------------------------------------------------------
  template <class T>
  void saveToFile(T const& object, std::string const& filename) const
  {
    LOGI("Saving file '%s'", filename.c_str());
    try
      {
        ILoader<T>& loader = getLoader<T>(File::extension(filename));
        loader.saveToFile(object, filename);
        LOGI("Sucessfuly saved the file '%s'", filename.c_str());
        notifySaveSucess<T>(filename);
      }
    catch (LoaderException const &e)
      {
        LOGF("Failed saving the file '%s'", filename.c_str());
        notifySaveFailure<T>(filename, e.what());
        throw e;
      }
  }

  //------------------------------------------------------------------
  //! \brief Check if a ILoader<T> exists according to file extension.
  //------------------------------------------------------------------
  template <class T>
  inline bool hasLoader(std::string const& extension) const
  {
    return LoaderHolder<T>::m_loaders.find(extension)
      != LoaderHolder<T>::m_loaders.end();
  }

  //------------------------------------------------------------------
  //! \brief Return the list of loaders.
  //------------------------------------------------------------------
  template <class T>
  LoaderContainer<T> const &loaders() const
  {
    return LoaderHolder<T>::m_loaders;
  }

//FIXME ---> Mettre dans un classe Listener<T> + macro pour generer les notify

  //! \brief Attach a new listener to loader events.
  template <class T>
  void addListener(ILoaderListener<T>& listener)
  {
    LoaderHolder<T>::m_listeners.push_back(&listener);
  }

  //! \brief Remove the given listener from events.
  template <class T>
  void removeListener(ILoaderListener<T>& listener)
  {
    auto it = std::find(LoaderHolder<T>::m_listeners.begin(),
                        LoaderHolder<T>::m_listeners.end(), &listener);
    if (it != LoaderHolder<T>::m_listeners.end())
      {
        LoaderHolder<T>::m_listeners.erase(it);
      }
  }

  //! \brief Notify all listeners that the resource was succesfully loaded.
  template <class T>
  void notifySaveSucess(std::string const& filename) const
  {
    uint32_t i = LoaderHolder<T>::m_listeners.size();
    while (i--)
      {
        LoaderHolder<T>::m_listeners[i]->onSaveSucess(filename);
      }
  }

  //! \brief Notify
  template <class T>
  void notifySaveFailure(std::string const& filename, std::string const& msg) const
  {
    uint32_t i = LoaderHolder<T>::m_listeners.size();
    while (i--)
      {
        LoaderHolder<T>::m_listeners[i]->onSaveFailure(filename, msg);
      }
  }

  //! \brief Notify all listeners that the resource was succesfully loaded.
  template <class T>
  void notifyLoadSucess(std::string const& filename) const
  {
    uint32_t i = LoaderHolder<T>::m_listeners.size();
    while (i--)
      {
        LoaderHolder<T>::m_listeners[i]->onLoadSucess(filename);
      }
  }

  //! \brief Notify
  template <class T>
  void notifyLoadFailure(std::string const& filename, std::string const& msg) const
  {
    uint32_t i = LoaderHolder<T>::m_listeners.size();
    while (i--)
      {
        LoaderHolder<T>::m_listeners[i]->onLoadFailure(filename, msg);
      }
  }

//FIXME <--- Mettre dans un classe Listener<T> + macro pour generer les notify

protected:

  //------------------------------------------------------------------
  //! \brief Look in the hash table if the file extension is known and
  //! return the associated ILoader<T>.
  //------------------------------------------------------------------
  template <class T>
  inline ILoader<T>& getLoader(std::string const& extension) const
  {
    auto it = LoaderHolder<T>::m_loaders.find(extension);
    if (!((it != LoaderHolder<T>::m_loaders.end()) && it->second))
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
