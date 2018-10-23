//=====================================================================
// SimTaDyn: A GIS in a spreadsheet.
// Copyright 2018 Quentin Quadrat <lecrapouille@gmail.com>
//
// This file is part of SimTaDyn.
//
// SimTaDyn is free software: you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with GNU Emacs.  If not, see <http://www.gnu.org/licenses/>.
//=====================================================================

#ifndef IGLOBJECT_HPP_
#  define IGLOBJECT_HPP_

// **************************************************************
//! \file IGLObject.hpp
//! This class get its inspiration from the Glumpy project
//! code (Python + Numpy + modern OpenGL: a fast, scalable and
//! beautiful scientific visualization): https://github.com/glumpy/glumpy
//!
//! Original Copyright from Glumpy project's Python code:
//! Copyright (c) 2009-2016 Nicolas P. Rougier. All rights reserved.
//! Distributed under the (new) BSD License.
// **************************************************************

#  include "ClassCounter.tpp"
#  include "Logger.hpp"
#  include "NonCppStd.hpp"
#  include <assert.h>
#  include <cstdint>

// **************************************************************
//! \class IGLObject IGLObject.hpp
//! \brief IGLObject is an interface for managing a generic OpenGL
//! object.
// **************************************************************
template<class T>
class IGLObject//: protected InstanceCounter<IGLObject<T>>
{
public:

  IGLObject()
  {
    init();
  }

  //! \brief Constructor with the name of the object.
  //! Note: name is public and is used for debug purpose.
  IGLObject(std::string const& name)
    : m_name(name)
  {
    init();
  }

  virtual ~IGLObject()
  {
    destroy();
  }

  //! \brief Return the GPU reference of the object.
  inline T gpuID() const
  {
    return m_handle;
  }

  inline std::string& name()
  {
    return m_name;
  }

  inline GLenum target() const
  {
    return m_target;
  }

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
  virtual inline bool needUpdate() const
  {
    return m_need_update;
  }

  //! \brief
  virtual inline bool canBeReleased() const
  {
    return false == m_need_create;
  }

  //! \brief Activate the object on the GPU. Perform
  //! some pending operations (cretion setup, update)
  //! if needed.
  void begin()
  {
    /* FIXME if (unlikely(!opengl::hasCreatedContext()))
      {
        LOGD("O::begin perdu");
        return ;
        }*/

    if (likely(isValid()))
      {
        activate();
        if (needUpdate())
          {
            m_need_update = update();
          }
      }
    else
      {
        if (likely(needCreate()))
          {
            m_need_create = create();
          }

        if (likely(isValid()))
          {
            activate();

            if (unlikely(needSetup()))
              {
                m_need_setup = setup();
                if (unlikely(m_need_setup))
                  return ;
              }

            if (likely(needUpdate()))
              {
                m_need_update = update();
              }
          }
      }
  }

  //! \brief Deactivate the object on the GPU.
  inline void end()
  {
    /*if (unlikely(!opengl::hasCreatedContext()))
      return ;*/

    deactivate();
  }

  //! \brief Delete the object from GPU memory.
  void destroy()
  {
    if (isValid())
      {
        deactivate();
        release();
      }
    init();
  }

  //! \brief Valid object is an OpenGL has created with success.
  inline bool isValid() const
  {
    return m_handle > handle_reset_value();
  }

protected:

  //! \brief
  inline void redoSetup()
  {
    m_need_setup = true;
  }

    //! \brief
  inline void forceUpdate()
  {
    m_need_update = true;
  }

private:

  inline T handle_reset_value() const;

  void init()
  {
    m_handle = handle_reset_value();
    m_target = 0U;
    m_need_setup = true;
    m_need_create = true;
    m_need_update = false;
  }

  //! \brief Pure virtual. Configure the object on the GPU.
  virtual bool setup() = 0;

  //! \brief Pure virtual. Allocate ressources on the GPU.
  virtual bool create() = 0;

  //! \brief Pure virtual. Activate the object on the GPU.
  virtual void activate() = 0;

  //! \brief Pure virtual. Deactivate the object on the GPU.
  virtual void deactivate() = 0;

  //! \brief Pure virtual. Delete the object from GPU memory.
  virtual void release() = 0;

  //! \brief Pure virtual. Update the object on the GPU.
  virtual bool update() = 0;

private:

  //! Object name for debug purpose.
  std::string m_name;
  //! \brief
  bool m_need_setup;
  //! \brief
  bool m_need_create;
  //! \brief
  bool m_need_update;

protected:

  //! \brief OpenGL object identifer GPU side.
  T m_handle;
  //! \brief the type of object on the GPU.
  GLenum m_target;
};

template<>
inline GLenum IGLObject<GLenum>::handle_reset_value() const { return 0u; }

template<>
inline GLint IGLObject<GLint>::handle_reset_value() const { return -1; }

#endif /* IGLOBJECT_HPP_ */
