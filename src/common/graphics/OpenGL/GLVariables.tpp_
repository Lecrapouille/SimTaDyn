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
// along with GNU Emacs.  If not, see <http://www.gnu.org/licenses/>.
//=====================================================================

#ifndef GLVARIABLES_HPP
#  define GLVARIABLES_HPP

#  include "Matrix.tpp"

// TODO: quand un Attribute a change alors prevenir son GLProgram
// afin qu'il recalcule les tailles.

// **************************************************************
//!
// **************************************************************
class IGLVariable: public IGLObject<GLint>
{
public:

  //! \brief
  IGLVariable(const char *name, GLenum gltype, GLuint prog)
    : IGLObject(name)
  {
    m_gltype = gltype;
    m_program = prog;
  }

  /* FIXME: pouvoir faire prog["color"] = 2.0f; au lieu de
   * m_quad.setUniformVal("scale", 2.0f);
  //Manipule donnee depuis le CPU
  template<class U>
  IGLVariable& operator=(const U& val)
  {
    std::cout << "FFFFFFFFF " << val << std::endl;
    //setValue(val); // FIXME: impossible
    forceUpdate();
    return *this;
    }*/
protected:

  GLenum  m_gltype;
  GLuint  m_program;
};

// **************************************************************
//!
// **************************************************************
template<class T>
class GLAttribute: public IGLVariable
{
public:

  GLAttribute(const char *name, GLint dim, GLenum gltype, GLuint prog)
    : IGLVariable(name, gltype, prog),
      m_data("VBO_" + std::string(name))
  {
    assert((dim >= 1) && (dim <= 4));
    m_dim = dim;
  }

  virtual ~GLAttribute() override { destroy(); }

  inline PendingContainer<T> const& cdata() const
  {
    return m_data.m_container;
  }

  inline PendingContainer<T>& data()
  {
    return m_data.m_container;
  }

private:

  virtual bool create() override
  {
    m_handle = glCheck(glGetAttribLocation(m_program, name().c_str()));
    m_index = static_cast<GLuint>(m_handle);
    return false;
  }

  virtual void release() override
  {
  }

  virtual void activate() override
  {
    m_data.begin();
    glCheck(glEnableVertexAttribArray(m_index));
    glCheck(glVertexAttribPointer(m_index,
                                  m_dim,
                                  m_gltype,
                                  GL_FALSE,
                                  m_stride,
                                  (const GLvoid*) m_offset));
  }

  virtual void deactivate() override
  {
    m_data.end();
    if (isValid() /* && m_data.isVBO() */)
      {
        glCheck(glDisableVertexAttribArray(m_index));
      }
  }

  virtual bool setup() override
  {
    return false;
  }

  virtual bool update() override
  {
    return false;
  }

public:

  size_t m_stride = 0;
  size_t m_offset = 0;

private:

  GLVertexBuffer<T> m_data;
  GLint m_dim;
  GLuint m_index;
};

// **************************************************************
//!
// **************************************************************
template<class T>
class IGLUniform: public IGLVariable
{
public:

  // Note T and gltype shall match. Not checks are made
  IGLUniform(const char *name, GLenum gltype, GLuint prog)
    : IGLVariable(name, gltype, prog)
  {
  }

  virtual ~IGLUniform() override
  {
    destroy();
  }

  inline T const& data() const
  {
    return m_data;
  }

  inline T& data()
  {
    // FIXME always modified ?
    forceUpdate();

    return m_data;
  }

private:

  virtual bool create() override
  {
    m_handle = glCheck(glGetUniformLocation(m_program, name().c_str()));
    return false;
  }

  virtual void release() override
  {
  }

  virtual void activate() override
  {
  }

  virtual void deactivate() override
  {
  }

  virtual bool setup() override
  {
    return false;
  }

  virtual bool update() override
  {
    return false;
  }

protected:

  T m_data;
};

// **************************************************************
//!
// **************************************************************
template<class T>
class GLUniform: public IGLUniform<T>
{
public:

  GLUniform(const char *name, GLenum gltype, GLuint prog)
    : IGLUniform<T>(name, gltype, prog)
  {
  }

  // Manipule donnee depuis le CPU
  template<class U>
  GLUniform<T>& operator=(const U& val)
  {
    IGLUniform<T>::data() = T(val);
    return *this;
  }

private:

  virtual bool update() override
  {
    setValue(IGLUniform<T>::m_data);
    return false;
  }

  inline void setValue(const T& value) const;
};

template<>
inline void GLUniform<int>::setValue(const int& value) const
{
  glCheck(glUniform1i(m_handle, value));
}

template<>
inline void GLUniform<float>::setValue(const float& value) const
{
  glCheck(glUniform1f(m_handle, value));
}

template<>
inline void GLUniform<Vector2f>::setValue(const Vector2f& value) const
{
  glCheck(glUniform2f(m_handle, value.x, value.y));
}

template<>
inline void GLUniform<Vector3f>::setValue(const Vector3f& value) const
{
  glCheck(glUniform3f(m_handle, value.x, value.y, value.z));
}

template<>
inline void GLUniform<Vector4f>::setValue(const Vector4f& value) const
{
  glCheck(glUniform4f(m_handle, value.x, value.y, value.z, value.w));
}

template<>
inline void GLUniform<Vector2i>::setValue(const Vector2i& value) const
{
  glCheck(glUniform2i(m_handle, value.x, value.y));
}

template<>
inline void GLUniform<Vector3i>::setValue(const Vector3i& value) const
{
  glCheck(glUniform3i(m_handle, value.x, value.y, value.z));
}

template<>
inline void GLUniform<Vector4i>::setValue(const Vector4i& value) const
{
  glCheck(glUniform4i(m_handle, value.x, value.y, value.z, value.w));
}

/*
template<>
inline void GLUniform<float*>::setValue(const float* values, uint count) const
{
  glCheck(glUniform1fv(m_handle, count, values));
}

template<>
inline void GLUniform<Vector2f*>::setValue(const Vector2f* values, uint count) const
{
  glCheck(glUniform2fv(m_handle, count, static_cast<float*>(values)));
}

template<>
inline void GLUniform<Vector3f*>::setValue(const Vector3f* values, uint count) const
{
  glCheck(glUniform3fv(m_handle, count, static_cast<float*>(values)));
}

template<>
inline void GLUniform<Vector4f*>::setValue(const Vector4f* values, uint count) const
{
  glCheck(glUniform4fv(m_handle, count, static_cast<float*>(values)));
}
*/

template<>
inline void GLUniform<Matrix22f>::setValue(const Matrix22f& value) const
{
  glCheck(glUniformMatrix2fv(m_handle, 1, GL_FALSE, &value[0][0]));
}

template<>
inline void GLUniform<Matrix33f>::setValue(const Matrix33f& value) const
{
  glCheck(glUniformMatrix3fv(m_handle, 1, GL_FALSE, &value[0][0]));
}

template<>
inline void GLUniform<Matrix44f>::setValue(const Matrix44f& value) const
{
  glCheck(glUniformMatrix4fv(m_handle, 1, GL_FALSE, &value[0][0]));
}

// **************************************************************
//! \brief A GLSampler is an Opengl uniform for texture
// **************************************************************
template<class T>
class GLSampler: public IGLUniform<T>
{
public:

  GLSampler(const char *name, GLenum gltype, uint32_t texture_count, GLuint prog)
    : IGLUniform<T>(name, gltype, prog)
  {
    m_texture_count = texture_count;
  }

  inline uint32_t textureID() const
  {
    return m_texture_count;
  }

private:

  virtual void activate() override
  {
    glCheck(glActiveTexture(GL_TEXTURE0 + m_texture_count));
    IGLUniform<T>::m_data.begin();
  }

  virtual bool update() override
  {
    glCheck(glUniform1i(IGLUniform<T>::m_handle,
                        static_cast<GLint>(m_texture_count)));
    return false;
  }

private:

  uint32_t m_texture_count = 0;
};

// **************************************************************
//! TODO
// **************************************************************
#if 0
class GLSampler1D: public GLSampler<GLTexture1D>
{
public:

  GLSampler1D(const char *name, uint32_t texture_count, GLuint prog)
    : GLSampler<GLTexture1D>(name, GL_SAMPLER_1D, texture_count, prog)
  {
  }
};
#endif

// **************************************************************
//!
// **************************************************************
class GLSampler2D: public GLSampler<GLTexture2D>
{
public:

  GLSampler2D(const char *name, uint32_t texture_count, GLuint prog)
    : GLSampler<GLTexture2D>(name, GL_SAMPLER_2D, texture_count, prog)
  {
  }
};

// **************************************************************
//!
// **************************************************************
#if 0
class GLSampler3D: public GLSampler<GLTexture3D>
{
public:

  GLSampler3D(const char *name, uint32_t texture_count, GLuint prog)
    : GLSampler<GLTexture3D>(name, GL_SAMPLER_CUBE, texture_count, prog)
  {
  }
};
#endif

#endif
