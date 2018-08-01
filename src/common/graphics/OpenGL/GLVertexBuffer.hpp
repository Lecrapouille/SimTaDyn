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

#  include "PendingData.hpp"
#  include "GLAttrib.hpp"
#  include <vector>

// **************************************************************
//! \brief Buffer objects are OpenGL objects that store an array of
//! unformatted memory allocated by the OpenGL context (aka: the
//! GPU). These can be used to store vertex data, pixel data retrieved
//! from images or the framebuffer, and a variety of other things.
// **************************************************************
template<typename T>
class GLBuffer: public GLObject, protected PendingData
{
public:

  //! \brief Constructor with the object name
  GLBuffer(std::string const& name, const GLenum target, const GLenum usage = GL_DYNAMIC_DRAW)
    : GLObject(name), PendingData(), m_attrib(name)
  {
    GLObject::m_target = target;
    m_usage = usage;
  }

  //! \brief Constructor with the object name
  GLBuffer(const char *name, const GLenum target, const GLenum usage = GL_DYNAMIC_DRAW)
    : GLObject(name), PendingData(), m_attrib(name)
  {
    GLObject::m_target = target;
    m_usage = usage;
  }

  //! \brief Destructor: release data from the GPU and CPU memory.
  virtual ~GLBuffer()
  {
    GLObject::destroy();
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
    if (GLObject::m_need_create)
      {
        uint32_t s = m_data.capacity();
        m_data.reserve(nth - s + 1U);
        while (s--)
          {
            m_data.push_back(m_data[0]);
          }
      }
    else
      {
        const uint32_t s = m_data.capacity();
        std::string msg = "GLBuffer " + m_name + "["
          + std::to_string(nth) + "], max elt: "
          + std::to_string(s);
        throw std::out_of_range(msg.c_str());
      }
  }

  //! \brief Insert or change element at the given position.
  inline T& operator[](size_t nth)
  {
    if (nth > m_data.capacity())
      {
        reserve(nth);
      }
    PendingData::tagAsPending(nth);
    return m_data[nth];
  }

  inline const T& operator[](size_t nth) const
  {
    return m_data[nth];
  }

  inline void push_back(T const &e)
  {
    m_data.push_back(e);
    PendingData::tagAsPending(m_data.size() - 1U);
  }

  inline void pop_back()
  {
    m_data.pop_back(); // FIXME --Pending::last ssi last == size
  }

  inline void push_front(T const &e)
  {
    auto it = m_data.begin();
    m_data.insert(it, e);
    PendingData::tagAsPending(0U, m_data.size() - 1U);
  }

  //! \brief Return the number of elements a block can store
  inline uint32_t size() const
  {
    return m_data.size();
  }

  inline uint32_t capacity() const
  {
    return m_data.capacity();
  }

  //! \brief Append elements from a given vector.
  //! \param vect
  void add(std::vector<T> &vect)
  {
    uint32_t size = vect.size();
    if (size > m_data.capacity())
      {
        reserve(size);
      }

    uint32_t offset = m_data.size();
    PendingData::tagAsPending(offset, offset + size);

    while (size--)
      {
        m_data.push_back(vect[size]);
      }
  }

  void add(const T *array, const uint32_t size)
  {
    if (size > m_data.capacity())
      {
        reserve(size);
      }

    uint32_t offset = m_data.size();
    PendingData::tagAsPending(offset, offset + size);

    for (uint32_t s = 0U; s < size; ++s)
      {
        m_data.push_back(array[s]);
      }
  }

  virtual void setup(GLShader& program, const GLint size, const GLenum type, const GLboolean normalized = GL_FALSE)
  {
    m_attrib.setup(program, size, type, normalized);
  }

  virtual void start() override
  {
    GLObject::start();
    m_attrib.start();
  }

  virtual void stop() override
  {
    GLObject::stop();
    m_attrib.stop();
  }

protected:

  virtual bool create() override
  {
    const uint32_t bytes = m_data.capacity() * sizeof (T);
    glCheck(glGenBuffers(1, &m_handle));
    activate();
    glCheck(glBufferData(m_target, bytes, nullptr, m_usage));
    //deactivate();
    return false;
  }

  virtual void release() override
  {
    glCheck(glDeleteBuffers(1, &m_handle));
  }

  virtual void activate() override
  {
    glCheck(glBindBuffer(m_target, m_handle));
  }

  virtual void deactivate() override
  {
    glCheck(glBindBuffer(m_target, 0));
  }

  virtual bool setup() override
  {
    return false;
  }

  virtual inline bool needUpdate() const override
  {
    return PendingData::hasPendingData();
  }

  virtual bool update() override
  {
    uint32_t pos_start;
    uint32_t pos_end;
    PendingData::getPendingData(pos_start, pos_end);

    uint32_t offset = sizeof (T) * pos_start;
    uint32_t nbytes = sizeof (T) * (pos_end - pos_start + 1U);
    glCheck(glBufferSubData(m_target, offset, nbytes, &m_data[0]));
    PendingData::clearPending();
    return false;
  }

  //protected:
public:

  GLAttrib m_attrib;
  GLenum m_usage;
  std::vector<T> m_data;
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
