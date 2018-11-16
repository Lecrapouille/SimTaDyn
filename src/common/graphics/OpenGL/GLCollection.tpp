// -*- c++ -*- Coloration Syntaxique pour Emacs
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
// along with SimTaDyn.  If not, see <http://www.gnu.org/licenses/>.
//=====================================================================

#ifndef GL_COLLECTION_TPP_
#  define GL_COLLECTION_TPP_

//! \file This class get its inspiration from the code of the Glumpy
//! project (Python + Numpy + modern OpenGL: a fast, scalable and
//! beautiful scientific visualization). https://github.com/glumpy/glumpy
//!
//! Original Copyright from Glumpy project's Python code:
//! Copyright (c) 2009-2016 Nicolas P. Rougier. All rights reserved.
//! Distributed under the (new) BSD License.

#  include "GLAttrib.hpp"
//#  include "GLVertexArray.hpp"
#  include "Set.tpp"
#  include <algorithm>
#  include <cassert>

// **************************************************************
//!
// **************************************************************
template<typename T, const size_t N>
class GLBlockBuffer: public GLObject<GLenum>, public Block<T, N>
{
protected:

#  include "ContainerEnums.ipp"

public:

  //! \brief A GLBlockBuffer is an interface used to upload data to a GPU
  //! array buffer (GL_ARRAY_BUFFER or GL_ELEMENT_ARRAY_BUFFER).
  GLBlockBuffer(const bool lazy, const GLenum target, const GLenum usage = GL_DYNAMIC_DRAW)
    : GLObject("GLBlockBuffer"), Block<T, N>(lazy)
  {
    m_target = target;
    m_usage = usage;
  }

  GLBlockBuffer(const bool lazy, std::string const& name, const GLenum target, const GLenum usage = GL_DYNAMIC_DRAW)
    : GLObject(name), Block<T, N>(lazy)
  {
    m_target = target;
    m_usage = usage;
  }

  GLBlockBuffer(const bool lazy, const char *name, const GLenum target, const GLenum usage = GL_DYNAMIC_DRAW)
    : GLObject(name), Block<T, N>(lazy)
  {
    m_target = target;
    m_usage = usage;
  }

  //! \brief Delete the object from GPU memory.
  ~GLBlockBuffer()
  {
    GLObject::destroy();
  }

protected:

  //! \brief Allocate memory in the GPU.
  virtual bool create() override
  {
    // Total size of the container
    const size_t bytes = Block<T, N>::size() * sizeof (T);

    glCheck(glGenBuffers(1, &m_handle));
    activate();
    glCheck(glBufferData(m_target, bytes, nullptr, m_usage));
    //deactivate();
    return false;
  }

  //! \brief Delete the object from GPU memory.
  virtual inline void release() override
  {
    glCheck(glDeleteBuffers(1, &m_handle));
  }

  //! \brief Bind the buffer to some target.
  virtual inline void activate() override
  {
    glCheck(glBindBuffer(m_target, m_handle));
  }

  //! \brief Unbind the current bound buffer.
  virtual inline void deactivate() override
  {
    glCheck(glBindBuffer(m_target, 0));
  }

  //! \brief Whether object needs to be updated.
  virtual inline bool needUpdate() const override
  {
    return Block<T, N>::hasPendingData();
  }

  //! \brief Upload all pending data to GPU.
  virtual inline bool update() override
  {
    size_t pos_start;
    size_t pos_end;

    PendingData::getPendingData(pos_start, pos_end);
    LOGI("GLVertexBuffer named '%s' updating %u --> %u", m_name.c_str(), pos_start, pos_end);
    m_offset = sizeof (T) * pos_start;
    m_nbytes = sizeof (T) * (pos_end - pos_start + 1U);

    LOGI("GLVertexBuffer named '%s' updating Offset:%u --> Bytes:%u", m_name.c_str(), m_offset, m_nbytes);
    glCheck(glBufferSubData(m_target, m_offset, m_nbytes, Block<T, N>::m_block));
    Block<T, N>::clearPending();

    return false;
  }

  //! \brief Dummy configuration.
  virtual bool setup() override
  {
    return false;
  }

private:

  //! Indicate which elements have been changed.
  size_t m_offset, m_nbytes;

protected:

  GLenum m_usage;
};

// **************************************************************
//! \brief Buffer for vertex attribute data.
// **************************************************************
template<typename T, const size_t N>
class GLVertexBlockBuffer: public GLBlockBuffer<T, N>
{
public:

  GLVertexBlockBuffer(const bool lazy, const GLenum usage = GL_DYNAMIC_DRAW)
    : GLBlockBuffer<T, N>(lazy, "GLVertexBlockBuffer", GL_ARRAY_BUFFER, usage)
  {
  }

  GLVertexBlockBuffer(const bool lazy, std::string const& name, const GLenum usage = GL_DYNAMIC_DRAW)
    : GLBlockBuffer<T, N>(lazy, name, GL_ARRAY_BUFFER, usage)
  {
  }

  GLVertexBlockBuffer(const bool lazy, const char *name, const GLenum usage = GL_DYNAMIC_DRAW)
    : GLBlockBuffer<T, N>(lazy, name, GL_ARRAY_BUFFER, usage)
  {
  }
};

// **************************************************************
//! \brief Buffer for index data.
// **************************************************************
template<typename T, const size_t N>
class GLIndexBlockBuffer: public GLBlockBuffer<T, N>
{
public:

  GLIndexBlockBuffer(const bool lazy, const GLenum usage = GL_DYNAMIC_DRAW)
    : GLBlockBuffer<T, N>(lazy, "GLIndexBlockBuffer", GL_ELEMENT_ARRAY_BUFFER, usage)
  {
  }

  GLIndexBlockBuffer(const bool lazy, std::string const& name, const GLenum usage = GL_DYNAMIC_DRAW)
    : GLBlockBuffer<T, N>(lazy, name, GL_ELEMENT_ARRAY_BUFFER, usage)
  {
  }

  GLIndexBlockBuffer(const bool lazy, const char *name, const GLenum usage = GL_DYNAMIC_DRAW)
    : GLBlockBuffer<T, N>(lazy, name, GL_ELEMENT_ARRAY_BUFFER, usage)
  {
  }
};

// **************************************************************
//! \brief
// **************************************************************
template<typename T, const size_t N,
         template<typename X, const size_t Y> class Block = GLBlockBuffer>
class GLCollection: public GLAttrib, public Set<T, N, Block>
{
public:

  GLCollection(std::string const& name, const size_t reserve_elements = 1)
    : GLAttrib(name), Set<T,N,Block>(reserve_elements)
  {
  }

  GLCollection(const char *name, const size_t reserve_elements = 1)
    : GLAttrib(name), Set<T,N,Block>(reserve_elements)
  {
  }

  virtual void setup(GLShader& program, const GLint size, const GLenum type, const GLboolean normalized = GL_FALSE) override
  {
    if (nullptr == Set<T, N, Block>::block(0))
      {
        Set<T, N, Block>::reserve(1);
      }
    Set<T, N, Block>::block(0)->start();
    GLAttrib::setup(program, size, type, normalized);
    GLAttrib::start();
    Set<T, N, Block>::block(0)->stop();
  }

  virtual void stop() override
  {
    if (nullptr != Set<T, N, Block>::block(0))
      Set<T, N, Block>::block(0)->stop();
    GLAttrib::stop();
  }

};

// **************************************************************
//! \brief
// **************************************************************
template<typename T, const size_t N,
         template<typename X, const size_t Y> class Block = GLVertexBlockBuffer>
class GLVertexCollection: public GLCollection<T, N, Block>
{
public:

  GLVertexCollection(const size_t reserve_elements = 1)
    : GLCollection<T,N,Block>("GLVertexCollection", reserve_elements)
  {
    LOGI("New GLVertexCollection with %u elements", reserve_elements);
  }

  GLVertexCollection(const char *name, const size_t reserve_elements = 1)
    : GLCollection<T,N,Block>(name, reserve_elements)
  {
    LOGI("New GLVertexCollection with %u elements", reserve_elements);
  }

  GLVertexCollection(std::string const& name, const size_t reserve_elements = 1)
    : GLCollection<T,N,Block>(name, reserve_elements)
  {
    LOGI("New GLVertexCollection with %u elements", reserve_elements);
  }
};

// **************************************************************
//! \brief
// **************************************************************
template<typename T, const size_t N,
         template<typename X, const size_t Y> class Block = GLIndexBlockBuffer>
class GLIndexCollection: public GLCollection<T, N, Block>
{
public:

  GLIndexCollection(const size_t reserve_elements = 1)
    : GLCollection<T,N,Block>("GLIndexCollection", reserve_elements)
  {
    LOGI("New GLIndexCollection with %u elements", reserve_elements);
  }

  GLIndexCollection(const char *name, const size_t reserve_elements = 1)
    : GLCollection<T,N,Block>(name, reserve_elements)
  {
    LOGI("New GLIndexCollection with %u elements", reserve_elements);
  }

  GLIndexCollection(std::string const& name, const size_t reserve_elements = 1)
    : GLCollection<T,N,Block>(name, reserve_elements)
  {
    LOGI("New GLIndexCollection with %u elements", reserve_elements);
  }
};

#endif /* GL_COLLECTION_TPP_ */
