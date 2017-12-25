//=====================================================================
// SimTaDyn: A GIS in a spreadsheet.
// Copyright 2017 Quentin Quadrat <lecrapouille@gmail.com>
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

#ifndef GLOBJECT_HPP_
#  define GLOBJECT_HPP_

// **************************************************************
//! \file GLObject.hpp
//! This class get its inspiration from the Glumpy project
//! code (Python + Numpy + modern OpenGL: a fast, scalable and
//! beautiful scientific visualization): https://github.com/glumpy/glumpy
//!
//! Original Copyright from Glumpy project's Python code:
//! Copyright (c) 2009-2016 Nicolas P. Rougier. All rights reserved.
//! Distributed under the (new) BSD License.
// **************************************************************

#  include "OpenGL.hpp"
#  include "ClassCounter.tpp"
#  include "Logger.hpp"
#  include <assert.h>
#  include <cstdint>

// **************************************************************
//! \class GLObject GLObject.hpp
//! \brief GLObject is an interface for managing a generic OpenGL
//! object.
// **************************************************************
class GLObject
{
public:

  //! \brief Empty constructor with no name.
  //! Note: name is public and is used for debug purpose.
  GLObject()
  {
    //LOGI("New GLObject with no name");
  }

  //! \brief Constructor with the name of the object.
  //! Note: name is public and is used for debug purpose.
  GLObject(std::string const& name)
    : m_name(name)
  {
    //LOGI("New GLObject with name '%s'", name.c_str());
  }

  //! \brief Constuctor with the name of the object.
  //! Note: name is public and is used for debug purpose.
  GLObject(const char *name)
    : m_name(name)
  {
    assert(nullptr != name);
    //LOGI("New GLObject with name '%s'", name);
  }

  //! \brief Return the GPU reference of the object.
  operator int()
  {
    return m_handle;
  }

  //! \brief Activate the object on the GPU. Perform
  //! some pending operations (cretion setup, update)
  //! if needed.
  virtual void start()
  {
    //LOGI("Starting GLObject named '%s'", m_name.c_str());

    //if (!SimTaDyn::glIsFunctional())
    //   return ;

    if (needCreate())
      {
        m_need_create = create();
      }

    if (needSetup())
      {
        m_need_setup = setup();
      }

    if (isValid())
      {
        activate();
      }
    else if (m_throw_enable)
      {
        std::string msg("OpenGL Object '");
        msg += m_name;
        msg += "' has not been correctly created";
        OpenGLException e(msg);
        throw e;
      }

    if (needUpdate())
      {
        m_need_update = update();
      }
  }

  //! \brief Deactivate the object on the GPU.
  virtual void stop()
  {
    //LOGI("Stoping GLObject named '%s'", m_name.c_str());

    //if (!SimTaDyn::glIsFunctional())
    //   return ;

    deactivate();
  }

  //! \brief
  virtual inline bool isValid() const
  {
    //LOGI("GLObject named '%s' is valid ? %u", m_name.c_str(), m_handle > 0U);
    return m_handle > 0U;
  }

protected:

  //! \brief Delete the object from GPU and CPU memory.
  virtual void destroy()
  {
    //LOGI("Destroying GLObject named '%s'", m_name.c_str());
    if (!canBeReleased())
      return ;

    if (!isValid())
      return ;

    deactivate();
    release();
    m_handle = 0U;
    m_need_setup = true;
    m_need_create = true;
    m_need_update = true;
  }

  //! \brief Pure virtual. Allocate ressources on the GPU.
  virtual bool create() = 0;
  //! \brief Pure virtual. Delete the object from GPU memory.
  virtual void release() = 0;
  //! \brief Pure virtual. Activate the object on the GPU.
  virtual void activate() = 0;
  //! \brief Pure virtual. Deactivate the object on the GPU.
  virtual void deactivate() = 0;
  //! \brief Pure virtual. Configure the object on the GPU.
  virtual bool setup() = 0;
  //! \brief Pure virtual. Update the object on the GPU.
  virtual bool update() = 0;

  //! \brief
  virtual inline bool needSetup() const
  {
    //LOGI("GLObject named '%s' need setup ? %u", m_name.c_str(), m_need_setup);
    return m_need_setup;
  }

  //! \brief
  virtual inline bool needCreate() const
  {
    //LOGI("GLObject named '%s' need be created ? %u", m_name.c_str(), m_need_create);
    return m_need_create;
  }

  //! \brief
  virtual inline bool needUpdate() const
  {
    //LOGI("GLObject named '%s' need be updated ? %u", m_name.c_str(), m_need_update);
    return m_need_update;
  }

  //! \brief
  virtual inline bool canBeReleased() const
  {
    //LOGI("GLObject named '%s' can be released ? %u", m_name.c_str(), false == m_need_create);
    return false == m_need_create;
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

public:

  //! Object name for debug purpose.
  std::string m_name;
};

#endif /* GLOBJECT_HPP_ */
