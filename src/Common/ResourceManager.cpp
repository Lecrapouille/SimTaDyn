#include "ResourceManager.hpp"

// *************************************************************************************************
//
// *************************************************************************************************
void ResourceManager::add(IResource* resource)
{
  if (nullptr == resource)
    return ;

  Key id = resource->id();

  if (m_resources.find(id) != m_resources.end())
    return ;

  m_resources[id] = resource;
}

// *************************************************************************************************
//
// *************************************************************************************************
void ResourceManager::remove(const Key id)
{
  auto it = m_resources.find(id);
  if (it != m_resources.end())
    {
      m_resources.erase(it);
    }
}
