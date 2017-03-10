#ifndef RESOURCE_MANAGER_HPP_
#  define RESOURCE_MANAGER_HPP_

#  include "Resources.hpp"
#  include "SimTaDynLogger.hpp"
#  include <map>
#  include <mutex>

template <class K> class IResource;

template <class K> class ResourceManager
{
public:

  typedef std::map<K, IResource<K>*> map_resources;
  typedef typename map_resources::iterator map_resources_it;

  ResourceManager()
  {
    LOGI("Creating the ResourceManager");
  }

  ~ResourceManager()
  {
    LOGI("Destroying the ResourceManager");
    if (!m_resources.empty())
    {
      auto it = m_resources.begin();
      auto end = m_resources.end();

      for (; it != end; ++it)
        {
          IResource<K> *resource = it->second;
          uint32_t n = resource->owners();
          if (0 == n)
            {
              resource->dispose();
            }
          else
            {
              CPP_LOG(logger::Error) << "  ==> The resource ID " << resource->id()
                                     << " is still aquired by " << n << " owners\n";
            }
        }
    }
  }


  //! \brief Insert an allocated reource in the list of resources.
  void insert(IResource<K>* resource)
  {
    if (nullptr == resource)
      return ;

    std::lock_guard<std::mutex> lock(m_mutex);

    if (m_resources.find(resource->id()) != m_resources.end())
      {
        LOGF("Trying to add a resource already present in the ResourceManager");
        return ;
      }

    resource->ownBy(this);
    m_resources[resource->id()] = resource;
  }

  //! \brief Get the resource referenced by its unique identifer from
  //! the list of reources.
  //! FIXME: not safe against delete.
  template <class T> T* acquire(K const& id) const
  {
    std::lock_guard<std::mutex> lock(m_mutex);

    auto it = m_resources.find(id);
    if (it == m_resources.end())
      {
        LOGF("Trying to take a resource already present in the ResourceManager");
        return nullptr;
      }

    it->second->acquire();
    return static_cast<T*>(it->second);
  }

  //! \brief The resource is no longer
  void dispose(K const& id)
  {
    std::lock_guard<std::mutex> lock(m_mutex);

    auto it = m_resources.find(id);
    if (it == m_resources.end())
      {
        LOGF("Trying to dispose from a resource not present in the ResourceManager");
        return ;
      }
    if (0 == it->second->dispose())
      {
        m_resources.erase(it);
      }
  }

  inline bool empty() const
  {
    return m_resources.empty();
  }

  inline map_resources_it begin()
  {
    return m_resources.begin();
  }

  inline map_resources_it end()
  {
    return m_resources.end();
  }

  inline map_resources_it find(const K& id)
  {
    return m_resources.find(id);
  }

protected:

  std::map<K, IResource<K>*> m_resources;
  std::mutex m_mutex;
};

#endif /* RESOURCE_MANAGER_HPP_ */
