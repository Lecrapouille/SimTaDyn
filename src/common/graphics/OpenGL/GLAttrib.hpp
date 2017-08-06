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

#ifndef GLATTRIB_HPP_
#  define GLATTRIB_HPP_

#  include "GLShader.hpp"

// **************************************************************
//! \brief
// **************************************************************
class GLObjectExceptionNotLocated: public GLObjectException
{
public:

  GLObjectExceptionNotLocated(std::string const& GLObject_name, std::string const& GLShader_name)
  {
    m_msg = "The OpenGL attribute '" + GLObject_name + "' has not been correctly located in the shader program '"
      + GLShader_name + "'";
  }
};

// **************************************************************
//! \brief
// **************************************************************
class GLObjectExceptionNoShader: public GLObjectException
{
public:

  GLObjectExceptionNoShader(std::string const& GLObject_name)
  {
    m_msg = "The OpenGL attribute '" + GLObject_name + "' failed because no shader program was given. "
      "Call setup((GLShader& program, const GLint size, const GLenum type, const GLboolean) before calling"
      "start() method";
  }
};

// **************************************************************
//! \brief
// **************************************************************
class GLAttrib: public GLObject
{
public:

  //! \brief Empty constructor without name
  GLAttrib()
    : GLObject()
  {
    m_handle = (GLenum) -1;
  }

  //! \brief Constructor with the object name
  GLAttrib(std::string const& name)
    : GLObject(name)
  {
    m_handle = (GLenum) -1;
  }

  //! \brief Constructor with the object name
  GLAttrib(const char *name)
    : GLObject(name)
  {
    m_handle = (GLenum) -1;
  }

  //! \brief Destructor: release data from the GPU and CPU memory.
  virtual ~GLAttrib()
  {
    destroy();
  }

  //! \brief Setup attributes
  virtual void setup(GLShader& program, const GLint size, const GLenum type, const GLboolean normalized = GL_FALSE)
  {
    m_program = &program;
    m_normalized = normalized;

    // FIXME: look into the shader code (ie. vec3 ==> size = 3 type =
    // float) and only for attributes and replace param given in the
    // constructor.
    //if ('a' != m_identifier[0]) return true;
    //if (!m_program.getTypeOf(m_identifier.c_str(), &m_size, &m_type)) {
    //  m_handle = (GLenum) -1; return false; }
    m_size = size;
    m_type = type;

    m_need_setup = true;
    //m_need_setup = setup();
  }

  virtual bool isValid() const override
  {
    return ((GLint) m_handle) != -1;
  }

protected:

  virtual bool create() override
  {
    return false;
  }

  virtual void release() override
  {
  }

  virtual void activate() override
  {
    glCheck(glEnableVertexAttribArray((GLint) m_handle));
    glCheck(glVertexAttribPointer((GLint) m_handle, m_size, m_type, m_normalized, 0, nullptr));
  }

  virtual void deactivate() override
  {
    glCheck(glEnableVertexAttribArray(0));
  }

  // Note: VBO needs to be activated before calling this function, then deactivated
  virtual bool setup() override
  {
    if (nullptr == m_program)
      {
        if (m_throw_enable)
          {
            GLObjectExceptionNoShader e(m_name);
            throw e;
          }
        else
          {
            LOGES("GLAttrib named '%s' failed setup because no shader program was given", m_name.c_str());
            return true;
          }
      }

    m_handle = (GLenum) m_program->locateAttrib(m_name.c_str());
    if (isValid() && (0 != m_size) && (0 != m_type))
      {
        activate();
        glCheck(glVertexAttribPointer((GLint) m_handle, m_size, m_type, m_normalized, 0, nullptr));
        //deactivate();

        return false;
      }
    else if (m_throw_enable)
      {
        GLObjectExceptionNotLocated e(m_name, m_program->m_name);
        throw e;
      }
    else
      {
        LOGES("The OpenGL attribute '%s' has not been correctly located in the shader program '%s'",
              m_name.c_str(), m_program->m_name.c_str());
        return true;
      }
  }

  virtual bool update() override
  {
    return false;
  }

protected:

  GLShader *m_program = nullptr;
  GLint m_size = 0;
  GLenum m_type = 0;
  GLboolean m_normalized = false;
};

#endif /* GLATTRIB_HPP_ */
