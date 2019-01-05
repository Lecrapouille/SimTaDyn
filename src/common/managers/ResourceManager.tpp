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

#ifndef RESOURCE_MANAGER_TPP_
#  define RESOURCE_MANAGER_TPP_

#  include "NonCppStd.hpp"
#  include "Singleton.tpp"
#  include "ManagerException.hpp"
#  include "Logger.hpp"
#  include <map>
#  include <mutex>
#  include <memory>
#  include <atomic>
#  include <utility>

// ***********************************************************************************************
//! \brief insertion strategy for the ResourceManager. When inserting
//! a new resource in the ResourceManager container, the slot is
//! supposed to be empty (meaning the id is not already used). If this
//! is not the case we have to decide of the behavior.
// ***********************************************************************************************
namespace resource
{
  enum class Strategy
    {
      //! \brief An exception will occur.
      ThrowException,

      //! \brief The older resource will not be smashed by the new
      //! one. Therefore the new one is not inserted.
      UseOlder,

      //! \brief The older resource will be smashed by the new
      //! one.
      Replace,

      //! \brief Do nothing just return nullptr
      ReturnNull
    };
}

// ***********************************************************************************************
//! \brief A class that is responsible of the life management of
//! resources like images, fonts or music ... This class stores
//! resources internally and let clients to access them. Resources are
//! automaticaly release when no longer used. This class is thrread
//! safe.
// ***********************************************************************************************
template <class I, class R>
class ResourceManager
  : public Singleton<ResourceManager<I, R>>
{
private:

  //------------------------------------------------------------------
  //! \brief Mandatory by the Singleton design.
  //------------------------------------------------------------------
  friend class Singleton<ResourceManager<I, R>>;

public:

  //------------------------------------------------------------------
  //! \brief Private because of Singleton.
  //------------------------------------------------------------------
  ResourceManager() { }

  //------------------------------------------------------------------
  //! \brief Private because of Singleton. Check if resources is still
  //! acquired which show a bug in the management of resources.
  //------------------------------------------------------------------
  ~ResourceManager()
  {
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

  //------------------------------------------------------------------
  //! \brief insertion strategy:
  //------------------------------------------------------------------
  inline void strategy(resource::Strategy strategy)
  {
    m_strategy = strategy;
  }

  inline resource::Strategy strategy() const
  {
    return m_strategy;
  }

  //------------------------------------------------------------------
  //! \brief Create a resource, store it internaly with a given
  //! strategy and return its reference.
  //------------------------------------------------------------------
  template<typename... Args>
  inline std::shared_ptr<R> create(I const& id, resource::Strategy strategy, Args&... args)
  {
    std::lock_guard<std::mutex> lock(m_mutex);

    return insert(id, strategy, args...);
  }

  //------------------------------------------------------------------
  //! \brief Create a resource, store it internaly with the current
  //! defined strategy and return its reference.
  //------------------------------------------------------------------
  /*template<typename... Args>
  inline std::shared_ptr<R> create(I const& id, Args&... args)
  {
    LOGI("Strategy %d", (int) strategy());
    return create(id, strategy(), std::forward<Args>(args)...);
    }*/

  //------------------------------------------------------------------
  //! \brief Call it when the resources is no longer needed. If someone
  //! is still using it, the resource is not destroyed. If nobody uses
  //! it, the resource is destroyed.
  //------------------------------------------------------------------
  void remove(I const& id)
  {
    std::lock_guard<std::mutex> lock(m_mutex);

    auto it = m_resources.find(id);
    if (it != m_resources.end())
      {
        size_t owners = it->second.use_count();
        if (owners > 1_z)
          {
            const bool force = true;
            CPP_LOG(logger::Warning)
              << "Trying to dispose of the resource #"
              << id << " currently used by " << (owners - 1)
              << " owners !" << (force ? "Force removing" : "Action ignored")
              << ".\n";

            if (!force)
              return ;
          }

        m_resources.erase(it);
      }
    else
      {
        CPP_LOG(logger::Warning)
          << "Failed to release resource '" << id
          << "' not currently stored in ResourceManager\n";
      }
  }

  //------------------------------------------------------------------
  //! \brief Call it when the resources is needed.
  //! \param id the unique identifier of the resource.
  //! \return the resource if it exists else return and indefined data.
  //! \note this method does not trig exception. the resource id shall
  //! exist. If you want securized method call instead acquire().
  //------------------------------------------------------------------
  R& operator[](I const& id)
  {
    std::lock_guard<std::mutex> lock(m_mutex);

    auto it = m_resources.find(id);
    return *(it->second);
  }

  //------------------------------------------------------------------
  //! \brief Call it when the resources is needed.
  //! \param id the unique identifier of the resource.
  //! \return the resource if it exists, else throw an exception.
  //------------------------------------------------------------------
  /*  R& acquireRef(I const& id)
  {
    std::lock_guard<std::mutex> lock(m_mutex);

    auto it = m_resources.find(id);
    if (it == m_resources.end())
      throw ResourceManager("Failed to access resource, ID not currently stored in ResourceManager");

    return *(it->second);
    }*/

  std::shared_ptr<R> acquire(I const& id)
  {
    std::lock_guard<std::mutex> lock(m_mutex);

    auto it = m_resources.find(id);
    if (it == m_resources.end())
      return nullptr;

    return it->second;
  }

  //------------------------------------------------------------------
  //! \brief Rename the
  //------------------------------------------------------------------
  std::shared_ptr<R> rename(I const& oldKey, I const& newKey, resource::Strategy strategy)
  {
    std::lock_guard<std::mutex> lock(m_mutex);

    auto old_it = m_resources.find(oldKey);
    if (old_it != m_resources.end())
      {
        auto new_it = m_resources.find(newKey);
        if (new_it == m_resources.end())
          {
            std::swap(new_it->second, old_it->second);
            m_resources.erase(old_it);
            return new_it->second;
          }
        else
          {
            switch (strategy)
              {
              case resource::Strategy::ThrowException:
                LOGD("resource::Strategy::ThrowException");
                throw ResourceManagerException("Failed to load resource, ID already stored in ResourceManager");
              case resource::Strategy::Replace:
                LOGD("resource::Strategy::Replace");
                std::swap(new_it->second, old_it->second);
                m_resources.erase(old_it);
                return new_it->second;
              case resource::Strategy::UseOlder:
                LOGD("resource::Strategy::UseOlder");
                m_resources.erase(old_it);
                return new_it->second;
              case resource::Strategy::ReturnNull:
                return nullptr;
              }
          }
      }
    else
      {
        CPP_LOG(logger::Warning) << "Failed renaming '" << oldKey << "'. This resource does not exist\n";
        return nullptr;
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

  //------------------------------------------------------------------
  //! \brief Pretty print the content of the class.
  //------------------------------------------------------------------
  void debug() const
  {
    std::lock_guard<std::mutex> lock(m_mutex);

    std::cout << "ResourceManager::debug():" << std::endl;
    for (const auto& it: m_resources)
      {
        std::cout << it.first << std::endl;
      }
  }

private:

  //------------------------------------------------------------------
  //! \brief Insert an allocated resource in the list of resources.
  //------------------------------------------------------------------
  template<typename... Args>
  std::shared_ptr<R> insert(I const& id, resource::Strategy strategy, Args&... args)
  {
    std::shared_ptr<R> resource = nullptr;

    auto it = m_resources.find(id);
    if (it == m_resources.end())
      {
        LOGD("New resource created");
        resource = std::make_unique<R>(args...);
        m_resources[id] = resource;
      }
    else
      {
        CPP_LOG(logger::Info)
          << "The resource '" << id << "' is already stored in ResourceManager\n";

        switch (strategy)
          {
          case resource::Strategy::ThrowException:
            LOGD("resource::Strategy::ThrowException");
            throw ResourceManagerException("Failed to load resource, ID already stored in ResourceManager");
            break;
          case resource::Strategy::Replace:
            LOGD("resource::Strategy::Replace");
            resource = std::make_unique<R>();
            it->second = resource;
            break;
          case resource::Strategy::UseOlder:
            LOGD("resource::Strategy::UseOlder");
            resource = it->second;
            break;
          case resource::Strategy::ReturnNull:
            LOGD("resource::Strategy::ReturnNull");
            resource = nullptr;
            break;
          }
      }
    return resource;
  }

private:

  std::atomic<resource::Strategy> m_strategy{resource::Strategy::ThrowException};
  std::map<I, std::shared_ptr<R>> m_resources;
  mutable std::mutex m_mutex;
};

#endif /* RESOURCE_MANAGER_TPP_ */
