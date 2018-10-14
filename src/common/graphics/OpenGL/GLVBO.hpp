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

#ifndef GLVERTEX_BUFFER_HPP_
#  define GLVERTEX_BUFFER_HPP_

#  include "IGLObject.hpp"
#  include "PendingContainer.tpp"

// **************************************************************
//! \brief Buffer objects are OpenGL objects that store an array of
//! unformatted memory allocated by the OpenGL context (aka: the
//! GPU). These can be used to store vertex data, pixel data retrieved
//! from images or the framebuffer, and a variety of other things.
// **************************************************************
template<typename T>
class GLBuffer: public IGLObject<GLenum>
{
public:

  //! \brief Constructor with the object name
  GLBuffer(std::string const& name, const GLenum target, const GLenum usage = GL_DYNAMIC_DRAW)
    : IGLObject(name)
  {
    IGLObject::m_target = target;
    m_usage = usage;
  }

  //! \brief Constructor with the object name
  GLBuffer(const char *name, const GLenum target, const GLenum usage = GL_DYNAMIC_DRAW)
    : IGLObject(name)
  {
    IGLObject::m_target = target;
    m_usage = usage;
  }

  virtual ~GLBuffer() override { destroy(); }

  // private:

  virtual bool create() override
  {
    glCheck(glGenBuffers(1, &m_handle));
    LOGD("VBO '%s' create %d", name().c_str(), m_handle);
    return false;
  }

  virtual void release() override
  {
    LOGD("VBO '%s' release", name().c_str());
    glCheck(glDeleteBuffers(1, &m_handle));
  }

  virtual void activate() override
  {
    LOGD("VBO '%s' activate", name().c_str());
    glCheck(glBindBuffer(m_target, m_handle));
  }

  virtual void deactivate() override
  {
    LOGD("VBO '%s' deactivate", name().c_str());
    glCheck(glBindBuffer(m_target, 0));
  }

  virtual bool setup() override
  {
    const GLsizeiptr bytes = static_cast<GLsizeiptr>
      (qq.capacity() * sizeof (T));
    LOGD("VBO '%s' setup for %u elements %u", name().c_str(), qq.capacity(), bytes);

    glCheck(glBufferData(m_target, bytes, NULL, m_usage));

    return false;
  }

  virtual inline bool needUpdate() const override
  {
bool tt = qq.hasPendingData();
   LOGD("VBO needUpdate %d", tt);
    return tt;
  }

  virtual bool update() override
  {
    LOGD("VBO '%s' update", name().c_str());
    size_t pos_start, pos_end;
    qq.getPendingData(pos_start, pos_end);
    qq.clearPending();
LOGD("VBO::update: Pending %u %u", pos_start, pos_end);
    size_t offset = sizeof (T) * pos_start;
    size_t nbytes = sizeof (T) * (pos_end - pos_start + 1_z);
LOGD("VBO::update: off %u byte %u", offset, nbytes);
    glCheck(glBufferSubData(m_target,
                            static_cast<GLintptr>(offset),
                            static_cast<GLsizeiptr>(nbytes),
                            &qq.m_container[0]));
    for (size_t i = 0; i < qq.m_container.size(); ++i)
      {
        CPP_LOG(logger::Debug) << "VBO[" << (int) i << "] = " << qq.m_container[i] << "\n";
      }
    return false;
  }

  inline const T& operator[](size_t n) const
  {
    return qq.operator[](n);
  }

  inline T& operator[](size_t nth)
  {
    if (nth > qq.capacity())
      {
        reserve(nth);
      }
    return qq.operator[](nth);
  }

  //! \brief Append elements from a given vector.
  //! \param vect
  /*void append(std::vector<T> &vect)
  {
    append(&vect[0], vect.size());
  }

  void append(const T *array, const size_t size)
  {
    tryExpand();
    qq.append(array, size);
  }*/

private:

  void tryExpand() const
  {
    if (!IGLObject::needCreate())
      {
        throw std::out_of_range("Cannot change buffer size once loaded on GPU");
      }
  }

  //! if nth is lower than the current size. The nth is updated.
  //! Else (if nth is greater than the current size) two possible
  //! cases:
  //! -- Either the container size has not been reserved into the GPU
  //! (ie, if the method start() have not been called): the container
  //! size will change if In the case.
  //! -- the method start() have not been called, and so the container
  //! size cannot increase. A out_of_range exception is called.
  void reserve(size_t nth)
  {
    tryExpand();
    qq.resize(nth);
  }

private:
public: // FIXME
  PendingContainer<T> qq;// FIXME heritage ne marche pas
  GLenum m_usage;
};

// **************************************************************
//! \brief Buffer for vertex attribute data.
// **************************************************************
template<typename T>
class GLVertexBuffer: public GLBuffer<T>
{
public:

  //! \brief Constructor with the object name
  GLVertexBuffer(std::string const& name, const GLenum usage = GL_DYNAMIC_DRAW)
    : GLBuffer<T>(name, GL_ARRAY_BUFFER, usage)
  {
  }

  //! \brief Constructor with the object name
  GLVertexBuffer(const char *name, const GLenum usage = GL_DYNAMIC_DRAW)
    : GLBuffer<T>(name, GL_ARRAY_BUFFER, usage)
  {
  }
};

// **************************************************************
//! \brief Buffer for index data.
// **************************************************************
template<typename T>
class GLIndexBuffer: public GLBuffer<T>
{
public:

  //! \brief Constructor with the object name
  GLIndexBuffer(std::string const& name, const GLenum usage = GL_DYNAMIC_DRAW)
    : GLBuffer<T>(name, GL_ELEMENT_ARRAY_BUFFER, usage)
  {
  }

  //! \brief Constructor with the object name
  GLIndexBuffer(const char *name, const GLenum usage = GL_DYNAMIC_DRAW)
    : GLBuffer<T>(name, GL_ELEMENT_ARRAY_BUFFER, usage)
  {
  }
};

#endif /* GLVERTEX_BUFFER_HPP_ */
