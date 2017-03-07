#ifndef SHADER_HPP_
#  define SHADER_HPP_

///! \brief This file contains the class managing OpenGL shaders :
///! read, compile, load into the GPU.

#  include "OpenGL.hpp"

//! \class GLShader
//!
//! Shaders are scripts running in the GPU manipulating vertices,
//! pixels and geometry. There are respectively named vertex shader,
//! fragment shader and geometry shader. Once loaded in the GPU they
//! are attached into a single program identifier. With OpenGL >= 3.3
//! shaders are needed for displaying an OpenGL scene.
//!
//! This class loads into the GPU a pair of vertex and fragment shader
//! and optionaly a geometry shader.
class GLShader
{
public:

  //! \brief Empty constructor. Do nothing.
  GLShader()
  {
  }

  //! \brief Constructor with the path of shader scripts. Compile and
  //! load them into the GPU as a program (createShaderProgram() is
  //! called).
  //! \param vertex_shader_filename the path of the file containing
  //! the vertex shader script.
  //! \param fragement_shader_filename the path of the file containing
  //! the vertex fragment script.
  //! \param geometry_shader_filename the path of the file containing
  //! the vertex geometry script.
  GLShader(const char* vertex_shader_filename,
           const char* fragment_shader_filename,
           const char* geometry_shader_filename = nullptr)
  {
    m_program = createShaderProgram(vertex_shader_filename,
                                    fragment_shader_filename,
                                    geometry_shader_filename);
  }

  //! \brief Destructor. The program (if loaded) is removed from the GPU.
  ~GLShader()
  {
    abort();
  }

  //! \brief Once program is no longer used, release it from the GPU
  //! memory. Can be used to abort the shader.
  void abort();

  //! \brief A program can be activated if and only if shaders have
  //! been loaded into a program (else nothing is done).
  inline void on() const
  {
    if (0 != m_program)
      {
        glUseProgram(m_program);
      }
  }

  //! \brief A program can be desactivated if and only if shaders have
  //! been loaded into a program (else nothing is done).
  inline void off() const
  {
    glUseProgram(0);
  }

  //! \brief Accessor. Return the program identifier. Return 0 if the
  //! shaders have not been loaded intot the GPU.
  inline GLuint id() const
  {
    return m_program;
  }

  //! \brief Open, read, compile and load a fragment, a vertex shader
  //! and optionaly a geometry shader an link them as a program. The
  //! older program, if present, is released from GPU memory before
  //! being replacing by the new one.
  inline GLuint createShaderProgram(std::string const& vertex_shader_filename,
                                    std::string const& fragment_shader_filename,
                                    std::string const& geometry_shader_filename)
  {
    return createShaderProgram(vertex_shader_filename.c_str(),
                               fragment_shader_filename.c_str(),
                               geometry_shader_filename.c_str());
  }

  inline GLuint createShaderProgram(std::string const& vertex_shader_filename,
                                    std::string const& fragment_shader_filename)
  {
    return createShaderProgram(vertex_shader_filename.c_str(),
                               fragment_shader_filename.c_str(),
                               nullptr);
  }

  //! \brief Open, read, compile and load a fragment, a vertex shader
  //! and optionaly a geometry shader an link them as a program. The
  //! older program is released from GPU memory before replacing by
  //! the new one.
  GLuint createShaderProgram(const char* vertex_shader_filename,
                             const char* fragment_shader_filename,
                             const char* geometry_shader_filename = nullptr);

  //! \brief Find in the shader program a variable name.
  //! \param name the attribute or uniform shader variable to look for.
  //! \return the id of the variable or -1 if not found or the given name
  //! is erroneous.
  GLint locate(const char *name);

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

  //! \brief the program identifier.
  GLuint m_program = 0;
};

#endif /* SHADERHPP_ */
