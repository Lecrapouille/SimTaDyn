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

// **************************************************************
//!
// **************************************************************
class IGLVariable: public IGLObject<GLint>
{
public:

  enum Kind { ATTRIBUTE, UNIFORM };

  //! \brief
  IGLVariable(const char *name, GLenum gltype, GLuint prog, Kind kind)
    : IGLObject(name)
  {
    m_gltype = gltype;
    m_program = prog;
    m_kind = kind;
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

  inline Kind kind() const { return m_kind; }

protected:

  Kind m_kind;
  GLenum  m_gltype;
  GLuint  m_program;
};

// **************************************************************
//!
// **************************************************************
template<class T> // T = VBO<X> | VAO  et X=float|vector2/3/4f
class GLAttribute: public IGLVariable
{
public:

  GLAttribute(const char *name, GLenum gltype, GLuint prog)
    : IGLVariable(name, gltype, prog, ATTRIBUTE),
      m_data("VBO_" + std::string(name))
  {
  }

  virtual ~GLAttribute() override { destroy(); }

  inline GLVertexBuffer<T> const& cdata() const { return m_data; }
  inline GLVertexBuffer<T>& data() { return m_data; }

  // Manipule donnee depuis le CPU
  template<class U>
  GLAttribute& operator=(const U& val)
  {
    m_data.qq = T(val);
    forceUpdate();
    return *this;
  }

  template<class U>
  GLAttribute& operator=(std::initializer_list<U> il)
  {
    m_data.qq = il;
    forceUpdate();
    return *this;
  }

private:

  //inline void setValue(const T& value) const;

  virtual bool create() override
  {
    m_handle = glCheck(glGetAttribLocation(m_program, name().c_str()));
    return false;
  }

  virtual void release() override
  {
  }

  virtual void activate() override
  {
    LOGD("Attrib '%s' activate %d", name().c_str(), m_handle);
    m_data.activate();
    glCheck(glEnableVertexAttribArray(m_handle));
    glCheck(glVertexAttribPointer(m_handle,
                                  m_data.qq.size(),
                                  m_gltype,
                                  GL_FALSE,
                                  sizeof (m_data), // stride
                                  (const GLvoid*) 0)); // offset
  }

  virtual void deactivate() override
  {
    LOGD("Attrib '%s' deactivate", name().c_str());
    m_data.deactivate();
    if (isValid() /* && m_data.isVBO() */)
      {
        glCheck(glDisableVertexAttribArray(m_handle));
      }
  }

  virtual bool setup() override
  {
    return false;
  }

  virtual bool update() override
  {
    LOGD("Attrib '%s' update", name().c_str());
    //setValue(m_data);
    m_data.update();
    return false;
  }

private:

  GLVertexBuffer<T> m_data;
};

/*
template<>
inline void GLAttribute<float>::setValue(const float& value) const
{
  glCheck(glVertexAttrib1f(m_handle, value));
}

template<>
inline void GLAttribute<Vector2f>::setValue(const Vector2f& value) const
{
  glCheck(glVertexAttrib2f(m_handle, value.x, value.y));
}

template<>
inline void GLAttribute<Vector3f>::setValue(const Vector3f& value) const
{
  glCheck(glVertexAttrib3f(m_handle, value.x, value.y, value.z));
}

template<>
inline void GLAttribute<Vector4f>::setValue(const Vector4f& value) const
{
  glCheck(glVertexAttrib4f(m_handle, value.x, value.y, value.z, value.w));
  }*/

// **************************************************************
//!
// **************************************************************
template<class T>
class GLUniform: public IGLVariable
{
public:

  GLUniform(const char *name, GLenum gltype, GLuint prog)
    : IGLVariable(name, gltype, prog, UNIFORM)
  {
    // FIXME T et gltype doivent correspondre
  }

  virtual ~GLUniform() override { destroy(); }

  inline T const& data() const { return m_data; }

  // Manipule donnee depuis le CPU
  template<class U>
  GLUniform& operator=(const U& val)
  {
    m_data = T(val);
    forceUpdate();
    return *this;
  }

private:

  inline void setValue(const T& value) const;

  virtual bool create() override
  {
    m_handle = glCheck(glGetUniformLocation(m_program, name().c_str()));
    return false;
  }

  virtual void release() override
  {
  }

  // FIXME: specialization pour Texture
  virtual void activate() override
  {
    LOGD("Uniform '%s' activate", name().c_str());
    switch (m_gltype)
      {
      case GL_SAMPLER_1D:
      case GL_SAMPLER_2D:
      case GL_SAMPLER_CUBE:
        std::cerr << "Texture pas encore geree" << std::endl;
        glCheck(glActiveTexture(GL_TEXTURE0 /*+ m_texture_unit*/));
        //m_data.activate();
        break;
      }
  }

  virtual void deactivate() override
  {
    LOGD("Uniform '%s' deactivate", name().c_str());
  }

  virtual bool setup() override
  {
    return false;
  }

  virtual bool update() override
  {
    LOGD("Uniform '%s' update", name().c_str());
    setValue(m_data);
    return false;
  }

private:

  int m_texture_unit = 0;
  T m_data;
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
//!
// **************************************************************


#endif
