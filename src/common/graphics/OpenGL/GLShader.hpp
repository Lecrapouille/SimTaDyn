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

#ifndef GLSHADER_HPP_
#  define GLSHADER_HPP_

///! \brief This file contains the class managing OpenGL shaders :
///! read, compile, load into the GPU.

#  include "GLObject.hpp"

// **************************************************************
//!
// **************************************************************
class GLShaderNotLoadedException: public GLObjectException
{
public:

  GLShaderNotLoadedException(std::string const& name)
  {
    m_msg = "No vertex and fragment shaders have been given for the GLShader '" + name + "'";
  }
};

// **************************************************************
//! \class GLShader is a class for managing OpenGL shader scripts
//! (like loading them into the GPU).
//!
//! Shaders are scripts running in the GPU manipulating vertices,
//! pixels and geometry. There are respectively named vertex shader,
//! fragment shader and geometry shader. Once loaded in the GPU they
//! are attached into a single program identifier. With OpenGL >= 3.3
//! shaders are needed for displaying an OpenGL scene.
//!
//! This class loads into the GPU a pair of vertex and fragment shader
//! and optionaly a geometry shader.
// **************************************************************
class GLShader: public GLObject
{
public:

  //! \brief Empty constructor. Do nothing.
  GLShader()
    : GLObject()
  {
  }

  //! \brief Constructor with the object name
  GLShader(std::string const& name)
    : GLObject(name)
  {
  }

  //! \brief Constructor with the object name
  GLShader(const char *name)
    : GLObject(name)
  {
  }

  //! \brief Destructor: release data from the GPU and CPU memory.
  virtual ~GLShader()
  {
    destroy();
  }

protected:

  //! \brief Ideally would call the load() method but not possible due
  //! to parameters.
  virtual bool create() override
  {
    if (!isValid())
      {
        if (m_throw_enable)
          {
            GLShaderNotLoadedException e(m_name);
            throw e;
          }
        return true;
      }
    return false;
  }

  //! \brief Empty method.
  virtual inline bool setup() override
  {
    return false;
  }

  //! \brief Empty method.
  virtual inline bool update() override
  {
    return false;
  }

  //! \brief Once program is no longer used, release it from the GPU
  //! memory. Can be used to abort the shader.
  virtual void release() override;

  //! \brief A program can be activated if and only if shaders have
  //! been loaded into a program (else nothing is done).
  virtual inline void activate() override
  {
    glUseProgram(m_handle);
  }

  //! \brief A program can be desactivated if and only if shaders have
  //! been loaded into a program (else nothing is done).
  virtual inline void deactivate() override
  {
    glUseProgram(0U);
  }

public:

  //! \brief Accessor. Return the program identifier. Return 0 if the
  //! shaders have not been loaded intot the GPU.
  inline GLuint program() const
  {
    return m_handle;
  }

  //! \brief Open, read, compile and load a fragment, a vertex shader
  //! and optionaly a geometry shader an link them as a program. The
  //! older program, if present, is released from GPU memory before
  //! being replacing by the new one.
  //! \param vertex_shader_filename the path of the file containing
  //! the vertex shader script.
  //! \param fragement_shader_filename the path of the file containing
  //! the vertex fragment script.
  //! \param geometry_shader_filename the path of the file containing
  //! the vertex geometry script.
  inline GLuint load(std::string const& vertex_shader_filename,
                     std::string const& fragment_shader_filename,
                     std::string const& geometry_shader_filename)
  {
    return load(vertex_shader_filename.c_str(),
                fragment_shader_filename.c_str(),
                geometry_shader_filename.c_str());
  }

  inline GLuint load(std::string const& vertex_shader_filename,
                     std::string const& fragment_shader_filename)
  {
    return load(vertex_shader_filename.c_str(),
                fragment_shader_filename.c_str(),
                nullptr);
  }

  //! \brief Open, read, compile and load a fragment, a vertex shader
  //! and optionaly a geometry shader an link them as a program. The
  //! older program is released from GPU memory before replacing by
  //! the new one.
  GLuint load(const char* vertex_shader_filename,
              const char* fragment_shader_filename,
              const char* geometry_shader_filename = nullptr);

  //! \brief Find in the shader program a variable name.
  //! \param name the attribute or uniform shader variable to look for.
  //! \return the id of the variable or -1 if not found or the given name
  //! is erroneous.
  GLint locate(const char *name) const;
  inline GLint locateAttrib(const char *name) const
  {
    assert(nullptr != name);
    GLint res = glCheck(glGetAttribLocation(m_handle, name));
    return res;
  }

private:

  //! \brief Check and display for shader compiler errors.
  bool checkShaderCompileStatus(GLuint obj);

  //! \brief Check and display for shader linker error.
  bool checkProgramLinkStatus(GLuint obj);

  //! \brief Read a shader script file, compile it and load it in the
  //! GPU memory.
  GLuint createShader(int shader_type, const char* shader_filename);

  //! \brief Once shaders are loaded as unique program, release the
  //! GPU memory.
  void cleanShader(GLuint vertex, GLuint fragment, GLuint geometry);
};

#endif /* SHADERHPP_ */
