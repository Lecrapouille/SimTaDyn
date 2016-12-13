#include "OpenGL.hpp"
#include "simtypes.hpp"

namespace simGL
{
  // *************************************************************************************************
  // Open, read and compile a fragment or vertex shader
  // @param shader_type GL_VERTEX_SHADE or GL_FRAGMENT_SHADER
  // @param shader_file_path the filename of the shader to compile
  // @return 0 if failed. Else return the ID of the compiled shader
  // *************************************************************************************************
  static GLuint createShader(int shader_type, const std::string& shader_file_path)
  {
    if ((GL_FRAGMENT_SHADER != shader_type) && (GL_VERTEX_SHADER != shader_type))
      {
        std::cerr << "[FAILED] incorrect type of shader (shall be GL_FRAGMENT_SHADER or GL_VERTEX_SHADER)" << std::endl;
        return 0;
      }

    std::string whoami(shader_file_path); // = (shader_type == GL_VERTEX_SHADER ? "vertex" : "fragment")
      //+ " shader file '" + std::string(shader_file_path) + "'";

    std::cout << "  * Compiling " << whoami << ": ";
    std::string shader_code;
    std::ifstream infile;

    // Read the Shader code from a file
    infile.open(shader_file_path, std::ifstream::in);
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

    // Compile shader
    char const *vertex_source_pointer = shader_code.c_str();
    GLuint shader = glCreateShader(shader_type);
    glShaderSource(shader, 1, &vertex_source_pointer, nullptr);
    glCompileShader(shader);

    int status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE)
      {
        int log_len;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_len);

        std::string log_space(log_len + 1, ' ');
        glGetShaderInfoLog(shader, log_len, nullptr, (GLchar*) log_space.c_str());

        std::cerr << "[FAILED] " << log_space << std::endl;
        glDeleteShader(shader);
        return 0;
      }

    std::cout << "[DONE]" << std::endl;
    return shader;
  }

  // *************************************************************************************************
  //
  // *************************************************************************************************
  GLuint createShaderProgram(const std::string& vertex_shader_file_path, const std::string& fragment_shader_file_path)
  {
    GLuint program = 0;
    GLuint fragment, vertex;
    std::string log_space = "";

    std::cout << "Linking shaders '" << vertex_shader_file_path
              << "' with '" << fragment_shader_file_path << "':"
              << std::endl;

    vertex = simGL::createShader(GL_VERTEX_SHADER, vertex_shader_file_path);
    if (0 == vertex)
      goto l_error;

    fragment = simGL::createShader(GL_FRAGMENT_SHADER, fragment_shader_file_path);
    if (0 == fragment)
      {
        glDeleteShader(vertex);
        goto l_error;
      }

    program = glCreateProgram();
    glAttachShader(program, vertex);
    glAttachShader(program, fragment);
    glLinkProgram(program);

    int status;
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if (GL_FALSE == status)
      {
        int log_len;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_len);

        log_space.reserve(log_len + 1);
        glGetProgramInfoLog(program, log_len, nullptr, (GLchar*) log_space.c_str());
        log_space = ' ' + log_space;

        glDeleteProgram(program);
        program = 0;
      }

    glDetachShader(program, vertex);
    glDetachShader(program, fragment);
    glDeleteShader(vertex);
    glDeleteShader(fragment);

l_error:
    if (0 == program)
      {
        std::cerr << "[FAILED]" << log_space << std::endl;
      }
    else
      {
        std::cout << "[DONE]" << std::endl;
      }

    return program;
  }
} // namespace simGL
