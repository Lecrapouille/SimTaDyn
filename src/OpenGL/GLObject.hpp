#ifndef GLOBJECT_HPP_
#  define GLOBJECT_HPP_

// **************************************************************
//! \file This class get its inspiration from the code of the Glumpy
//! project (Python + Numpy + modern OpenGL: a fast, scalable and
//! beautiful scientific visualization). https://github.com/glumpy/glumpy
//!
//! Original Copyright from Glumpy project's Python code:
//! Copyright (c) 2009-2016 Nicolas P. Rougier. All rights reserved.
//! Distributed under the (new) BSD License.
// **************************************************************

#  include "Exception.hpp"
#  include "OpenGL.hpp"
#  include "Types.hpp"
#  include "ClassCounter.hpp"
#  include <assert.h>

//! This macro (from the library POCO) will declare a class
//! GLObjectException derived from simtadyn::Exception.
POCO_DECLARE_EXCEPTION(GLObjectException, simtadyn::Exception)

// **************************************************************
//! \brief
// **************************************************************
class GLObjectExceptionNotCreated: public GLObjectException
{
public:

  GLObjectExceptionNotCreated(std::string const& name)
  {
    m_msg = "OpenGL Object '" + name + "' has not been correctly created";
  }
};


// **************************************************************
//! \brief \class GLObject is an interface for managing a generic
//! OpenGL object.
// **************************************************************
class GLObject: private ClassCounter<GLObject>
{
public:

  //! \brief Empty constructor without name
  GLObject()
  {
  }

  //! \brief Constrcutor with the name of the object
  GLObject(std::string const& name)
    : m_name(name)
  {
  }

  //! \brief Constrcutor with the name of the object
  GLObject(const char *name)
    : m_name(name)
  {
    assert(nullptr != name);
  }

  //! \brief Return the GPU reference of the object.
  inline GLenum id() const
  {
    return m_handle;
  }

  //! \brief Return the GPU reference of the object.
  operator int()
  {
    return id();
  }

  //! \brief Return the GPU type of the object (i.e.
  //! GL_ARRAY_BUFFER, ...)
  inline GLenum target() const
  {
    return m_target;
  }

  //! \brief Activate the object on the GPU. Perform
  //! some pending operations (cretion setup, update)
  //! if needed.
  virtual void begin()
  {
    if (!SimTaDyn::glIsFunctional())
       return ;

    if (needCreate())
      {
        create();
        m_need_create = false;
      }

    if (needSetup())
      {
        setup();
        m_need_setup = false;
      }

    if (isActivable())
      {
        activate();
      }
    else if (m_throw_enable)
      {
        GLObjectExceptionNotCreated e(m_name);
        throw e;
      }

    if (needUpdate())
      {
        update();
        m_need_update = false;
      }
  }

  //! \brief Deactivate the object on the GPU.
  virtual void end()
  {
    deactivate();
  }

  //! \brief Delete the object from GPU memory.
  virtual void destroy()
  {
    if (!needDelete())
      return ;

    release();
    m_handle = 0U;
    m_need_setup = true;
    m_need_create = true;
    m_need_update = true;
  }

  //! \brief Track the
  static Key howMany()
  {
    return ClassCounter<GLObject>::howMany();
  }

public:

  std::string m_name;

protected:

  //! \brief Pure virtual. Allocate ressources on the GPU.
  virtual void create() = 0;
  //! \brief Pure virtual. Delete the object from GPU memory.
  virtual void release() = 0;
  //! \brief Pure virtual. Activate the object on the GPU.
  virtual void activate() = 0;
  //! \brief Pure virtual. Deactivate the object on the GPU.
  virtual void deactivate() = 0;
  //! \brief Pure virtual. Configure the object on the GPU.
  virtual void setup() = 0;
  //! \brief Pure virtual. Update the object on the GPU.
  virtual void update() = 0;

  //! \brief
  virtual inline bool needSetup() const
  {
    return m_need_setup;
  }

  //! \brief
  virtual inline bool needCreate() const
  {
    return m_need_create;
  }

  //! \brief
  virtual inline bool needUpdate() //FIXME const
  {
    return m_need_update;
  }

  //! \brief
  virtual inline bool needDelete() const
  {
    return false == m_need_create;
  }

  //! \brief
  virtual bool isActivable()
  {
    return m_handle > 0U;
  }

  //! \brief OpenGL object identifer GPU side.
  GLenum m_handle = 0U;
  //! \brief the type of object on the GPU.
  GLenum m_target = 0U;
  //! \brief
  bool m_need_setup = true;
  //! \brief
  bool m_need_create = true;
  //! \brief
  bool m_need_update = true;
  bool m_throw_enable = true;
};

#endif /* GLOBJECT_HPP_ */
