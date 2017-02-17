#include "Shader.hpp"
#include "simtypes.hpp"
#include <assert.h>

// **************************************************************
//! \param vertex the identifer of the loaded vertex shader.
//! \param fragment the identifer of the loaded fragment shader.
//! \param geometry the identifer of the loaded geometry shader.
// **************************************************************
void GLShader::cleanShader(GLuint vertex, GLuint fragment, GLuint geometry)
{
  glDetachShader(m_program, vertex);
  glDetachShader(m_program, fragment);
  glDetachShader(m_program, geometry);

  glDeleteShader(vertex);
  glDeleteShader(fragment);
  glDeleteShader(geometry);
}

// **************************************************************
//!
// **************************************************************
void GLShader::cleanProgram()
{
  if (0 != m_program)
    {
      glDeleteProgram(m_program);
      m_program = 0;
    }
}

// **************************************************************
//! \param obj the identifer of the loaded shader.
//! \return true if case of success, else return false.
// **************************************************************
bool GLShader::checkShaderCompileStatus(GLuint obj)
{
  GLint status;

  glGetShaderiv(obj, GL_COMPILE_STATUS, &status);
  if (GL_FALSE == status)
    {
      GLint length;
      glGetShaderiv(obj, GL_INFO_LOG_LENGTH, &length);
      std::vector<char> log(length);
      glGetShaderInfoLog(obj, length, &length, &log[0]);
      std::cerr << "[FAILED] " << &log[0];
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

  glGetProgramiv(obj, GL_LINK_STATUS, &status);
  if (GL_FALSE == status)
    {
      GLint length;
      glGetProgramiv(obj, GL_INFO_LOG_LENGTH, &length);
      std::vector<char> log(length);
      glGetProgramInfoLog(obj, length, &length, &log[0]);
      std::cerr << "[FAILED] " << &log[0];
      return false;
    }
  std::cout << "[DONE]" << std::endl;
  return true;
}

// **************************************************************
//! @param shader_type GL_VERTEX_SHADE or GL_FRAGMENT_SHADER
//! @param shader_filename the filename of the shader to compile
//! @return 0 if failed. Else return the ID of the compiled shader
// **************************************************************
GLuint GLShader::createShader(int shader_type, const char* shader_filename)
{
  if (nullptr == shader_filename)
    return 0;

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

  std::cout << " shader '" << shader_filename << "':" << std::endl;
  std::string shader_code;
  std::ifstream infile;

  // Read the shader code from a file
  infile.open(shader_filename, std::ifstream::in);
  if (infile.is_open())
    {
      infile.seekg(0, std::ios::end);
      shader_code.resize((unsigned int) infile.tellg());
      infile.seekg(0, std::ios::beg);
      infile.read(&shader_code[0], shader_code.size());
      infile.close();
    }
  else
    {
      std::cerr << "[FAILED] Cannot open this file" << std::endl;
      return 0;
    }

  // Create and compile the shader
  char const *source = shader_code.c_str();
  int length = shader_code.size();
  GLuint shader = glCreateShader(shader_type);
  glShaderSource(shader, 1, &source, &length);
  glCompileShader(shader);
  if (!checkShaderCompileStatus(shader))
    {
      return 0;
    }

  return shader;
}

// **************************************************************
//! \param vertex_shader_filename the path of the file containing
//! the vertex shader script.
//! \param fragement_shader_filename the path of the file containing
//! the vertex fragment script.
//! \param geometry_shader_filename the path of the file containing
//! the vertex geometry script.
//! \return the program identifier or 0 if a failure occured.
// **************************************************************
GLuint GLShader::createShaderProgram(const char* vertex_shader_filename,
                                     const char* fragment_shader_filename,
                                     const char* geometry_shader_filename)
{
  GLuint vertex;
  GLuint fragment = 0;
  GLuint geometry = 0;

  cleanProgram();
  std::cout << "Linking shaders '" << vertex_shader_filename
            << "' with '" << fragment_shader_filename
            << "and '" << geometry_shader_filename
            << "':" << std::endl;

  vertex = createShader(GL_VERTEX_SHADER, vertex_shader_filename);
  fragment = createShader(GL_FRAGMENT_SHADER, fragment_shader_filename);
  if ((0 == vertex) || (0 == fragment))
    goto l_end;

  m_program = glCreateProgram();
  glAttachShader(m_program, vertex);
  glAttachShader(m_program, fragment);

  // FIXME: je ne suis pas trop sur de ce code
  glBindFragDataLocation(m_program, 0, "outColor");

  // Geometry is optional
  if (nullptr != geometry_shader_filename)
    {
      geometry = createShader(GL_GEOMETRY_SHADER, geometry_shader_filename);
      glAttachShader(m_program, geometry);
    }

  glLinkProgram(m_program);
  checkProgramLinkStatus(m_program);

l_end:
  cleanShader(vertex, fragment, geometry);
  return m_program;
}
