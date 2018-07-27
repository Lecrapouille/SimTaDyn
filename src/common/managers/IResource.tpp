// -*- c++ -*- Coloration Syntaxique pour Emacs

#ifndef RESOURCES_TPP_
#  define RESOURCES_TPP_

//#  include "ResourceManager.tpp"
#  include "Logger.hpp"
#  include <atomic>

// Inspired by: http://loulou.developpez.com/tutoriels/moteur3d/
// document: "Part 4: Gestion des ressources" and its code "YesEngine"
// but this current code is different than the original code,
// as well as its behavior.

// **************************************************************
//! \brief Forward declaration
// **************************************************************
template <class T>
class ResourceManager;

// **************************************************************
//! \brief An application produces and consums a big amount of
//! resources. A resource can be a texture, a sound, a GIS maps, a
//! Forh script, ... Recurent problems are a resource can be mistakly
//! created in several occurences or free by a process while used by
//! another. To avoid this problem, this class count the number of
//! owners holding it and auto delete itself when nobody uses it.
//! This class is only managed by the ResourceManager (almost methods
//! are protected) which stores all resources and make an interface
//! for other process which needs resources.
// **************************************************************
template <class T>
class IResource
{
private:

  //------------------------------------------------------------------
  //! \brief Only the ResourceManager can uses methods acquire() and
  //! dispose().
  //------------------------------------------------------------------
  friend class ResourceManager<T>;

public:

  //------------------------------------------------------------------
  //! \brief Empty constructor. Set to 1 the number of owners. Derived
  //! class shall init m_id.
  //------------------------------------------------------------------
  IResource(T const& id)
    : m_owners(1U), m_id(id)
  {
  }

  //------------------------------------------------------------------
  //! \brief Virtual destructor to forbide instanciating this class
  //! and force to use inheritance.
  //------------------------------------------------------------------
  virtual ~IResource()
  {
    // Uncomment this line for Resources not managed directly by the
    // resource manager. In this case: ask the ResourceManager to
    // remove it. Note: compared to
    // http://loulou.developpez.com/tutoriels/moteur3d/ a process cannot
    // call dispose, only the ResourceManager can do it.
    //ResourceManager<T>::instance().remove(IResource<T>::id());
  }

  //------------------------------------------------------------------
  //! \brief Return the unique identifier. Derived class shall init
  //! m_id.
  //------------------------------------------------------------------
  virtual inline T id() const
  {
    return m_id;
  }

  //------------------------------------------------------------------
  //! \brief Return the number of owners holding this resource.
  //------------------------------------------------------------------
  inline uint32_t owners()
  {
    return m_owners - 1U;
  }

protected:

  //------------------------------------------------------------------
  //! \brief Call this function when a process needs the resource.
  // TODO: add a list with owners names for debug.
  //------------------------------------------------------------------
  inline void acquire()
  {
    ++m_owners;
  }

  //------------------------------------------------------------------
  //! \brief Call this function when you no longer need the resource.
  //! When nobody is using the resource it will be automaticly
  //! destroyed.
  //! \return the number of available references of the resource.
  //------------------------------------------------------------------
  uint32_t dispose()
  {
    uint32_t count = --m_owners;

    if (0 == count)
      delete this;

    return count;
  }

private:

  //------------------------------------------------------------------
  //! \brief Forbid the constructor by copy.
  //------------------------------------------------------------------
  IResource(IResource<T>&);

  //------------------------------------------------------------------
  //! \brief Forbid the copy.
  //------------------------------------------------------------------
  void operator=(IResource<T>&);

  //------------------------------------------------------------------
  //! \brief Forbid deleting this class. Call the method dispose()
  //------------------------------------------------------------------
  //FIXME void operator delete(void *) {}

protected:

  //------------------------------------------------------------------
  //! \brief Count the number of references (owners) of this resource.
  //------------------------------------------------------------------
  std::atomic<uint32_t> m_owners;

  //------------------------------------------------------------------
  //! The unique identifer for the resource.
  //------------------------------------------------------------------
  T m_id;
};

#endif /* RESOURCES_TPP_ */
