#include "Shader.hpp"
#include <vector>
#include <cassert>
#include <fstream>

// **************************************************************
//! \param vertex the identifer of the loaded vertex shader.
//! \param fragment the identifer of the loaded fragment shader.
//! \param geometry the identifer of the loaded geometry shader.
// **************************************************************
void GLShader::cleanShader(GLuint vertex, GLuint fragment, GLuint geometry)
{
  if (0 != vertex)
    glCheck(glDetachShader(m_program, vertex));
  if (0 != fragment)
    glCheck(glDetachShader(m_program, fragment));
  if (0 != geometry)
    glCheck(glDetachShader(m_program, geometry));

  if (0 != vertex)
    glCheck(glDeleteShader(vertex));
  if (0 != fragment)
    glCheck(glDeleteShader(fragment));
  if (0 != geometry)
    glCheck(glDeleteShader(geometry));
}

// **************************************************************
//!
// **************************************************************
void GLShader::cleanProgram()
{
  if (0 != m_program)
    {
      glCheck(glDeleteProgram(m_program));
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

  glCheck(glGetShaderiv(obj, GL_COMPILE_STATUS, &status));
  if (GL_FALSE == status)
    {
      GLint length;
      glCheck(glGetShaderiv(obj, GL_INFO_LOG_LENGTH, &length));
      std::vector<char> log(length);
      glCheck(glGetShaderInfoLog(obj, length, &length, &log[0]));
      std::cerr << "[FAILED] " << std::endl << &log[0];
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
      glCheck(glGetProgramInfoLog(obj, length, &length, &log[0]));
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
      infile.read(&shader_code[0], shader_code.size());
      infile.close();
    }
  else
    {
      std::cerr << "[FAILED] " << std::endl
                << "Cannot open this file" << std::endl;
      return 0;
    }

  // Create and compile the shader
  char const *source = shader_code.c_str();
  int length = shader_code.size();
  GLuint shader = glCheck(glCreateShader(shader_type);)
    glCheck(glShaderSource(shader, 1, &source, &length));
  glCheck(glCompileShader(shader));
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
  GLuint fragment;
  GLuint geometry = 0;

  cleanProgram();
  std::cout << "Linking shaders '" << vertex_shader_filename
            << "' with '" << fragment_shader_filename;
  if (nullptr != geometry_shader_filename)
    {
      std::cout << "' and '" << geometry_shader_filename;
    }
  std::cout << "' :" << std::endl;

  vertex = createShader(GL_VERTEX_SHADER, vertex_shader_filename);
  fragment = createShader(GL_FRAGMENT_SHADER, fragment_shader_filename);
  if ((0 == vertex) || (0 == fragment))
    goto l_end;

  m_program = glCheck(glCreateProgram());
  glCheck(glAttachShader(m_program, vertex));
  glCheck(glAttachShader(m_program, fragment));

  // FIXME: je ne suis pas trop sur de ce code
  glCheck(glBindFragDataLocation(m_program, 0, "outColor"));

  // Geometry is optional
  if (nullptr != geometry_shader_filename)
    {
      geometry = createShader(GL_GEOMETRY_SHADER, geometry_shader_filename);
      glCheck(glAttachShader(m_program, geometry));
    }

  glCheck(glLinkProgram(m_program));
  checkProgramLinkStatus(m_program);

l_end:
  cleanShader(vertex, fragment, geometry);
  return m_program;
}

GLint GLShader::locate(const char *name)
{
  int res = -1;

  // Correct shader variable name shall start either by a_ for
  // attribute or u_ for uniform.
  if ((nullptr != name) && ('\0' != name[0]) && ('_' == name[1]))
    {
      if (('a' == name[0]) || ('i' == name[0]))
        {
          res = glCheck(glGetAttribLocation(m_program, name));
        }
      else if ('u' == name[0])
        {
          res = glCheck(glGetUniformLocation(m_program, name));
        }
    }

  // Failed loaction shader variable
  if (-1 == res)
    {
      std::cerr << "[FAILED] getting shader location for '"
                << name << "'" << std::endl;
    }
  return res;
}
