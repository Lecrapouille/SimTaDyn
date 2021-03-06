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
//
// This file is a derived work of https://github.com/glumpy/glumpy
//
// Copyright (c) 2009-2016 Nicolas P. Rougier. All rights reserved.
// Distributed under the (new) BSD License.
//=====================================================================

#ifndef GLVAO_HPP
#  define GLVAO_HPP

// *****************************************************************************
//! \file GLVAO.hpp handles the OpenGL representation of 3D models.
// *****************************************************************************

#  include "Matrix.tpp"
#  include "GLVBO.tpp"
#  include "GLTextures.hpp"
#  include <unordered_map>
#  include <vector>

// *****************************************************************************
//! \class GLVAO GLVAO.hpp
//!
//! \brief Class handling the OpenGL representation of a 3D model.
//!
//! GLVAO has its list of VBOs and textures populated by a GLProgram once
//! bind. GLVAO is drawn by a GLProgram.
// *****************************************************************************
class GLVAO: public IGLObject<GLenum>
{
  //! \brief GLProgram directly modifies GLVAO states.
  friend class GLProgram;

  // Unordered map = hash table = O(1) access time.
  using GLBufferPtr = std::unique_ptr<IGLBuffer>;
  using IGLTexturePtr = std::unique_ptr<IGLTexture>;
  using mapGLBuffer = std::unordered_map<std::string, GLBufferPtr>;
  using mapGLTexture = std::unordered_map<std::string, IGLTexturePtr>;

public:

  //----------------------------------------------------------------------------
  //! \brief Constructor.
  //!
  //! Give a name to the instance. This constructor makes no other
  //! actions.
  //!
  //! \param name the name of this instance (used for debug and traces).
  //----------------------------------------------------------------------------
  GLVAO(std::string const& name)
    : IGLObject(name)
  {
    m_target = GL_ARRAY_BUFFER;
  }

  //----------------------------------------------------------------------------
  //! \brief Destructor. Release elements in CPU and GPU memories.
  //----------------------------------------------------------------------------
  virtual ~GLVAO() override
  {
    destroy();
  }

  //----------------------------------------------------------------------------
  //! \brief Return the list of VBO names. This is method is mainly
  //! used for debug purpose.
  //!
  //! \note if the VAO has never been bound to a GLProgram the return
  //! lit will be empty.
  //----------------------------------------------------------------------------
  std::vector<std::string> VBONames()
  {
    std::vector<std::string> list;
    list.reserve(m_vbos.size());
    for (auto& it: m_vbos)
      {
        list.push_back(it.first);
      }
    return list;
  }

  //----------------------------------------------------------------------------
  //! \brief Return the list of textures names. This is method is mainly
  //! used for debug purpose.
  //!
  //! \note if the VAO has never been bound to a GLProgram the return
  //! lit will be empty.
  //!
  //! \note Do not be confused with meaning of texture name. We do not
  //! refer to the jpeg, png or bmp file name but to the shader
  //! uniform name (used in shaders).
  //----------------------------------------------------------------------------
  std::vector<std::string> TextureNames()
  {
    std::vector<std::string> list;
    list.reserve(m_textures.size());
    for (auto& it: m_textures)
      {
        list.push_back(it.first);
      }
    return list;
  }

  //----------------------------------------------------------------------------
  //! \brief Check if this instance holds a GLIndexBuffer.
  //!
  //! \return true if the .
  //----------------------------------------------------------------------------
  inline bool hasIndex()
  {
    return nullptr != m_index.get();
  }

  //----------------------------------------------------------------------------
  //! \brief Get the index type of uint32_t.
  //!
  //! This method wraps the \a index() method hidding the misery of the
  //! template.
  //----------------------------------------------------------------------------
  inline GLIndexBuffer<uint32_t>& index32(BufferUsage const usage = BufferUsage::DYNAMIC_DRAW)
  {
    return index<uint32_t>(usage);
  }

  //----------------------------------------------------------------------------
  //! \brief Get the index type of uint16_t.
  //!
  //! This method wraps the \a index() method hidding the misery of the
  //! template.
  //----------------------------------------------------------------------------
  inline GLIndexBuffer<uint16_t>& index16(BufferUsage const usage = BufferUsage::DYNAMIC_DRAW)
  {
    return index<uint16_t>(usage);
  }

  //----------------------------------------------------------------------------
  //! \brief Get the index type of uint8_t.
  //!
  //! This method wraps the \a index() method hidding the misery of the
  //! template.
  //----------------------------------------------------------------------------
  inline GLIndexBuffer<uint8_t>& index8(BufferUsage const usage = BufferUsage::DYNAMIC_DRAW)
  {
    return index<uint8_t>(usage);
  }

  //----------------------------------------------------------------------------
  //! \brief Check if this instance has a non empty list of VBOs.
  //!
  //! \return true if the list is non empty.
  //----------------------------------------------------------------------------
  inline bool hasVBOs() const
  {
    return 0_z != m_vbos.size();
  }

  //----------------------------------------------------------------------------
  //! \brief Check if the name passed as parameter matches the name of
  //! a VBO.
  //!
  //! \return true if contains it, else return false.
  //----------------------------------------------------------------------------
  inline bool hasVBO(const char *name) const
  {
    return m_vbos.end() != m_vbos.find(name);
  }

  //----------------------------------------------------------------------------
  //! \brief Check if this instance has a non empty list of textures.
  //!
  //! \return true if the list is non empty.
  //----------------------------------------------------------------------------
  inline bool hasTextures() const
  {
    return 0_z != m_textures.size();
  }

  //----------------------------------------------------------------------------
  //! \brief Check if the name passed as parameter matches the name of
  //! a texture.
  //!
  //! \return true if contains it, else return false.
  //----------------------------------------------------------------------------
  inline bool hasTexture(const char *name) const
  {
    return m_textures.end() != m_textures.find(name);
  }

  //----------------------------------------------------------------------------
  //! \brief
  //!
  //! \note: name duplicata is not managed because this case should
  //! never happen.
  //----------------------------------------------------------------------------
  template<typename T>
  bool createVBO(const char *name, size_t const vbo_init_size, BufferUsage const usage)
  {
    if (unlikely(hasVBO(name)))
      {
        LOGE("VAO '%s' Tried to create a VBO with name '%s' already used",
             cname(), name);
        return false;
      }
    LOGD("VAO '%s' creating a new VBO '%s' of %zu elements of %zu bytes",
         cname(), name, vbo_init_size, sizeof (T));
    m_vbos[name] = std::make_unique<GLVertexBuffer<T>>(name, vbo_init_size, usage);
    return true;
  }

  //----------------------------------------------------------------------------
  //! \brief Get the VBO float 4D vector.
  //!
  //! This method wraps the \a VBO() method hidding the misery of the
  //! template.
  //----------------------------------------------------------------------------
  inline GLVertexBuffer<Vector4f>& vector4f(const char *name)
  {
    return VBO<Vector4f>(name);
  }

  //----------------------------------------------------------------------------
  //! \brief Get the VBO float 3D vector.
  //!
  //! This method wraps the \a VBO() method hidding the misery of the
  //! template.
  //----------------------------------------------------------------------------
  inline GLVertexBuffer<Vector3f>& vector3f(const char *name)
  {
    return VBO<Vector3f>(name);
  }

  //----------------------------------------------------------------------------
  //! \brief Get the VBO float 2D vector.
  //!
  //! This method wraps the \a VBO() method hidding the misery of the
  //! template.
  //----------------------------------------------------------------------------
  inline GLVertexBuffer<Vector2f>& vector2f(const char *name)
  {
    return VBO<Vector2f>(name);
  }

  //----------------------------------------------------------------------------
  //! \brief Get the VBO float scalar.
  //!
  //! This method wraps the \a VBO() method hidding the misery of the
  //! template.
  //----------------------------------------------------------------------------
  inline GLVertexBuffer<float>& scalarf(const char *name)
  {
    return VBO<float>(name);
  }

  //----------------------------------------------------------------------------
  //! \brief Get the 1D texture.
  //!
  //! This method wraps the \a texture() method hidding the misery of the
  //! template.
  //----------------------------------------------------------------------------
  inline GLTexture1D& texture1D(const char *name)
  {
    return texture<GLTexture1D>(name);
  }

  //----------------------------------------------------------------------------
  //! \brief Get the 2D texture.
  //!
  //! This method wraps the \a texture() method hidding the misery of the
  //! template.
  //----------------------------------------------------------------------------
  inline GLTexture2D& texture2D(const char *name)
  {
    return texture<GLTexture2D>(name);
  }

  //----------------------------------------------------------------------------
  //! \brief Get the 3D texture.
  //!
  //! This method wraps the \a texture() method hidding the misery of the
  //! template.
  //----------------------------------------------------------------------------
  inline GLTexture3D& texture3D(const char *name)
  {
    return texture<GLTexture3D>(name);
  }

  //----------------------------------------------------------------------------
  //! \brief Get the cubic texture.
  //!
  //! This method wraps the \a texture() method hidding the misery of the
  //! template.
  //----------------------------------------------------------------------------
  inline GLTextureCube& textureCube(const char *name)
  {
    return texture<GLTextureCube>(name);
  }

  //------------------------------------------------------------------
  //! \brief Check if all VBOs have the same size.
  //! \return true if all VBO have the same size, else return false.
  //------------------------------------------------------------------
  bool checkVBOSizes()
  {
    LOGD("VAO '%s' check VBO sizes", cname());
    if (likely(m_vbo_size_verified))
      {
        return true;
      }

    if (unlikely(prog == 0 || m_vbos.empty()))
      {
        LOGE("VAO '%s' is not yet bound to a GLProgram", cname());
        return false;
      }

    size_t size = m_vbos.begin()->second->size();
    for (auto& it: m_vbos)
      {
        if (unlikely(size != it.second->size()))
          {
            LOGE("VAO '%s' does not have all of its VBOs with the same size",
                  cname());
            for (auto& itt: m_vbos)
              {
                LOGE("=> VBO '%s' size is %zu", itt.first.c_str(), itt.second->size());
              }
            return false;
          }
      }
    m_vbo_size_verified = true;
    return true;
  }

private:

  //----------------------------------------------------------------------------
  //! \brief Locate and get the VBO refered by the name and its type T.
  //!
  //! \return the reference of the VBO if it exists.
  //!
  //! \throw OpenGLException if the VBO is not in the list
  //! (probably due to a typo in the name) or if the type T does not match.
  //----------------------------------------------------------------------------
  template<typename T>
  GLVertexBuffer<T>& VBO(const char *name)
  {
    LOGD("VAO '%s' get VBO '%s'", cname(), name);
    if (unlikely(nullptr == name))
      {
        throw OpenGLException("nullptr passed to VBO()");
      }

    auto ptr = m_vbos[name].get();
    if (unlikely(nullptr == ptr))
      {
        if (prog == 0 || m_vbos.empty())
          {
            throw OpenGLException("GLVertexBuffer '" + std::string(name) +
                                  "' does not exist because VAO '" + cname()
                                  + "' is not bound to a GLProgram");
          }
        else
          {
            throw OpenGLException("GLVertexBuffer '" + std::string(name) +
                                  "' does not exist");
          }
      }

    GLVertexBuffer<T> *vbo = dynamic_cast<GLVertexBuffer<T>*>(ptr);
    if (unlikely(nullptr == vbo))
      {
        throw OpenGLException("GLVertexBuffer '" + std::string(name) +
                              "' exists but has wrong template type");
      }

    m_vbo_size_verified = false;
    return *vbo;
  }

  //----------------------------------------------------------------------------
  //! \brief Get the GLIndexBuffer.
  //! \note if the GLIndexBuffer does not exist it is created.
  //----------------------------------------------------------------------------
  template<typename T>
  GLIndexBuffer<T>& index(BufferUsage const usage)
  {
    LOGD("VAO '%s' get index", cname());
    auto ptr = m_index.get();
    if (unlikely(nullptr == ptr))
      {
        m_index = std::make_unique<GLIndexBuffer<T>>(name() + "_index", usage);
        ptr = m_index.get();
      }

    GLIndexBuffer<T> *ib = dynamic_cast<GLIndexBuffer<T>*>(ptr);
    if (unlikely(nullptr == ib))
      {
        throw OpenGLException("GLIndexBuffer exists but has wrong template type");
      }

    return *ib;
  }

  //----------------------------------------------------------------------------
  //! \brief Locate and get the texture refered by the name and its type T.
  //!
  //! \return the reference of the texture if it exists.
  //!
  //! \throw OpenGLException if the VBO is not in the list
  //! (probably due to a typo in the name) or if the type T does not match.
  //----------------------------------------------------------------------------
  template<typename T>
  T& texture(const char *name)
  {
    LOGD("VAO '%s' get texture '%s'", cname(), name);
    if (unlikely(nullptr == name))
      {
        throw OpenGLException("nullptr passed to texture()");
      }

    auto ptr = m_textures[name].get();
    if (unlikely(nullptr == ptr))
      {
        if (prog == 0 || m_vbos.empty())
          {
            throw OpenGLException("GLTexture '" + std::string(name) +
                                  "' does not exist because VAO '" + cname()
                                  + "' is not bound to a GLProgram");
          }
        else
          {
            throw OpenGLException("GLTexture '" + std::string(name) +
                                  "' does not exist");
          }
      }

    T* tex = dynamic_cast<T*>(ptr);
    if (unlikely(nullptr == tex))
      {
        throw OpenGLException("GLTexture '" + std::string(name) +
                              "' exists but has wrong template type");
      }

    return *tex;
  }

  //----------------------------------------------------------------------------
  //! \brief
  //!
  //! \note: name duplicata is not managed because this case should
  //! never happen.
  //----------------------------------------------------------------------------
  template<typename T>
  bool createTexture(const char *name)
  {
    if (unlikely(hasTexture(name)))
      {
        LOGE("VAO '%s' Tried to create a texture with name '%s' already used",
              cname(), name);
        return false;
      }
    m_textures[name] = std::make_unique<T>(name);
    LOGD("VAO '%s' has a new texture '%s'", cname(), name);
    return true;
  }

  //----------------------------------------------------------------------------
  //! \brief Create a new OpenGL VAO.
  //----------------------------------------------------------------------------
  virtual bool create() override
  {
    LOGD("VAO '%s' create", cname());
    glCheck(glGenVertexArrays(1, &m_handle));
    return false;
  }

  //----------------------------------------------------------------------------
  //! \brief Bind the VAO to OpenGL.
  //----------------------------------------------------------------------------
  virtual void activate() override
  {
    LOGD("VAO '%s' activate", cname());
    glCheck(glBindVertexArray(m_handle));
  }

  //----------------------------------------------------------------------------
  //! \brief Dummy method. Nothing is made.
  //----------------------------------------------------------------------------
  virtual bool setup() override
  {
    return false;
  }

  //----------------------------------------------------------------------------
  //! \brief Dummy method. Nothing is made.
  //----------------------------------------------------------------------------
  virtual bool update() override
  {
    return false;
  }

  //----------------------------------------------------------------------------
  //! \brief Unbind the VAO from OpenGL.
  //----------------------------------------------------------------------------
  virtual void deactivate() override
  {
    LOGD("VAO '%s' deactivate", cname());
    glCheck(glBindVertexArray(0U));
  }

  //----------------------------------------------------------------------------
  //! \brief Destroy the OpenGL VAO.
  //----------------------------------------------------------------------------
  virtual void release() override
  {
    LOGD("VAO '%s' release", cname());
    glCheck(glDeleteVertexArrays(1, &m_handle));
  }

private:

  //! \brief Hold VBOs.
  // FIXME instead of hold IGLBuffer we should hold GLBuffer<T>
  // but this is not possible in C++. So I have no idea.
  mapGLBuffer m_vbos;
  //! \brief Hold textures.
  mapGLTexture m_textures;
  //! \brief Optionally hold vertex indices.
  GLBufferPtr m_index;
  //! \brief Hold the ID of the bound GLProgam.
  //! \note this varibale is modified durectly
  //! by the GLProgam.
  GLenum prog = 0;
  //! \brief boolean avoiding to make uncessary checks on VBO size.
  bool m_vbo_size_verified = false;
};

#endif // GLVAO_HPP
