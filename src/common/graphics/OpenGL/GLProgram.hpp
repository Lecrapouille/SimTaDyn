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
  inline GLAttribute<T>& attribute(const char *name)
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

  inline void draw(GLenum mode, GLint first, GLsizei count)
  {
    begin();
    glCheck(glDrawArrays(mode, first, count));
    glCheck(glBindBuffer(GL_ARRAY_BUFFER, 0));
    end();
  }

  /*inline void draw(GLenum mode)
    {
    draw(mode, 0, );
    }

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
    if (!needSetup()) // FIXME or call glUseProgram(0) is setup()
      {
        LOGD("%s: activate", name().c_str());
        glCheck(glUseProgram(m_handle));

        m_vao.begin();
        for (auto& it: m_variables)
          {
            it.second->begin();
          }
        for (auto& it: m_variables)
          {
            it.second->begin();
          }
      }
    else
      {
        LOGD("%s: failed activate: need setup", name().c_str());
      }
  }

  virtual bool setup() override
  {
    LOGD("%s: setup", name().c_str());
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
        LOGE("[FAILED] %s\n", &log[0U]);
        goto l_try_again;
      }

    // TODO: glValidateProgram(m_handle);

    // --- Construct list of uniforms and attributes
    LOGD("Index attrib and uniform variables");
    indexVariables();

    // --- Detach shaders
    LOGD("Detach shaders");
    detachAllShaders();
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
    LOGD("%s: deactivate", name().c_str());
    glCheck(glUseProgram(0U));
    for (auto& it: m_variables)
      {
        it.second->end();
      }
    for (auto& it: m_variables)
      {
        it.second->end();
      }
    m_vao.end();
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
    LOGD("%s: detach all sahders", name().c_str());
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

    glCheck(glGetProgramiv(m_handle, GL_ACTIVE_UNIFORMS, &count));
    m_total = count;
    printf("Active Uniforms: %d\n", count);
    for (GLint i = 0; i < count; ++i)
      {
        glCheck(glGetActiveUniform(m_handle, (GLuint)i, bufSize, &length, &size, &type, name));
        printf("Uniform #%d Type: %u Name: %s\n", i, type, name);
        if (GL_FLOAT == type) // FIXME leak
          m_variables[name] = new GLUniform<float>(name, type, gpuID());
        else if (GL_FLOAT_VEC2 == type)
          m_variables[name] = new GLUniform<Vector2f>(name, type, gpuID());
        else if (GL_FLOAT_VEC3 == type)
          m_variables[name] = new GLUniform<Vector3f>(name, type, gpuID());
        else if (GL_FLOAT_VEC4 == type)
          m_variables[name] = new GLUniform<Vector4f>(name, type, gpuID());
        else if (GL_INT == type)
          m_variables[name] = new GLUniform<int>(name, type, gpuID());
        else if (GL_INT_VEC2 == type)
          m_variables[name] = new GLUniform<Vector2i>(name, type, gpuID());
        else if (GL_INT_VEC3 == type)
          m_variables[name] = new GLUniform<Vector3i>(name, type, gpuID());
        else if (GL_INT_VEC4 == type)
          m_variables[name] = new GLUniform<Vector4i>(name, type, gpuID());
        //else if (GL_BOOL == type)
        //  m_variables[name] = new GLUniform<bool>(name, type, gpuID());
        //else if (GL_BOOL_VEC2 == type)
        //  m_variables[name] = new GLUniform<Vector2b>(name, type, gpuID());
        else if (GL_FLOAT_MAT2 == type)
          m_variables[name] = new GLUniform<Matrix22f>(name, type, gpuID());
        else if (GL_FLOAT_MAT3 == type)
          m_variables[name] = new GLUniform<Matrix33f>(name, type, gpuID());
        else if (GL_FLOAT_MAT4 == type)
        m_variables[name] = new GLUniform<Matrix44f>(name, type, gpuID());
        // else if gl.GL_SAMPLER_1D, gl.GL_SAMPLER_2D, GL_SAMPLER_CUBE
      }

    glCheck(glGetProgramiv(m_handle, GL_ACTIVE_ATTRIBUTES, &count));
    printf("Active Attributes: %d\n", count);
    m_total += count;
    for (GLint i = 0; i < count; ++i)
      {
        glCheck(glGetActiveAttrib(m_handle, (GLuint)i, bufSize, &length, &size, &type, name));
        printf("Attribute #%d Type: %u Name: %s\n", i, type, name);
        if (GL_FLOAT == type) // FIXME leak
          {
            m_variables[name] = new GLAttribute<float>(name, type, gpuID());
          }
        else if (GL_FLOAT_VEC2 == type)
          {
            m_variables[name] = new GLAttribute<Vector2f>(name, type, gpuID());
          }
        else if (GL_FLOAT_VEC3 == type)
          {
            auto p = new GLAttribute<Vector3f>(name, type, gpuID());
            printf("New Attrib V3f %p\n", p);
            m_variables[name] = p;
          }
        else if (GL_FLOAT_VEC4 == type)
          {
            auto p = new GLAttribute<Vector4f>(name, type, gpuID());
            printf("New Attrib V4f %s: %p\n", name, p);
            m_variables[name] = p;
          }
      }
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
