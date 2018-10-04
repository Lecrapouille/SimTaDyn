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

#ifndef GLSHADER_HPP_
#  define GLSHADER_HPP_

//! \brief This file contains the class managing OpenGL shaders :
//! read, compile, load into the GPU.

#  include "IGLObject.hpp"
#  include "File.hpp"
#  include <vector>

// **************************************************************
//! \class GLShader GLShader.hpp
//! \brief
// **************************************************************
class GLShader: public IGLObject<GLuint>
{
public:

  //! \brief Empty constructor. Do nothing.
  GLShader(const GLenum target) noexcept
    : IGLObject()
  {
    m_target = target;
  }

  GLShader(std::string const& name, const GLenum target) noexcept
    : IGLObject(name)
  {
    m_target = target;
  }

  virtual ~GLShader() override { destroy(); }

  void loadFromString(std::string const& script)
  {
    if (!needSetup())
      {
        LOGE("Failed Shader already compiled");
        //throw OpenGLException("Failed Shader already compiled");
      }

    m_shader_code = script;
    if (name().empty())
      {
        name() = "shader script";
      }
  }

  void loadFromFile(std::string const& filename)
  {
    if (!needSetup())
      {
        LOGE("Failed Shader already compiled");
        //throw OpenGLException("Failed Shader already compiled");
      }

    std::ifstream infile;
    bool res = false;

    // Read the shader code from a file
    infile.open(filename, std::ifstream::in);
    if (infile.is_open())
      {
        infile.seekg(0, std::ios::end);
        std::streampos pos = infile.tellg();
        if (pos > 0)
          {
            m_shader_code.resize(static_cast<size_t>(pos));
            infile.seekg(0, std::ios::beg);
            infile.read(&m_shader_code[0U],
                        static_cast<std::streamsize>(m_shader_code.size()));
            res = true;
            if (name().empty())
              {
                name() = File::fileName(filename);
              }
          }
        infile.close();
      }

    if (!res)
      {
        LOGE("Failed open file '%s'", filename.c_str());
        //throw OpenGLException("Failed open file '" + filename + "'");
      }
    else
      {
        LOGI("Shader file '%s' loaded", filename.c_str());
      }
  }

  inline bool loaded() const
  {
    return !m_shader_code.empty();
  }

  inline bool compiled() const
  {
    return !needSetup();
  }

  inline std::string const& code() const
  {
    return m_shader_code;
  }

private:

  virtual bool create() override
  {
    LOGD("%s: create", name().c_str());
    m_handle = glCheck(glCreateShader(m_target));
    LOGD("handle = %u", m_handle);
    return false;
  }

  virtual void release() override
  {
    LOGD("%s: release", name().c_str());
    glCheck(glDeleteShader(m_handle));
  }

  //! \brief Empty method.
  virtual void activate() override
  {
    //LOGD("%s: activate", name().c_str());
  }

  //! \brief Empty method.
  virtual void deactivate() override
  {
    //LOGD("%s: deactivate", name().c_str());
  }

  //! \brief Empty method.
  virtual bool setup() override
  {
    LOGD("%s: setup", name().c_str());
    // Nothing to compile ? Try again later
    if (m_shader_code.empty())
      {
        LOGD("Shader code source empty");
        return true;
      }

    GLint status;
    char const *source = m_shader_code.c_str();
    GLint length = static_cast<GLint>(m_shader_code.size());
    glCheck(glShaderSource(m_handle, 1, &source, &length));
    glCheck(glCompileShader(m_handle));
    glCheck(glGetShaderiv(m_handle, GL_COMPILE_STATUS, &status));
    if (GL_FALSE == status)
      {
        glCheck(glGetShaderiv(m_handle, GL_INFO_LOG_LENGTH, &length));
        std::vector<char> log(static_cast<size_t>(length));
        glCheck(glGetShaderInfoLog(m_handle, length, &length, &log[0]));
        LOGE("[FAILED] %s\n", &log[0U]);
        // In case of failure, let the ability to load
        // a new script
        m_shader_code.clear();
        return true;
      }
    return false;
  }

  virtual bool update() override
  {
    LOGD("%s: update", name().c_str());
    return false;
    //FIXME return needSetup ? true : false;
  }

private:

  std::string m_shader_code;
};

// **************************************************************
//!
// **************************************************************
class GLVertexShader: public GLShader
{
public:

  GLVertexShader()
    : GLShader(GL_VERTEX_SHADER)
  {
  }

  GLVertexShader(std::string const& name)
    : GLShader(name, GL_VERTEX_SHADER)
  {
  }
};

// **************************************************************
//!
// **************************************************************
class GLFragmentShader: public GLShader
{
public:

  GLFragmentShader()
    : GLShader(GL_FRAGMENT_SHADER)
  {
  }

  GLFragmentShader(std::string const& name)
    : GLShader(name, GL_FRAGMENT_SHADER)
  {
  }
};

// **************************************************************
//!
// **************************************************************
class GLGeometryShader: public GLShader
{
public:

  GLGeometryShader()
    : GLShader(GL_GEOMETRY_SHADER)
  {
  }

  GLGeometryShader(std::string const& name)
    : GLShader(name, GL_GEOMETRY_SHADER)
  {
  }
};

#endif /* GLSHADER_HPP_ */
