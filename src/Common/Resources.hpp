#ifndef RESOURCES_HPP_
#  define RESOURCES_HPP_

#  include "Types.hpp"

class IResource
{
  //friend class ResourceManager;

public:

  IResource(const Key id)
    : m_id(id), m_count(0)
  {
  }
  virtual ~IResource() = 0;
  inline Key id() const
  {
    return m_id;
  }

  inline void take()
  {
    ++m_count;
  }

  bool release();

private:

  IResource(IResource&);
  void operator =(IResource&);

  Key m_id;
  uint32_t m_count;
};

#endif /* RESOURCES_HPP_ */
