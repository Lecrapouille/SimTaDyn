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

  //------------------------------------------------------------------
  //! \brief Empty constructor. Do nothing.
  //------------------------------------------------------------------
  GLProgram(std::string const& name)
    : IGLObject(name), m_vao("VAO_" + name)
  {
  }

  //------------------------------------------------------------------
  //! \brief Destructor. Release GPU memory
  //------------------------------------------------------------------
  virtual ~GLProgram() override
  {
    destroy();
  }

  GLProgram& attachShaders(GLVertexShader&   vertex_shader,
                           GLFragmentShader& fragment_shader,
                           GLGeometryShader& geometry_shader)
  {
    m_shaders.push_back(vertex_shader);
    m_shaders.push_back(fragment_shader);
    m_shaders.push_back(geometry_shader);
    return *this;
  }

  GLProgram& attachShaders(GLVertexShader&   vertex_shader,
                           GLFragmentShader& fragment_shader)
  {
    m_shaders.push_back(vertex_shader);
    m_shaders.push_back(fragment_shader);
    return *this;
  }

  GLProgram& attachShader(GLShader& shader)
  {
    m_shaders.push_back(shader);
    return *this;
  }

  //------------------------------------------------------------------
  //! \brief Check if the program has been linked with success.
  //! Note: the method name is compiled() but should be linked() because
  //! compile is reserved for shaders.
  //------------------------------------------------------------------
  inline bool compiled() const
  {
    return m_compiled;
  }

  //------------------------------------------------------------------
  //! \brief Return the shader error message
  //------------------------------------------------------------------
  inline bool hasErrored() const
  {
    return !m_error_msg.empty();
  }

  //------------------------------------------------------------------
  //! \brief Return the shader error message. member variable is then
  //! cleared.
  //------------------------------------------------------------------
  inline std::string error()
  {
    std::string tmp(m_error_msg);
    m_error_msg.clear();
    return tmp;
  }

  //------------------------------------------------------------------
  //! \brief Return the list of shader names
  //------------------------------------------------------------------
  std::vector<std::string> shaderNames()
  {
    std::vector<std::string> list;
    for (auto &it: m_shaders)
      {
        list.push_back(it.name());
      }
    return list;
  }

  //------------------------------------------------------------------
  //! \brief Return the list of failed shaders
  //------------------------------------------------------------------
  std::vector<std::string> failedShaders()
  {
    std::vector<std::string> list;
    for (auto &it: m_shaders)
      {
        if (!it.compiled())
          {
            list.push_back(it.name());
          }
      }
    return list;
  }

  //------------------------------------------------------------------
  //! \brief Check if the uniform variable exists in the shader code
  //------------------------------------------------------------------
  inline bool hasUniform(const char *name) const
  {
    return m_uniforms.end() != m_uniforms.find(name);
  }

  //------------------------------------------------------------------
  //! \brief Locate the uniform variable by its name and its type T.
  //! Return the reference of the data.
  //------------------------------------------------------------------
  template<class T>
  inline T& uniform(const char *name)
  {
    return getUniform<T>(name).data();
  }

  //------------------------------------------------------------------
   //! \brief Check if the attribute variable exists in the shader code
  // FIXME if begin() a reussi
  // FIXME: peut on avoir un nom doublon attrib / unifo ??
  //------------------------------------------------------------------
  inline bool hasAttribute(const char *name) const
  {
    return m_attributes.end() != m_attributes.find(name);
  }

  //------------------------------------------------------------------
  //! \brief Locate the attribute variable by its name and its type T.
  //! Return the reference of the VBO.
  //------------------------------------------------------------------
  template<class T>
  inline GLVertexBuffer<T>& attribute(const char *name)
  {
    return getAttribute<T>(name).data();
  }

  //------------------------------------------------------------------
  //! \brief Get the shader variable. If the name does not refer to a
  //! valid variable an exception is triggered.
  //! Example GLProgram prog; prog["position"] = { ... };
  //------------------------------------------------------------------
  inline IGLVariable& operator[](std::string const& name)
  {
    auto it_uniform = m_uniforms.find(name);
    auto it_attribute = m_attributes.find(name);

    if ((it_uniform != m_uniforms.end()) &&
        (it_attribute != m_attributes.end()))
      {
        throw std::out_of_range("OpenGL variable '" + name +
                                "' can be either attribute or uniform");
      }
    else if (it_uniform != m_uniforms.end())
      {
        return *(it_uniform->second);
      }
    else if (it_attribute != m_attributes.end())
      {
        return *(it_attribute->second);
      }
    else
      {
        throw std::out_of_range("Name '" + name + "' is not an shader variable");
      }
  }

  //------------------------------------------------------------------
  //! \brief
  //------------------------------------------------------------------
  void throw_if_not_compiled()
  {
    if (unlikely(!compiled()))
      {
        throw OpenGLException("Failed OpenGL program has not been compiled");
      }
  }

  //------------------------------------------------------------------
  //! \brief check if all GLAttribute have their VBO with the
  //! same size. TODO change this method to a callback: on_GLVariable_changed()
  //------------------------------------------------------------------
  void throw_if_inconsitency_attrib_sizes(/* updated_size */)
  {
    /* TODO
    if (likely(!GLVariable_modified)) return ;

    for (auto& it: m_attributes)
      {
        if (it.size() != updated_size)
          {
            throw OpenGLException("Failed OpenGL attributes have not the same size");
          }
      }
    */
  }

  //------------------------------------------------------------------
  //! \brief Render primitives
  //------------------------------------------------------------------
  void draw(GLenum mode, GLint first, GLsizei count)
  {
    throw_if_not_compiled();
    throw_if_inconsitency_attrib_sizes();

    // FIXME: A optimiser car ca prend 43 appels OpenGL alors qu'avant
    // il suffisait entre 16 et 35
    m_vao.begin();
    begin();
    glCheck(glDrawArrays(mode, first, count));
    glCheck(glBindBuffer(GL_ARRAY_BUFFER, 0));
    end();
    m_vao.end();
  }

  //------------------------------------------------------------------
  //! \brief Render all primitives
  //------------------------------------------------------------------
  inline void draw(GLenum /*mode*/)
  {
    //throw_if_not_compiled();
    //throw_if_inconsitency_attrib_sizes();
    //draw(mode, 0, m_attributes.begin()->second->size());
  }

  //------------------------------------------------------------------
  //! \brief Render primitives from their indices
  //------------------------------------------------------------------
  template<class T>
  void draw(GLenum mode, GLIndexBuffer<T> const& index)
  {
    throw_if_not_compiled();
    throw_if_inconsitency_attrib_sizes();

    m_vao.begin();
    begin();
    index.begin();
    glCheck(glDrawElements(mode, index.size(), index.type(), 0));
    index.end();
    glCheck(glBindBuffer(GL_ARRAY_BUFFER, 0));
    end();
    m_vao.end();
  }

  //------------------------------------------------------------------
  //! \brief
  //------------------------------------------------------------------
  inline virtual bool isValid() const override
  {
    return compiled();
  }

private:

  //------------------------------------------------------------------
  //! \brief
  //------------------------------------------------------------------
  virtual bool create() override
  {
    m_handle = glCheck(glCreateProgram());
    return false;
  }

  //------------------------------------------------------------------
  //! \brief
  //------------------------------------------------------------------
  virtual bool setup() override
  {
    bool failure = false;

    // Compile shaders if they have not yet compiled
    for (auto &it: m_shaders)
      {
        it.begin();
        if (it.hasErrored())
          {
            m_error_msg += "Shader '" + it.name() +
              "' has not been compiled. Reason is '" +
              it.error() + "'";
            failure = true;
          }
      }

    if (!failure)
      {
        // Attach shaders to program
        for (auto &it: m_shaders)
          {
            glCheck(glAttachShader(m_handle, it.gpuID()));
          }

        // Compile the program
        glCheck(glLinkProgram(m_handle));
        m_compiled = checkLinkageStatus(m_handle);
        if (m_compiled)
          {
            m_error_msg.clear();
            // Create the list of attributes and uniforms
            getAllAttributesAndUniforms();
            // Release shaders stored in GPU.
            detachAllShaders();
          }
      }

    return !m_compiled;
  }

  //------------------------------------------------------------------
  //! \brief
  //------------------------------------------------------------------
  virtual void activate() override
  {
    glCheck(glUseProgram(m_handle));
    for (auto& it: m_attributes)
      {
        it.second->begin();
      }
    for (auto& it: m_uniforms)
      {
        it.second->begin();
      }
  }

  //------------------------------------------------------------------
  //! \brief Dummy method.
  //------------------------------------------------------------------
  virtual bool update() override
  {
    return false;
  }

  //------------------------------------------------------------------
  //! \brief A program can be desactivated if and only if shaders have
  //! been loaded into a program (else nothing is done).
  //------------------------------------------------------------------
  virtual void deactivate() override
  {
    glCheck(glUseProgram(0U));

    for (auto& it: m_attributes)
      {
        it.second->end();
      }
    for (auto& it: m_uniforms)
      {
        it.second->end();
      }
  }

  //------------------------------------------------------------------
  //! \brief Once program is no longer used, release it from the GPU
  //! memory. Can be used to abort the shader.
  //------------------------------------------------------------------
  virtual void release() override
  {
    detachAllShaders();
    glCheck(glDeleteProgram(m_handle));
  }

  //------------------------------------------------------------------
  //! \brief Create Attribute and Uniform instances
  //------------------------------------------------------------------
  void getAllAttributesAndUniforms()
  {
    const GLsizei bufSize = 64;
    GLchar name[bufSize];
    GLsizei length = 0;
    GLint size = 0;
    GLint count = 0;
    GLenum type;

    // Get all uniforms
    glCheck(glGetProgramiv(m_handle, GL_ACTIVE_UNIFORMS, &count));
    for (GLint i = 0; i < count; ++i)
      {
        glCheck(glGetActiveUniform(m_handle, (GLuint)i, bufSize,
                                   &length, &size, &type, name));
        LOGD("Uniform #%d Type: %u Name: %s", i, type, name);
        m_uniforms[name] = factoryUniform(type, name);
      }

    // Get all attributes
    glCheck(glGetProgramiv(m_handle, GL_ACTIVE_ATTRIBUTES, &count));
    for (GLint i = 0; i < count; ++i)
      {
        glCheck(glGetActiveAttrib(m_handle, (GLuint)i, bufSize,
                                  &length, &size, &type, name));
        LOGD("Attribute #%d Type: %u Name: %s", i, type, name);
        m_attributes[name] = factoryAttribute(type, name);
      }
  }

  //------------------------------------------------------------------
  //! \brief Create Attribute instances
  //------------------------------------------------------------------
  IGLVariable* factoryAttribute(GLenum type, const char *name)
  {
    IGLVariable* glvariable;

    switch (type)
      {
      case GL_FLOAT:
        glvariable = new GLAttribute<float>(name, 1u, GL_FLOAT, gpuID());
        break;
      case GL_FLOAT_VEC2:
        glvariable = new GLAttribute<Vector2f>(name, 2u, GL_FLOAT, gpuID());
        break;
      case GL_FLOAT_VEC3:
        glvariable = new GLAttribute<Vector3f>(name, 3u, GL_FLOAT, gpuID());
        break;
      case GL_FLOAT_VEC4:
        glvariable = new GLAttribute<Vector4f>(name, 4u, GL_FLOAT, gpuID());
        break;
      default:
        m_error_msg += "\nAttribute '" + std::string(name) +
          "' type is not managed";
        glvariable = nullptr;
        break;
      }

    return glvariable;
  }

  //------------------------------------------------------------------
  //! \brief Create Uniform instances
  //------------------------------------------------------------------
  IGLVariable* factoryUniform(GLenum type, const char *name)
  {
    IGLVariable* glvariable;

    switch (type)
      {
      case GL_FLOAT:
        glvariable = new GLUniform<float>(name, type, gpuID()); // FIXME: std_unique
        break;
      case GL_FLOAT_VEC2:
        glvariable = new GLUniform<Vector2f>(name, type, gpuID());
        break;
      case GL_FLOAT_VEC3:
        glvariable = new GLUniform<Vector3f>(name, type, gpuID());
        break;
      case GL_FLOAT_VEC4:
        glvariable = new GLUniform<Vector4f>(name, type, gpuID());
        break;
      case GL_INT:
        glvariable = new GLUniform<int>(name, type, gpuID());
        break;
      case GL_INT_VEC2:
        glvariable = new GLUniform<Vector2i>(name, type, gpuID());
        break;
      case GL_INT_VEC3:
        glvariable = new GLUniform<Vector3i>(name, type, gpuID());
        break;
      case GL_INT_VEC4:
        glvariable = new GLUniform<Vector4i>(name, type, gpuID());
        break;
        //case GL_BOOL:
        //glvariable = new GLUniform<bool>(name, type, gpuID());
        //break;
        //case GL_BOOL_VEC2:
        //glvariable = new GLUniform<Vector2b>(name, type, gpuID());
        //break;
      case GL_FLOAT_MAT2:
        glvariable = new GLUniform<Matrix22f>(name, type, gpuID());
        break;
      case GL_FLOAT_MAT3:
        glvariable = new GLUniform<Matrix33f>(name, type, gpuID());
        break;
      case GL_FLOAT_MAT4:
        glvariable = new GLUniform<Matrix44f>(name, type, gpuID());
        break;
      case GL_SAMPLER_1D:
      case GL_SAMPLER_2D:
      case GL_SAMPLER_CUBE:
        {
          GLUniform</*GLTexture2D*/float>* gluni =
            new GLUniform</*GLTexture2D*/float>(name, type, gpuID());
          gluni->m_texture_unit = m_textures_count;
          m_textures_count += 1u;
          glvariable = gluni;
        }
      default:
        m_error_msg += "\nUniform '" + std::string(name) +
          "' type is not managed";
        glvariable = nullptr;
        break;
      }
    return glvariable;
  }

  //------------------------------------------------------------------
  //! \brief Locate the uniform variable by its name and its type T.
  //! \return the uniform instance if found else throw the exception
  //! std::out_of_range
  //------------------------------------------------------------------
public: //FIXME
  template<class T>
  inline GLUniform<T>& getUniform(const char *name)
  {
    if (unlikely(nullptr == name))
      {
        throw std::invalid_argument("nullptr passed to getUniform");
      }

    auto ptr = m_uniforms[name];
    if (unlikely(nullptr == ptr))
      {
        throw std::out_of_range("GLUniform '" + std::string(name) +
                                "' does not exist");
      }

    GLUniform<T> *uniform_ptr = dynamic_cast<GLUniform<T>*>(ptr);
    if (unlikely(nullptr == uniform_ptr))
      {
        throw std::out_of_range("GLUniform '" + std::string(name) +
                                "' exists but has wrong template type");
      }
    return *uniform_ptr;
  }

private:

  //------------------------------------------------------------------
  //! \brief Locate the attribute variable by its name and its type T.
  //! \return the uniform instance if found else throw the exception
  //! std::out_of_range
  //------------------------------------------------------------------
  template<class T>
  inline GLAttribute<T>& getAttribute(const char *name)
  {
    if (unlikely(nullptr == name))
      {
        throw std::invalid_argument("nullptr passed to getAttribute");
      }

    auto ptr = m_attributes[name];
    if (unlikely(nullptr == ptr))
      {
        throw std::out_of_range("GLAttribute '" + std::string(name) +
                                "' does not exist");
      }

    GLAttribute<T> *attrib_ptr = dynamic_cast<GLAttribute<T>*>(ptr);
    if (unlikely(nullptr == attrib_ptr))
      {
        throw std::out_of_range("GLAttribute '" + std::string(name) +
                                "' exists but has wrong template type");
      }
    return *attrib_ptr;
  }

  void detachAllShaders()
  {
    for (auto &it: m_shaders)
      {
        if (it.isValid())
          {
            glCheck(glDetachShader(m_handle, it.gpuID()));
          }
      }
  }

  //------------------------------------------------------------------
  //! \return true if case of success, else return false.
  //------------------------------------------------------------------
  bool checkLinkageStatus(GLuint obj)
  {
    GLint status;

    glCheck(glGetProgramiv(obj, GL_LINK_STATUS, &status));
    if (GL_FALSE == status)
      {
        GLint length;
        glCheck(glGetProgramiv(obj, GL_INFO_LOG_LENGTH, &length));
        std::vector<char> log(length);
        glCheck(glGetProgramInfoLog(obj, length, &length, &log[0U]));
        m_error_msg += &log[0U];
        LOGES("%s", m_error_msg.c_str());
      }
    else
      {
        m_error_msg.clear();
      }
    return status;
  }

private:

  std::map<std::string, IGLVariable*> m_attributes;
  std::map<std::string, IGLVariable*> m_uniforms;
  std::vector<GLShader>               m_shaders;
  GLVAO                               m_vao;
  std::string                         m_error_msg;
  bool                                m_compiled = false;
  uint32_t                            m_textures_count = 0u;
};

#endif /* GLPROGRAM_HPP_ */
