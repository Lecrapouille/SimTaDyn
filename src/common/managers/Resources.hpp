#ifndef RESOURCES_HPP_
#  define RESOURCES_HPP_

#  include "ResourceManager.hpp"
#  include <cstdint>

template <class K> class ResourceManager;

template <class K> class IResource
{
  friend class ResourceManager<K>;

public:

  IResource() { };
  virtual ~IResource() { };  // FIXME: private to avoid delete

  //! \brief Return the unique identifier. Need to be implemented
  //! by the inherited class.
  virtual K id() const = 0;

  //! \brief Call this function when you need the resource.
  inline void acquire()
  {
    ++m_count;
  }

  //! \brief Call this function when you no longer need the resource.
  //! When nobody is using the resource it will be automaticly
  //! destroyed.
  //! \return the number of available references of the resource.
  uint32_t dispose()
  {
    if (m_count > 0)
      --m_count;

    uint32_t count = m_count;

    if (0 == m_count)
      delete this;

    return count;
  }

  //! \brief Return the number of owner containing this resource.
  inline uint32_t owners()
  {
    return m_count;
  }

private:

  IResource(IResource<K>&);
  void operator =(IResource<K>&);

  inline void ownBy(ResourceManager<K>* owner)
  {
    m_owner = owner;
  }

  uint32_t m_count = 0;
  ResourceManager<K> *m_owner = nullptr;
};

#endif /* RESOURCES_HPP_ */
