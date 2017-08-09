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

#include "GLShader.hpp"
#include <vector>
#include <cassert>
#include <fstream>
#include <iostream>

// **************************************************************
//! \param vertex the identifer of the loaded vertex shader.
//! \param fragment the identifer of the loaded fragment shader.
//! \param geometry the identifer of the loaded geometry shader.
// **************************************************************
void GLShader::cleanShader(GLuint vertex, GLuint fragment, GLuint geometry)
{
  if (0U != vertex) {
    glCheck(glDetachShader(m_handle, vertex));
  }
  if (0U != fragment) {
    glCheck(glDetachShader(m_handle, fragment));
  }
  if (0U != geometry) {
    glCheck(glDetachShader(m_handle, geometry));
  }

  if (0U != vertex) {
    glCheck(glDeleteShader(vertex));
  }
  if (0U != fragment) {
    glCheck(glDeleteShader(fragment));
  }
  if (0U != geometry) {
    glCheck(glDeleteShader(geometry));
  }
}

// **************************************************************
//!
// **************************************************************
void GLShader::release()
{
  glCheck(glDeleteProgram(m_handle));
}

// **************************************************************
//! \param obj the identifer of the loaded shader.
//! \return true if case of success, else return false.
// **************************************************************
bool GLShader::checkShaderCompileStatus(GLuint obj)
{
  GLint status;

  glCheck(glGetShaderiv(obj, GL_COMPILE_STATUS, &status));
  if (GL_FALSE == status)
    {
      GLint length;
      glCheck(glGetShaderiv(obj, GL_INFO_LOG_LENGTH, &length));
      std::vector<char> log(length);
      glCheck(glGetShaderInfoLog(obj, length, &length, &log[0]));
      std::cerr << "[FAILED] " << std::endl << &log[0U];
      return false;
    }

  std::cout << "[DONE]" << std::endl;
  return true;
}

// **************************************************************
//! \return true if case of success, else return false.
// **************************************************************
bool GLShader::checkProgramLinkStatus(GLuint obj)
{
  GLint status;

  glCheck(glGetProgramiv(obj, GL_LINK_STATUS, &status));
  if (GL_FALSE == status)
    {
      GLint length;
      glCheck(glGetProgramiv(obj, GL_INFO_LOG_LENGTH, &length));
      std::vector<char> log(length);
      glCheck(glGetProgramInfoLog(obj, length, &length, &log[0U]));
      std::cerr << "[FAILED] " << &log[0U];
      return false;
    }
  std::cout << "[DONE]" << std::endl;
  return true;
}

// **************************************************************
//! @param shader_type GL_VERTEX_SHADE or GL_FRAGMENT_SHADER
//! @param shader_filename the filename of the shader to compile
//! @return 0U if failed. Else return the ID of the compiled shader
// **************************************************************
GLuint GLShader::createShader(int shader_type, const char* shader_filename)
{
  if (nullptr == shader_filename)
    return 0U;

  assert((GL_FRAGMENT_SHADER == shader_type) ||
         (GL_VERTEX_SHADER == shader_type) ||
         (GL_GEOMETRY_SHADER == shader_type));

  std::cout << "  * Compiling ";
  if (GL_FRAGMENT_SHADER == shader_type)
    std::cout << "fragment";
  else if (GL_VERTEX_SHADER == shader_type)
    std::cout << "vertex";
  else
    std::cout << "geometry";

  std::cout << " shader '" << shader_filename << "': ";
  std::string shader_code;
  std::ifstream infile;

  // Read the shader code from a file
  infile.open(shader_filename, std::ifstream::in);
  if (infile.is_open())
    {
      infile.seekg(0, std::ios::end);
      shader_code.resize((unsigned int) infile.tellg());
      infile.seekg(0, std::ios::beg);
      infile.read(&shader_code[0U], shader_code.size());
      infile.close();
    }
  else
    {
      std::cerr << "[FAILED] "
                << "Cannot open this file" << std::endl;
      return 0U;
    }

  // Create and compile the shader
  char const *source = shader_code.c_str();
  int length = shader_code.size();
  GLuint shader = glCheck(glCreateShader(shader_type));
  glCheck(glShaderSource(shader, 1, &source, &length));
  glCheck(glCompileShader(shader));
  if (!checkShaderCompileStatus(shader))
    {
      return 0U;
    }

  return shader;
}

// **************************************************************
//! \param vertex_shader_filename the path of the file containing
//! the vertex shader script.
//! \param fragment_shader_filename the path of the file containing
//! the vertex fragment script.
//! \param geometry_shader_filename the path of the file containing
//! the vertex geometry script.
//! \return the program identifier or 0U if a failure occured.
// **************************************************************
GLuint GLShader::load(const char* vertex_shader_filename,
                      const char* fragment_shader_filename,
                      const char* geometry_shader_filename)
{
  GLuint vertex;
  GLuint fragment;
  GLuint geometry = 0U;

  //LOGI("GLShader named '%s' will load shaders '%s' '%s' '%s'",
  //     m_name.c_str(), vertex_shader_filename,
  //     fragment_shader_filename, geometry_shader_filename);

  release();
  std::cout << "Linking shaders '" << vertex_shader_filename
            << "' with '" << fragment_shader_filename;
  if (nullptr != geometry_shader_filename)
    {
      std::cout << "' and '" << geometry_shader_filename;
    }
  std::cout << "' :" << std::endl;

  vertex = createShader(GL_VERTEX_SHADER, vertex_shader_filename);
  fragment = createShader(GL_FRAGMENT_SHADER, fragment_shader_filename);
  if ((0U == vertex) || (0U == fragment))
    goto l_end;

  m_handle = glCheck(glCreateProgram());
  glCheck(glAttachShader(m_handle, vertex));
  glCheck(glAttachShader(m_handle, fragment));

  // FIXME: je ne suis pas trop sur de ce code
  //glCheck(glBindFragDataLocation(m_handle, 0U, "outColor"));

  // Geometry is optional
  if (nullptr != geometry_shader_filename)
    {
      geometry = createShader(GL_GEOMETRY_SHADER, geometry_shader_filename);
      glCheck(glAttachShader(m_handle, geometry));
    }

  glCheck(glLinkProgram(m_handle));
  checkProgramLinkStatus(m_handle);

l_end:
  cleanShader(vertex, fragment, geometry);
  return m_handle;
}

GLint GLShader::locate(const char *name) const
{
  int res = -1;

  if (nullptr != name)
    {
      // Correct shader variable name shall start either by a_ for
      // attribute or u_ for uniform.
      if (('\0' != name[0U]) && ('_' == name[1U]))
        {
          if (('a' == name[0U]) || ('i' == name[0U]))
            {
              res = glCheck(glGetAttribLocation(m_handle, name));
            }
          else if ('u' == name[0U])
            {
              res = glCheck(glGetUniformLocation(m_handle, name));
            }
        }
    }

  // Failed loaction shader variable
  if (-1 == res)
    {
      if (nullptr != name)
        {
          std::cerr << "[FAILED] getting shader location for '"
                    << name << "'" << std::endl;
        }
      else
        {
          std::cerr << "[FAILED] attribute name shall not be nullptr"
                    << std::endl;
        }
    }

  return res;
}
