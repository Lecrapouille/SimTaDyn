#ifndef RESOURCE_MANAGER_HPP_
#  define RESOURCE_MANAGER_HPP_

#  include "Singleton.hpp"
#  include "Resources.hpp"
#  include <map>

class ResourceManager : public Singleton<ResourceManager>
{
  friend class Singleton<ResourceManager>;

public:
  template <class T> T* get(const Key id) const;
  void add(IResource* Resource);
  void remove(const Key id);

private:
  ResourceManager();
  ~ResourceManager();

  std::map<Key, IResource*> m_resources;
};

// *************************************************************************************************
//
// *************************************************************************************************
template <class T>
inline T* ResourceManager::get(const Key id) const
{
  auto it = m_resources.find(id);
  if (it != m_resources.end())
    {
      it->second->take();
      return static_cast<T*>(it->second);
    }
  return nullptr;
}

#endif /* RESOURCE_MANAGER_HPP_ */
