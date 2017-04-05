#ifndef GLSHADER_HPP_
#  define GLSHADER_HPP_

///! \brief This file contains the class managing OpenGL shaders :
///! read, compile, load into the GPU.

#  include "Exception.hpp"
#  include "GLObject.hpp"

//! This macro (from the library POCO) will declare a class
//! MapLoaderException derived from simtadyn::Exception.
POCO_DECLARE_EXCEPTION(GLShaderException, simtadyn::Exception)

// **************************************************************
//!
// **************************************************************
class GLShaderNotLoadedException: public GLShaderException
{
public:

  GLShaderNotLoadedException(const uint32_t id)
  {
    m_msg = "No vertex and fragment shaders have been given for GLObject "
      + std::to_string(id);
  }
};

// **************************************************************
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
// **************************************************************
class GLShader: public GLObject
{
public:

  //! \brief Empty constructor. Do nothing.
  GLShader()
  {
  }

  //! \brief Constructor with the path of shader scripts. Compile and
  //! load them into the GPU as a program (load() is
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
    load(vertex_shader_filename, fragment_shader_filename,
         geometry_shader_filename);
  }

  //! \brief Destructor. The program (if loaded) is removed from the GPU.
  ~GLShader()
  {
    destroy();
  }

  virtual void create() override
  {
    if (0U == m_handle)
      {
        GLShaderNotLoadedException e(cpuID());
        throw e;
      }
  }

  virtual inline void setup() override
  {
  }

  virtual inline void update() override
  {
  }

  //! \brief Once program is no longer used, release it from the GPU
  //! memory. Can be used to abort the shader.
  virtual void release() override;

  //! \brief A program can be activated if and only if shaders have
  //! been loaded into a program (else nothing is done).
  virtual inline void activate() override
  {
    if (0U != m_handle)
      {
        glUseProgram(m_handle);
      }
  }

  //! \brief A program can be desactivated if and only if shaders have
  //! been loaded into a program (else nothing is done).
  virtual inline void deactivate() override
  {
    glUseProgram(0U);
  }

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
};

#endif /* SHADERHPP_ */
