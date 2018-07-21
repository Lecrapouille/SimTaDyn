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

#ifndef RESOURCE_MANAGER_TPP_
#  define RESOURCE_MANAGER_TPP_

#  include "Singleton.tpp"
#  include "Resource.hpp"
#  include "Logger.hpp"
#  include <map>
#  include <mutex>

// **************************************************************
//! \brief Hold resources. Needed for meta-programming
// **************************************************************
template <class T>
class ResourceHolder
{
public:

  ResourceHolder() {}
  virtual ~ResourceHolder()
  {
    if (m_resources.empty())
      return ;

    for (auto& it: m_resources)
      {
        // Note: -1 because shared_ptr has 1 reference made by its
        // own container.
        auto n = it.second.use_count() - 1;
        if (n > 0u)
        {
          CPP_LOG(logger::Warning)
            << "Destroying the ResourceManager but its resource #"
            << it.first << " is still used by " << n << " owners\n";
        }
      }
  }

public:

  std::map<T, ResourcePtr> m_resources;
};

// **************************************************************
//! \brief
// **************************************************************
template <class R, class T>
class ResourceManager
  : public Singleton<ResourceManager<R, T>>,
    public ResourceHolder<T>
{
private:

  //------------------------------------------------------------------
  //! \brief Mandatory by design.
  //------------------------------------------------------------------
  friend class Singleton<ResourceManager<R, T>>;

public:

  //------------------------------------------------------------------
  //! \brief Private because of Singleton.
  //------------------------------------------------------------------
  ResourceManager() { }

  //------------------------------------------------------------------
  //! \brief Private because of Singleton. Check if resources is still
  //! acquired which show a bug in the management of resources.
  //------------------------------------------------------------------
  ~ResourceManager() { }

public:

  std::shared_ptr<R>
  create(const T& id, const bool force = true)
  {
    ResourcePtr r = Resource::create<R>();
    if (true == add(id, r, force))
      {
        return std::static_pointer_cast<R>(r);
      }
    return nullptr;
  }

  //------------------------------------------------------------------
  //! \brief Insert an allocated resource in the list of resources.
  //------------------------------------------------------------------
  bool add(const T& id, ResourcePtr& resource, const bool force = true)
  {
    CPP_LOG(logger::Info) << "Adding the resource #" << id << "\n";
    std::lock_guard<std::mutex> lock(m_mutex);

    const auto& it = ResourceHolder<T>::m_resources.find(id);
    if (it != ResourceHolder<T>::m_resources.end())
      {
        CPP_LOG(logger::Info) << "Replacing the resource #" << id
                              << " owned by " << (it->second.use_count() - 1)
                              << "\n";
        if (!force)
          return false;
      }

    ResourceHolder<T>::m_resources[id] = resource;
    return true;
  }

  //------------------------------------------------------------------
  //! Get the resource address in read only access.
  //------------------------------------------------------------------
  inline bool exist(const T& id) const
  {
    std::lock_guard<std::mutex> lock(m_mutex);

    const auto& it = ResourceHolder<T>::m_resources.find(id);
    if (it != ResourceHolder<T>::m_resources.end())
      {
        return true;
      }
    return false;
  }

  //------------------------------------------------------------------
  //! \brief Call it when the resources is needed.
  //! \param id the unique identifier of the resource.
  //! \return the adress of the resource if it exists, else return
  //! nullptr.
  //------------------------------------------------------------------
  inline std::shared_ptr<R> acquire(const T& id)
  {
    CPP_LOG(logger::Info) << "Acquiring the resource #" << id << "\n";
    std::lock_guard<std::mutex> lock(m_mutex);

    const auto& it = ResourceHolder<T>::m_resources.find(id);
    if (it != ResourceHolder<T>::m_resources.end())
      {
        return std::static_pointer_cast<R>(it->second);
      }
    else
      {
        CPP_LOG(logger::Warning)
          << "Trying to acquire a non-existent resource "
          << id << ". This current action is ignored !\n";
        return nullptr;
      }
  }

  //------------------------------------------------------------------
  //! \brief Call it when the resources is no longer needed. If someone
  //! is still using it, the resource is not destroyed. If nobody uses
  //! it, the resource is destroyed.
  //------------------------------------------------------------------
  void remove(const T& id, const bool force = true)
  {
    CPP_LOG(logger::Info) << "Disposing of the resource #" << id << "\n";
    std::lock_guard<std::mutex> lock(m_mutex);

    const auto& it = ResourceHolder<T>::m_resources.find(id);
    if (it != ResourceHolder<T>::m_resources.end())
      {
        uint32_t owners = it->second.use_count();
        if (owners > 1u)
          {
            CPP_LOG(logger::Warning)
              << "Trying to dispose of the resource #"
              << id << " currently used by " << (owners - 1)
              << " owners !" << (force ? "Force removing" : "Action ignored")
              << ".\n";

            if (!force)
              return ;
          }

        ResourceHolder<T>::m_resources.erase(it);
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

    return ResourceHolder<T>::m_resources.size();
  }

  void debug() const
  {
    std::cout << "ResourceManager::debug():" << std::endl;
    for (const auto& it: ResourceHolder<T>::m_resources)
      {
        std::cout << it.first << ": shared by "
          << (it.second.use_count() - 1)
          << " owners" << std::endl;
      }
  }

  //------------------------------------------------------------------
  //! \brief Allow several threads to access to the manager.
  //-----------------------------------------------------------------
  mutable std::mutex m_mutex;
};

#endif /* RESOURCE_MANAGER_TPP_ */
