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

#ifndef RESOURCE_MANAGER_TPP_
#  define RESOURCE_MANAGER_TPP_

#  include "Singleton.tpp"
#  include "Resource.hpp"
#  include "Logger.hpp"
#  include <map>
#  include <mutex>

template<class T>
class ResourceManager: public Singleton<ResourceManager<T>>
{
private:

  //------------------------------------------------------------------
  //! \brief Mandatory by design.
  //------------------------------------------------------------------
  friend class Singleton<ResourceManager<T>>;

  //------------------------------------------------------------------
  //! \brief Allow the resource to call methods like remove()
  //------------------------------------------------------------------
  // friend class IResource<T>;

  //------------------------------------------------------------------
  //! \brief Private because of Singleton.
  //------------------------------------------------------------------
  ResourceManager() { }

  //------------------------------------------------------------------
  //! \brief Private because of Singleton. Check if resources is still
  //! acquired which show a bug in the management of resources.
  //------------------------------------------------------------------
  virtual ~ResourceManager()
  {
    std::lock_guard<std::mutex> lock(m_mutex);

    LOGI("Destroying the ResourceManager");
    if (m_resources.empty())
      return ;

    for (auto it: m_resources)
      {
        IResource<T> *resource = it.second;
        uint32_t n = resource->owners();
        if (0U == n)
          {
            // Created but not used by someone
            // so can be released.
            resource->dispose();
          }
        else
          {
            CPP_LOG(logger::Warning)
              << "  ==> The resource "
              << resource->id()
              << " is still aquired by "
              << n << " owners\n";
          }
      }
  }

public:

  //------------------------------------------------------------------
  //! \brief Insert an allocated resource in the list of resources.
  //------------------------------------------------------------------
  bool add(IResource<T> *resource)
  {
    std::lock_guard<std::mutex> lock(m_mutex);

    if (nullptr == resource)
      {
        LOGF("Trying to add a resource with a NULL pointer");
        return false;
      }

    const T id = resource->id();
    if (m_resources.find(id) != m_resources.end())
      {
        CPP_LOG(logger::Warning)
          << "Trying to add a duplicated resource #"
          << id << ". This current action is ignored !\n";
        return false;
      }

    CPP_LOG(logger::Info)
      << "Added the resource #" << id
      << " to the ResourceManager\n";
    m_resources[id] = resource;
    return true;
  }

  //------------------------------------------------------------------
  //! \brief Call it when the resources is needed.
  //! \param id the unique identifier of the resource.
  //! \return the adress of the resource if it exists, else return
  //! nullptr.
  //------------------------------------------------------------------
  inline IResource<T> *acquire(T const& id)
  {
    std::lock_guard<std::mutex> lock(m_mutex);

    LOGI("Acquiring the resource #%u", id);
    auto it = m_resources.find(id);
    if (it != m_resources.end())
      {
        it->second->acquire();
        return it->second;
      }

    CPP_LOG(logger::Error)
      << "Trying to acquire a non-existent resource "
      << (uint32_t) id << ". This current action is ignored !\n";
    return nullptr;
  }

  //------------------------------------------------------------------
  //! Get the resource address in read only access.
  //------------------------------------------------------------------
  inline IResource<T> const *look(T const& id)
  {
    std::lock_guard<std::mutex> lock(m_mutex);

    auto it = m_resources.find(id);
    if (it != m_resources.end())
      {
        return it->second;
      }
    return nullptr;
  }

  //------------------------------------------------------------------
  //! \brief Call it when the resources is no longer needed. If someone
  //! is still using it, the resource is not destroyed. If nobody uses
  //! it, the resource is destroyed.
  //------------------------------------------------------------------
  void dispose(T const& id)
  {
    std::lock_guard<std::mutex> lock(m_mutex);

    auto it = m_resources.find(id);
    if (it != m_resources.end())
      {
        uint32_t owners = it->second->dispose();
        LOGI("Disposing of the resource #%u (%u owner(s))", id, owners);
        if (0U == owners)
          {
            // Note: compared to
            // http://loulou.developpez.com/tutoriels/moteur3d/ this
            // line was added because only the ResourceManager can
            // dispose of the resource.
            LOGI("Destroying the resource #%u", id);
            m_resources.erase(it);
          }
      }
    else
      {
        CPP_LOG(logger::Warning)
          << "Trying to dispose a non-existent resource #"
          << id << ". This current action is ignored !\n";
        return ;
      }
  }

  //------------------------------------------------------------------
  //! \brief Return the number of resources currently stored.
  //------------------------------------------------------------------
  inline uint32_t size()
  {
    std::lock_guard<std::mutex> lock(m_mutex);

    return m_resources.size();
  }

protected:

  //------------------------------------------------------------------
  //! \brief Remove an allocated resource in the list of resources.
  //------------------------------------------------------------------
  void remove(T const& id)
  {
    std::lock_guard<std::mutex> lock(m_mutex);

    auto it = m_resources.find(id);
    if (it != m_resources.end())
      {
        LOGI("Destroying the resource #%u", id);
        m_resources.erase(it);
      }
    else
      {
        CPP_LOG(logger::Warning)
          << "Trying to remove a non-existent resource "
          << id << ". This current action is ignored !\n";
      }
  }

  //------------------------------------------------------------------
  //! \brief the list of resources.
  //------------------------------------------------------------------
  std::map<T, IResource<T>*> m_resources;

  //------------------------------------------------------------------
  //! \brief Allow several threads to access to the manager.
  //-----------------------------------------------------------------
  std::mutex m_mutex;
};

#endif /* RESOURCE_MANAGER_TPP_ */
