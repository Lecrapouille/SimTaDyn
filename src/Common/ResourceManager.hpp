#ifndef RESOURCE_MANAGER_HPP_
#  define RESOURCE_MANAGER_HPP_

#  include "Singleton.hpp"
#  include "Resources.hpp"
#  include <map>

template <class K> class IResourceManager
{
public:

  //virtual ~IResourceManager() = 0;

  typedef std::map<K, IResource*> map_resources;
  typedef typename map_resources::iterator map_resources_it;

  template <class T> T* get(K const& id) const
  {
    auto it = m_resources.find(id);
    if (it != m_resources.end())
      {
        it->second->take();
        return static_cast<T*>(it->second);
      }
    return nullptr;
  }

  void add(K const& id, IResource* resource)
  {
    if (nullptr == resource)
      return ;

    if (m_resources.find(id) != m_resources.end())
      return ;

    m_resources[id] = resource;
  }


  void remove(K const& id)
  {
    auto it = m_resources.find(id);
    if (it != m_resources.end())
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

  std::map<K, IResource*> m_resources;
};


// *************************************************************************************************
//
// *************************************************************************************************
template <class K> class SResourceManager
  : public IResourceManager<K>, Singleton<SResourceManager<K>>
{
private:

  SResourceManager();
  ~SResourceManager();
};

// *************************************************************************************************
//
// *************************************************************************************************
template <class K> class ResourceManager : public IResourceManager<K>
{
public:

  ResourceManager() {}
  virtual ~ResourceManager() {}
};

#endif /* RESOURCE_MANAGER_HPP_ */
