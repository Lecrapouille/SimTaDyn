// -*- c++ -*- Coloration Syntaxique pour Emacs

#ifndef GLVERTEX_BUFFER_TPP_
#  define GLVERTEX_BUFFER_TPP_

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
template<typename T, const uint32_t N>
class GLBlockBuffer: public GLObject, public Block<T, N>
{
protected:

#  include "ContainerEnums.ipp"

public:

  //! \brief A GLBlockBuffer is an interface used to upload data to a GPU
  //! array buffer (GL_ARRAY_BUFFER or GL_ELEMENT_ARRAY_BUFFER).
  GLBlockBuffer(const GLenum target, const GLenum usage = GL_DYNAMIC_DRAW)
    : GLObject("GLBlockBuffer"), Block<T, N>()
  {
    LOGI("New GLBlockBuffer with no name");
    m_target = target;
    m_usage = usage;
  }

  GLBlockBuffer(std::string const& name, const GLenum target, const GLenum usage = GL_DYNAMIC_DRAW)
    : GLObject(name), Block<T, N>()
  {
    LOGI("New GLBlockBuffer named '%s'", name.c_str());
    m_target = target;
    m_usage = usage;
  }

  GLBlockBuffer(const char *name, const GLenum target, const GLenum usage = GL_DYNAMIC_DRAW)
    : GLObject(name), Block<T, N>()
  {
    LOGI("New GLBlockBuffer named '%s'", name);
    m_target = target;
    m_usage = usage;
  }

  //! \brief Delete the object from GPU memory.
  ~GLBlockBuffer()
  {
    GLObject::destroy();
  }

  //protected:

  //! \brief Allocate memory in the GPU.
  virtual bool create() override
  {
    // Total size of the container
    const uint32_t bytes = Block<T, N>::size() * sizeof (T);

    LOGI("GLVertexBuffer named '%s' reserving GPU memory", m_name.c_str());
    glCheck(glGenBuffers(1, &m_handle));
    activate();
    glCheck(glBufferData(m_target, bytes, nullptr, m_usage));
    //deactivate();
    return false;
  }

  //! \brief Delete the object from GPU memory.
  virtual inline void release() override
  {
    LOGI("GLVertexBuffer named '%s' release", m_name.c_str());
    glCheck(glDeleteBuffers(1, &m_handle));
  }

  //! \brief Bind the buffer to some target.
  virtual inline void activate() override
  {
    LOGI("GLVertexBuffer named '%s' activated #%u", m_name.c_str(), m_handle);
    glCheck(glBindBuffer(m_target, m_handle));
  }

  //! \brief Unbind the current bound buffer.
  virtual inline void deactivate() override
  {
    LOGI("GLVertexBuffer named '%s' deactivated #%u", m_name.c_str(), m_handle);
    glCheck(glBindBuffer(m_target, 0));
  }

  //! \brief Whether object needs to be updated.
  virtual inline bool needUpdate() const override
  {
    LOGI("GLVertexBuffer named '%s' need update ?", m_name.c_str());
    return Block<T, N>::hasPendingData();
  }

  //! \brief Upload all pending data to GPU.
  virtual inline bool update() override
  {
    uint32_t pos_start;
    uint32_t pos_end;

    LOGI("GLVertexBuffer named '%s' updating", m_name.c_str());
    if (Block<T, N>::hasPendingData(pos_start, pos_end)) // FIXME the if() is useless
      {
        LOGI("GLVertexBuffer named '%s' updating %u --> %u", m_name.c_str(), pos_start, pos_end);
        m_offset = sizeof (T) * pos_start;
        m_nbytes = sizeof (T) * (pos_end - pos_start + 1U);
        LOGI("GLVertexBuffer named '%s' updating Offset:%u --> Bytes:%u", m_name.c_str(), m_offset, m_nbytes);

        glCheck(glBufferSubData(m_target, m_offset, m_nbytes, Block<T, N>::m_block));
        Block<T, N>::clearPending();
      }
    return false;
  }

  //! \brief Dummy configuration.
  virtual bool setup() override
  {
    LOGI("GLVertexBuffer named '%s' setup", m_name.c_str());
    return false;
  }

private:

  //! Indicate which elements have been changed.
  uint32_t m_offset, m_nbytes;

protected:

  GLenum m_usage;
};

// **************************************************************
//! \brief Buffer for vertex attribute data.
// **************************************************************
template<typename T, const uint32_t N>
class GLVertexBlockBuffer: public GLBlockBuffer<T, N>
{
public:

  GLVertexBlockBuffer(const GLenum usage = GL_DYNAMIC_DRAW)
    : GLBlockBuffer<T, N>("GLVertexBlockBuffer", GL_ARRAY_BUFFER, usage)
  {
    ////LOGI("New GLVertexBlockBuffer with no name");
  }

  GLVertexBlockBuffer(std::string const& name, const GLenum usage = GL_DYNAMIC_DRAW)
    : GLBlockBuffer<T, N>(name, GL_ARRAY_BUFFER, usage)
  {
    ////LOGI("New GLVertexBlockBuffer named '%s'", name.c_str());
  }

  GLVertexBlockBuffer(const char *name, const GLenum usage = GL_DYNAMIC_DRAW)
    : GLBlockBuffer<T, N>(name, GL_ARRAY_BUFFER, usage)
  {
    ////LOGI("New GLVertexBlockBuffer named '%s'", name);
  }
};

// **************************************************************
//! \brief Buffer for index data.
// **************************************************************
template<typename T, const uint32_t N>
class GLIndexBlockBuffer: public GLBlockBuffer<T, N>
{
public:

  GLIndexBlockBuffer(const GLenum usage = GL_DYNAMIC_DRAW)
    : GLBlockBuffer<T, N>("GLIndexBlockBuffer", GL_ELEMENT_ARRAY_BUFFER, usage)
  {
    ////LOGI("New GLIndexBlockBuffer with no name");
  }

  GLIndexBlockBuffer(std::string const& name, const GLenum usage = GL_DYNAMIC_DRAW)
    : GLBlockBuffer<T, N>(name, GL_ELEMENT_ARRAY_BUFFER, usage)
  {
    ////LOGI("New GLIndexBlockBuffer named '%s'", name);
  }

  GLIndexBlockBuffer(const char *name, const GLenum usage = GL_DYNAMIC_DRAW)
    : GLBlockBuffer<T, N>(name, GL_ELEMENT_ARRAY_BUFFER, usage)
  {
    ////LOGI("New GLIndexBlockBuffer named '%s'", name);
  }
};

// **************************************************************
//! \brief
// **************************************************************
template<typename T, const uint32_t N,
         template<typename X, const uint32_t Y> class Block = GLBlockBuffer>
class GLBuffer: public GLAttrib, public Set<T, N, Block>
{
public:

  GLBuffer(std::string const& name, const uint32_t reserve_elements = 1)
    : GLAttrib(name), Set<T,N,Block>(reserve_elements)
  {
    LOGI("New GLBuffer with %u elements", reserve_elements);
  }

  GLBuffer(const char *name, const uint32_t reserve_elements = 1)
    : GLAttrib(name), Set<T,N,Block>(reserve_elements)
  {
    LOGI("New GLBuffer with %u elements", reserve_elements);
  }

  virtual void setup(GLShader& program, const GLint size, const GLenum type, const GLboolean normalized = GL_FALSE) override
  {
    if (nullptr == Set<T, N, Block>::block(0))
      Set<T, N, Block>::reserve(1);
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
template<typename T, const uint32_t N,
         template<typename X, const uint32_t Y> class Block = GLVertexBlockBuffer>
class GLVertexBuffer: public GLBuffer<T, N, Block>
{
public:

  GLVertexBuffer(const uint32_t reserve_elements = 1)
    : GLBuffer<T,N,Block>("GLVertexBuffer", reserve_elements)
  {
    LOGI("New GLVertexBuffer with %u elements", reserve_elements);
  }

  GLVertexBuffer(const char *name, const uint32_t reserve_elements = 1)
    : GLBuffer<T,N,Block>(name, reserve_elements)
  {
    LOGI("New GLVertexBuffer with %u elements", reserve_elements);
  }

  GLVertexBuffer(std::string const& name, const uint32_t reserve_elements = 1)
    : GLBuffer<T,N,Block>(name, reserve_elements)
  {
    LOGI("New GLVertexBuffer with %u elements", reserve_elements);
  }

};

// **************************************************************
//! \brief
// **************************************************************
template<typename T, const uint32_t N,
         template<typename X, const uint32_t Y> class Block = GLIndexBlockBuffer>
class GLIndexBuffer: public GLBuffer<T, N, Block>
{
public:

  GLIndexBuffer(const uint32_t reserve_elements = 1)
    : GLBuffer<T,N,Block>("GLIndexBuffer", reserve_elements)
  {
    LOGI("New GLIndexBuffer with %u elements", reserve_elements);
  }

  GLIndexBuffer(const char *name, const uint32_t reserve_elements = 1)
    : GLBuffer<T,N,Block>(name, reserve_elements)
  {
    LOGI("New GLIndexBuffer with %u elements", reserve_elements);
  }

  GLIndexBuffer(std::string const& name, const uint32_t reserve_elements = 1)
    : GLBuffer<T,N,Block>(name, reserve_elements)
  {
    LOGI("New GLIndexBuffer with %u elements", reserve_elements);
  }
};

#endif /* GLVERTEX_BUFFER_TPP_ */
