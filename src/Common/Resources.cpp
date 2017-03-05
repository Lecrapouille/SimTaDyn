#include "Resources.hpp"

IResource::~IResource()
{
  //ResourceManager::instance().remove(m_id);
}

bool IResource::release()
{
  --m_count;
  if (0 == m_count)
    {
      delete this;
      return true;
    }
  return false;
}
