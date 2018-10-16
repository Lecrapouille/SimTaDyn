#ifndef GLPROGRAM_HPP_
#  define GLPROGRAM_HPP_

//! \brief This file contains the class managing OpenGL shaders :
//! read, compile, load into the GPU.

#  include "NonCppStd.hpp"
#  include "GLShaders.hpp"
#  include "GLVariables.tpp"
#  include <map>

// **************************************************************
//! \class GLShader GLShader.hpp
//! \brief
// **************************************************************
class GLProgram: public IGLObject<GLenum>
{
public:

  //! \brief Empty constructor. Do nothing.
  GLProgram(std::string const& name)
    : IGLObject(name), m_vao("VAO_" + name)
  {
  }

  virtual ~GLProgram() override { destroy(); }

  GLProgram& attachShader(GLVertexShader& shader)
  {
    if (!needSetup())
      {
        LOGE("Failed Shader already compiled");
        //throw OpenGLException("Failed Shader already compiled");
      }
    m_vertex_shader = std::move(shader);
    //m_vertex_shader.begin(); FIXME: A faire ?
    std::cout << "P::VS: " << m_vertex_shader.gpuID() << std::endl;
    return *this;
  }

  GLProgram& attachShader(GLFragmentShader& shader)
  {
    if (!needSetup())
      {
        LOGE("Failed Shader already compiled");
        //throw OpenGLException("Failed Shader already compiled");
      }
    m_fragment_shader = std::move(shader);
    return *this;
  }

  GLProgram& attachShader(GLGeometryShader& shader)
  {
    if (!needSetup())
      {
        LOGE("Failed Shader already compiled");
        //throw OpenGLException("Failed Shader already compiled");
      }
    m_geometry_shader = std::move(shader);
    return *this;
  }

  inline bool compiled() const
  {
    return !needSetup();
  }

  inline bool hasUniform(const char *name) const
  {
    auto const& it = m_variables.find(name);
    return (m_variables.end() != it) &&
      (IGLVariable::UNIFORM == it->second->kind());
  }

  template<class T>
  inline GLUniform<T>& uniform(const char *name)
  {
    return *dynamic_cast<GLUniform<T>*>(m_variables[name]);
  }

  template<class T>
  inline T const& getUniformVal(const char *name)
  {
    return dynamic_cast<GLUniform<T>*>(m_variables[name])->data();
  }

  template<class T>
  void setUniformVal(const char *name, T const& val)
  {
    *dynamic_cast<GLUniform<T>*>(m_variables[name]) = val;
  }

  // FIXME if begin() a reussi
  // FIXME: peut on avoir un nom doublon attrib / unifo ??
  inline bool hasAttribute(const char *name) const
  {
    auto const& it = m_variables.find(name);
    return (m_variables.end() != it) &&
      (IGLVariable::ATTRIBUTE == it->second->kind());
  }

  template<class T>
  inline GLAttribute<T>& attribute(const char *name) // FIXME: Retourner le pointer pas la reference (si le prog n'a pas ete charge avec success)
  {
    auto* p = dynamic_cast<GLAttribute<T>*>(m_variables[name]);
    printf("Get attib %p\n", p);
    if (nullptr == p)
      {
        throw std::out_of_range("Name '" + std::string(name) +
                                "' is not an valid shader variable (or your failed with its template)");
      }
    return *p;
  }

  template<class T>
  inline T& getAttribVal(const char *name)
  {
    return dynamic_cast<GLAttribute<T>*>(m_variables[name])->data();
  }

  template<class T>
  void setAttribVal(const char *name, T const& val)
  {
    *dynamic_cast<GLAttribute<T>*>(m_variables[name]) = val;
  }

  // TODO GLProgram prog; prog['position'] = std::vector(...);
  inline IGLVariable& operator[](std::string const& name)
  {
    if (m_variables.end() == m_variables.find(name))
      {
        throw std::out_of_range("Name '" + name + "' is not an shader variable");
      }
    return *m_variables[name];
  }

  /*inline IGLVariable const& operator[](std::string const& name) const
    {
    if (m_variables.end() == m_variables.find(name))
    {
    throw std::out_of_range("Name '" + name + "' is not an shader variable");
    }
    return *m_variables[name];
    }*/

  // TODO function qui verifie que tous les attrib ont la meme taille et ont ete init
  /*bool
    size_t size = 0_z;
    for (const auto& it = m_variables)
    {
       if (GLVariable::ATTRIBUTE == it->second->kind())
       {
          size =
       }
    }
*/

// FIXME: ca prend 43 appels OpenGL alors qu'avant il suffisait de 35
  inline void draw(GLenum mode, GLint first, GLsizei count)
  {
    LOGD("%s: draw {", name().c_str());
    m_vao.begin(); // FIXME
    begin();
    glCheck(glDrawArrays(mode, first, count));
    glCheck(glBindBuffer(GL_ARRAY_BUFFER, 0));
    end();
    m_vao.end();
    LOGD("} %s: draw", name().c_str());
  }

  /*inline void draw(GLenum mode)
  {
    draw(mode, 0, getAttribSize());
  }*/

/*
    inline void draw(GLenum mode, GLIndexBuffer index)
    {
    begin();
    index.begin();
    glCheck(glDrawElements(mode, indices.size(), index.type(), 0));
    index.end();
    glCheck(glBindBuffer(GL_ARRAY_BUFFER, 0));
    end();
    }*/

protected:

  virtual bool create() override
  {
    LOGD("%s: create", name().c_str());
    m_handle = glCheck(glCreateProgram());
    return false;
  }

  //! \brief A program can be activated if and only if shaders have
  //! been loaded into a program (else nothing is done).
  virtual void activate() override
  {
    LOGD("%s: activate {", name().c_str());
    if (!needSetup()) // FIXME or call glUseProgram(0) is setup()
      {
        //m_vao.begin();
        glCheck(glUseProgram(m_handle));
        for (auto& it: m_variables)
          {
            it.second->begin();
          }
      }
    else
      {
        LOGD("%s: failed activate: need setup", name().c_str());
      }
    LOGD("} %s: activate", name().c_str());
  }

  virtual bool setup() override
  {
    LOGD("%s: setup {", name().c_str());
    // --- Check if we have scripts of vertex and frag shaders
    if (!m_vertex_shader.loaded())
      {
        LOGE("No vertex shader has been given");
        return true;
      }

    if (!m_fragment_shader.loaded())
      {
        LOGE("No Fragment shader has been given");
        return true;
      }

    // --- Compile and attach vertex and frag shaders
    LOGD("Compile and attach vertex and frag shaders");
    m_vertex_shader.begin();
    m_fragment_shader.begin();

    if (!m_vertex_shader.compiled() || !m_fragment_shader.compiled())
      {
        LOGE("Program shader failed compiling at least one shader");
        goto l_try_again;
      }
    glCheck(glAttachShader(m_handle, m_vertex_shader.gpuID()));
    glCheck(glAttachShader(m_handle, m_fragment_shader.gpuID()));

    // --- Compile and attach geometry shader
    if (m_geometry_shader.loaded())
      {
        m_geometry_shader.begin();
        if (!m_geometry_shader.compiled())
          {
            LOGE("Program shader failed compiling at least one shader");
            goto l_try_again;
          }
        glCheck(glAttachShader(m_handle, m_geometry_shader.gpuID()));
      }

    // --- Link shaders into a single program
    LOGD("Link shaders into a single program");
    GLint status;
    glCheck(glLinkProgram(m_handle));
    glCheck(glGetProgramiv(m_handle, GL_LINK_STATUS, &status));
    if (GL_FALSE == status)
      {
        GLint length;
        glCheck(glGetShaderiv(m_handle, GL_INFO_LOG_LENGTH, &length));
        std::vector<char> log(static_cast<size_t>(length));
        glCheck(glGetShaderInfoLog(m_handle, length, &length, &log[0]));
        LOGE("[FAILED] %s\n", &log[0U]); // FIXME: si program echoue alors acceder aux attrib va faire planter le prog
        goto l_try_again;
      }

    // TODO: glValidateProgram(m_handle);

    // --- Construct list of uniforms and attributes
    LOGD("Index attrib and uniform variables");
    indexVariables();

    // --- Detach shaders
    LOGD("Detach shaders");
    detachAllShaders();
    LOGD("} %s: setup", name().c_str());
    return false;

  l_try_again:
    m_vertex_shader.destroy();
    m_fragment_shader.destroy();
    m_geometry_shader.destroy();
    return true;
  }

  virtual bool update() override
  {
    LOGD("%s: update", name().c_str());
    return false;
  }

  //! \brief A program can be desactivated if and only if shaders have
  //! been loaded into a program (else nothing is done).
  virtual void deactivate() override
  {
    LOGD("%s: deactivate {", name().c_str());
    glCheck(glUseProgram(0U));
    for (auto& it: m_variables)
      {
        it.second->end();
      }

    //m_vao.end();
    LOGD("} %s: deactivate", name().c_str());
  }

  //! \brief Once program is no longer used, release it from the GPU
  //! memory. Can be used to abort the shader.
  virtual void release() override
  {
    LOGD("%s: release", name().c_str());
    //FIXME detachAllShaders();
    glCheck(glDeleteProgram(m_handle));
  }

private:

  void detachAllShaders()
  {
    LOGD("%s: detach all shaders", name().c_str());
    if (0 < m_vertex_shader.gpuID()) {
      glCheck(glDetachShader(m_handle, m_vertex_shader.gpuID()));
    }

    if (0 < m_fragment_shader.gpuID()) {
      glCheck(glDetachShader(m_handle, m_fragment_shader.gpuID()));
    }

    if (0 < m_geometry_shader.gpuID()) {
      glCheck(glDetachShader(m_handle, m_geometry_shader.gpuID()));
    }
  }

  // shader = VertexShader("uniform float color[2];")
  //   ==> "color[0]", gl.GL_FLOAT
  //   ==> "color[1]", gl.GL_FLOAT
  // "uniform vec4 color;" ==> "color", gl.GL_FLOAT_VEC4
  uint32_t indexVariables()
  {
    if (likely(m_indexed))
      return m_total;

    GLsizei length = 0;
    const GLsizei bufSize = 16;
    GLint size = 0;
    GLint count = 0;
    GLchar name[bufSize];
    GLenum type;
    IGLVariable* glvariable;

    glCheck(glGetProgramiv(m_handle, GL_ACTIVE_UNIFORMS, &count));
    m_total = count;
    LOGD("Number of Uniforms: %d", count);
    uint32_t texture_count = 0;
    for (GLint i = 0; i < count; ++i)
      {
        glCheck(glGetActiveUniform(m_handle, (GLuint)i, bufSize, &length, &size, &type, name));
        LOGD("Uniform #%d Type: %u Name: %s L:%d S:%d", i, type, name, size, length);
        if (GL_FLOAT == type) // FIXME leak
          glvariable = new GLUniform<float>(name, type, gpuID());
        else if (GL_FLOAT_VEC2 == type)
          glvariable = new GLUniform<Vector2f>(name, type, gpuID());
        else if (GL_FLOAT_VEC3 == type)
          glvariable = new GLUniform<Vector3f>(name, type, gpuID());
        else if (GL_FLOAT_VEC4 == type)
          glvariable = new GLUniform<Vector4f>(name, type, gpuID());
        else if (GL_INT == type)
          glvariable = new GLUniform<int>(name, type, gpuID());
        else if (GL_INT_VEC2 == type)
          glvariable = new GLUniform<Vector2i>(name, type, gpuID());
        else if (GL_INT_VEC3 == type)
          glvariable = new GLUniform<Vector3i>(name, type, gpuID());
        else if (GL_INT_VEC4 == type)
          glvariable = new GLUniform<Vector4i>(name, type, gpuID());
        //else if (GL_BOOL == type)
        //  glvariable = new GLUniform<bool>(name, type, gpuID());
        //else if (GL_BOOL_VEC2 == type)
        //  glvariable = new GLUniform<Vector2b>(name, type, gpuID());
        else if (GL_FLOAT_MAT2 == type)
          glvariable = new GLUniform<Matrix22f>(name, type, gpuID());
        else if (GL_FLOAT_MAT3 == type)
          glvariable = new GLUniform<Matrix33f>(name, type, gpuID());
        else if (GL_FLOAT_MAT4 == type)
          glvariable = new GLUniform<Matrix44f>(name, type, gpuID());
        else if ((GL_SAMPLER_1D == type) || (GL_SAMPLER_2D == type) || (GL_SAMPLER_CUBE == type))
          {
            GLUniform</*GLTexture2D*/float>* gluni = new GLUniform</*GLTexture2D*/float>(name, type, gpuID());
            gluni->m_texture_unit = texture_count;
            texture_count += 1u;
            glvariable = gluni;
          }
        else
          {
            LOGES("Uniform %s: type not managed", name);
            glvariable = nullptr;
          }

        if (nullptr != glvariable)
          {
            m_variables[name] = glvariable;
            //glvariable->begin();
          }
      }

    //Ajputer m_variables[name].begin();

    glCheck(glGetProgramiv(m_handle, GL_ACTIVE_ATTRIBUTES, &count));
    LOGD("Number of Attributes: %d", count);
    m_total += count;
    for (GLint i = 0; i < count; ++i)
      {
        glCheck(glGetActiveAttrib(m_handle, (GLuint)i, bufSize, &length, &size, &type, name));
        LOGD("Attribute #%d Type: %u Name: %s L:%d S:%d", i, type, name, size, length);
        if (GL_FLOAT == type) // FIXME leak
          {
            glvariable = new GLAttribute<float>(name, 1u, GL_FLOAT, gpuID());
          }
        else if (GL_FLOAT_VEC2 == type)
          {
            glvariable = new GLAttribute<Vector2f>(name, 2u, GL_FLOAT, gpuID());
          }
        else if (GL_FLOAT_VEC3 == type)
          {
            glvariable = new GLAttribute<Vector3f>(name, 3u, GL_FLOAT, gpuID());
          }
        else if (GL_FLOAT_VEC4 == type)
          {
            glvariable = new GLAttribute<Vector4f>(name, 4u, GL_FLOAT, gpuID());
          }
        else
          {
            LOGES("Attribute %s: type not managed", name);
            glvariable = nullptr;
          }

        if (nullptr != glvariable)
          {
            m_variables[name] = glvariable;
            //glvariable->begin();
          }
      }

    //Ajputer m_variables[derniere].end();

    m_indexed = true;
    return m_total;
  }

private:

  GLVAO            m_vao;
  GLVertexShader   m_vertex_shader;
  GLFragmentShader m_fragment_shader;
  GLGeometryShader m_geometry_shader;
  std::map<std::string, IGLVariable*> m_variables;
  bool m_indexed = false;
  size_t m_total = 0_z;
};

#endif /* GLPROGRAM_HPP_ */
