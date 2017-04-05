#ifndef GLOBJECT_HPP_
#  define GLOBJECT_HPP_

//! \file This class get its inspiration from the code of the Glumpy
//! project (Python + Numpy + modern OpenGL: a fast, scalable and
//! beautiful scientific visualization). https://github.com/glumpy/glumpy
//!
//! Original Copyright from Glumpy project's Python code:
//! Copyright (c) 2009-2016 Nicolas P. Rougier. All rights reserved.
//! Distributed under the (new) BSD License.

#  include "OpenGL.hpp"
#  include "Types.hpp"
#  include "ClassCounter.hpp"

//! \brief \class GLObject is an OpenGL reference
class GLObject: private ClassCounter<GLObject>
{
public:

  //! \brief Empty constructor. Give a ref
  GLObject()
  {
    m_cpu_id = howMany() - 1U;
  }

  virtual inline bool needSetup() const
  {
    return m_need_setup;
  }

  virtual inline bool needCreate() const
  {
    return m_need_create;
  }

  virtual inline bool needUpdate() //FIXME const
  {
    return m_need_update;
  }

  virtual inline bool needDelete() const
  {
    return false == m_need_create;
  }

  //! \brief Return the GPU reference of the object.
  inline GLenum gpuID() const
  {
    return m_handle;
  }

  //! \brief Return the CPU reference of the object.
  inline GLenum cpuID() const
  {
    return m_cpu_id;
  }

  //! \brief Return the GPU type of the object (GL_ARRAY_BUFFER, ...)
  inline GLenum target() const
  {
    return m_target;
  }

  //! \brief Activate the object on the GPU.
  virtual void begin()
  {
    if (needCreate())
      {
        create();
        m_need_create = false;
      }

    activate();

    if (needSetup())
      {
        setup();
        m_need_setup = false;
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

protected:

  virtual void create() = 0;
  //! \brief Delete the object from GPU memory
  virtual void release() = 0;
  //! \brief Activate the object on the GPU
  virtual void activate() = 0;
  //! \brief Deactivate the object on the GPU
  virtual void deactivate() = 0;
  //! \brief Configure the object on the GPU
  virtual void setup() = 0;
  //! \brief Update the object on the GPU
  virtual void update() = 0;

  GLenum m_handle = 0U;
  GLenum m_target = 0U;

private:

  bool m_need_setup = true;
  bool m_need_create = true;
  bool m_need_update = true;

  //! \brief Track the
  static Key howMany()
  {
    return ClassCounter<GLObject>::howMany();
  }

  Key m_cpu_id;

};

#endif /* GLOBJECT_HPP_ */
