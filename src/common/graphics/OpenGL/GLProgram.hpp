#ifndef GLPROGRAM_HPP_
#  define GLPROGRAM_HPP_

//! \brief This file contains the class managing OpenGL shaders :
//! read, compile, load into the GPU.

#  include "NonCppStd.hpp"
#  include "GLShaders.hpp"
#  include "GLVariables.tpp"
#  include <map>

// TODO: verifier les GLVariables non init dans le GPU
// **************************************************************
//! \class GLShader GLShader.hpp
//! \brief
// **************************************************************
class GLProgram: public IGLObject<GLenum>
{
  using IGLVariablePtr = std::unique_ptr<IGLVariable>;
  using map_t = std::map<std::string, IGLVariablePtr>;

public:

  //------------------------------------------------------------------
  //! \brief Empty constructor. Do nothing.
  //------------------------------------------------------------------
  GLProgram(std::string const& name)
    : IGLObject(name)
  {
  }

  //------------------------------------------------------------------
  //! \brief Destructor. Release GPU memory
  //------------------------------------------------------------------
  virtual ~GLProgram() override
  {
    destroy();
  }

  GLProgram& attachShader(GLShader& shader)
  {
    LOGD("Prog::attachShader");
    m_shaders.push_back(shader);
    return *this;
  }

  GLProgram& attachShaders(GLVertexShader&   vertex_shader,
                           GLFragmentShader& fragment_shader,
                           GLGeometryShader& geometry_shader)
  {
    LOGD("Prog::attachShaders");
    m_shaders.push_back(vertex_shader);
    m_shaders.push_back(fragment_shader);
    m_shaders.push_back(geometry_shader);
    return *this;
  }

  GLProgram& attachShaders(GLVertexShader&   vertex_shader,
                           GLFragmentShader& fragment_shader)
  {
    LOGD("Prog::attachShaders");
    m_shaders.push_back(vertex_shader);
    m_shaders.push_back(fragment_shader);
    return *this;
  }

  inline void bind(GLVAO& vao)
  {
#if 1
    m_vao = &vao;
    m_binded = true;
#else
    LOGD("Prog::bind VAO");
    m_vao = &vao;
    m_vao->name() = "VAO_" + name();
    m_vao->begin();
    if (!m_vao->isValid())
      {
        LOGE("Failed binding VAO '%s' to Program '%s'",
             m_vao->name().c_str(), name().c_str());
        m_vao = nullptr;
      }
    m_binded = (nullptr != m_vao);
#endif
  }

  inline bool binded() const
  {
    return m_binded;
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
  //! \brief Return the list of unifom names
  //------------------------------------------------------------------
  std::vector<std::string> uniformNames()
  {
    std::vector<std::string> list;
    for (auto &it: m_uniforms)
      {
        list.push_back(it.first);
      }
    return list;
  }

  //------------------------------------------------------------------
  //! \brief Return the list of unifom names
  //------------------------------------------------------------------
  std::vector<std::string> attributeNames()
  {
    std::vector<std::string> list;
    for (auto &it: m_attributes)
      {
        list.push_back(it.first);
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

  template<class T>
  inline const T& uniform(const char *name) const
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
  inline PendingContainer<T>& attribute(const char *name)
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
  //! \brief
  //------------------------------------------------------------------
  void throw_if_not_binded()
  {
    if (unlikely(!binded()))
      {
        throw OpenGLException("Failed OpenGL program has not been binded to a VAO");
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
    LOGD("Prog::draw");
    throw_if_not_compiled();
    throw_if_not_binded();
    throw_if_inconsitency_attrib_sizes();

    // FIXME: A optimiser car ca prend 43 appels OpenGL alors qu'avant
    // il suffisait entre 16 et 35
    m_vao->begin();
    begin();
    glCheck(glDrawArrays(mode, first, count));
    glCheck(glBindBuffer(GL_ARRAY_BUFFER, 0));
    end();
    m_vao->end();
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
    LOGD("Prog::drawIndex");
    throw_if_not_compiled();
    throw_if_not_binded();
    throw_if_inconsitency_attrib_sizes();

    m_vao->begin();
    begin();
    index.begin();
    glCheck(glDrawElements(mode, index.size(), index.type(), 0));
    index.end();
    glCheck(glBindBuffer(GL_ARRAY_BUFFER, 0));
    end();
    m_vao->end();
  }

  //------------------------------------------------------------------
  //! \brief
  //------------------------------------------------------------------
  /*inline virtual bool isValid() const override
  {
    LOGD("Prog::isValid %d", compiled());
    return compiled();
    }*/

private:

  //------------------------------------------------------------------
  //! \brief
  //------------------------------------------------------------------
  virtual bool create() override
  {
    LOGD("Prog::create");
    m_handle = glCheck(glCreateProgram());
    // Note: Contrary to VBO, GLProgram has to perform
    // its setup() before calling activate() !
    return false;
  }

  //------------------------------------------------------------------
  //! \brief
  //------------------------------------------------------------------
  virtual bool setup() override
  {
    bool failure = false;

    // Compile shaders if they have not yet compiled
    LOGD("Prog::setup compile shaders");
    for (auto &it: m_shaders)
      {
        it.begin();
        if (it.hasErrored())
          {
            std::string msg =
              "Shader '" + it.name() +
              "' has not been compiled. Reason is '" +
              it.error() + "'";
            LOGE("%s", msg.c_str());
            m_error_msg += msg;
            failure = true;
          }
      }

    if (!failure)
      {
        // Attach shaders to program
        LOGD("Prog::setup attach shaders");
        for (auto &it: m_shaders)
          {
            glCheck(glAttachShader(m_handle, it.gpuID()));
            it.attached(m_handle);
          }

        // Compile the program
        LOGD("Prog::setup compile prog");
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
    if (unlikely(!compiled()))
      return ;
    if (unlikely(!binded()))
      return ;

    LOGD("Prog::activate");
    m_vao->begin();
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
    LOGD("Prog::deactivate");
    glCheck(glUseProgram(0U));

    for (auto& it: m_attributes)
      {
        it.second->end();
      }
    for (auto& it: m_uniforms)
      {
        it.second->end();
      }
    GLVAO::unbind();
  }

  //------------------------------------------------------------------
  //! \brief Once program is no longer used, release it from the GPU
  //! memory. Can be used to abort the shader.
  //------------------------------------------------------------------
  virtual void release() override
  {
    LOGD("Prog::release");
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
    GLuint i;
    GLenum type;

    // Get all uniforms
    LOGD("Prog::get all attrib and uniform");
    glCheck(glGetProgramiv(m_handle, GL_ACTIVE_UNIFORMS, &count));
    i = static_cast<GLuint>(count);
    while (i--)
      {
        glCheck(glGetActiveUniform(m_handle, i, bufSize, &length,
                                   &size, &type, name));
        LOGD("Uniform #%u Type: %u Name: %s", i, type, name);
        addNewUniform(type, name);
      }

    // Get all attributes
    glCheck(glGetProgramiv(m_handle, GL_ACTIVE_ATTRIBUTES, &count));
    i = static_cast<GLuint>(count);
    while (i--)
      {
        glCheck(glGetActiveAttrib(m_handle, i, bufSize, &length,
                                  &size, &type, name));
        LOGD("Attribute #%u Type: %u Name: %s", i, type, name);
        addNewAttribute(type, name);
      }
  }

  //------------------------------------------------------------------
  //! \brief Create Attribute instances
  //------------------------------------------------------------------
  void addNewAttribute(GLenum type, const char *name)
  {
    GLenum EGL_FLOAT = static_cast<GLenum>(GL_FLOAT);
    switch (type)
      {
      case GL_FLOAT:
        m_attributes[name] = std::make_unique<GLAttribute<float>>(name, 1, EGL_FLOAT, gpuID());
        break;
      case GL_FLOAT_VEC2:
        m_attributes[name] = std::make_unique<GLAttribute<Vector2f>>(name, 2, EGL_FLOAT, gpuID());
        break;
      case GL_FLOAT_VEC3:
        m_attributes[name] = std::make_unique<GLAttribute<Vector3f>>(name, 3, EGL_FLOAT, gpuID());
        break;
      case GL_FLOAT_VEC4:
        m_attributes[name] = std::make_unique<GLAttribute<Vector4f>>(name, 4, EGL_FLOAT, gpuID());
        break;
      default:
        std::string msg = "Attribute '" + std::string(name) + "' type is not managed";
        LOGE("%s", msg.c_str());
        m_error_msg += '\n' + msg;
        break;
      }
  }

  //------------------------------------------------------------------
  //! \brief Create Uniform instances
  //------------------------------------------------------------------
  void addNewUniform(GLenum type, const char *name)
  {
    switch (type)
      {
      case GL_FLOAT:
        m_uniforms[name] = std::make_unique<GLUniform<float>>(name, type, gpuID());
        break;
      case GL_FLOAT_VEC2:
        m_uniforms[name] = std::make_unique<GLUniform<Vector2f>>(name, type, gpuID());
        break;
      case GL_FLOAT_VEC3:
        m_uniforms[name] = std::make_unique<GLUniform<Vector3f>>(name, type, gpuID());
        break;
      case GL_FLOAT_VEC4:
        m_uniforms[name] = std::make_unique<GLUniform<Vector4f>>(name, type, gpuID());
        break;
      case GL_INT:
        m_uniforms[name] = std::make_unique<GLUniform<int>>(name, type, gpuID());
        break;
      case GL_INT_VEC2:
        m_uniforms[name] = std::make_unique<GLUniform<Vector2i>>(name, type, gpuID());
        break;
      case GL_INT_VEC3:
        m_uniforms[name] = std::make_unique<GLUniform<Vector3i>>(name, type, gpuID());
        break;
      case GL_INT_VEC4:
        m_uniforms[name] = std::make_unique<GLUniform<Vector4i>>(name, type, gpuID());
        break;
        //case GL_BOOL:
        //m_uniforms[name] = std::make_unique<GLUniform<bool>>(name, type, gpuID());
        //break;
        //case GL_BOOL_VEC2:
        //m_uniforms[name] = std::make_unique<GLUniform<Vector2b>>(name, type, gpuID());
        //break;
      case GL_FLOAT_MAT2:
        m_uniforms[name] = std::make_unique<GLUniform<Matrix22f>>(name, type, gpuID());
        break;
      case GL_FLOAT_MAT3:
        m_uniforms[name] = std::make_unique<GLUniform<Matrix33f>>(name, type, gpuID());
        break;
      case GL_FLOAT_MAT4:
        m_uniforms[name] = std::make_unique<GLUniform<Matrix44f>>(name, type, gpuID());
        break;
        /*case GL_SAMPLER_1D:
        m_uniforms[name] = std::make_unique<GLSampler1D>(name, m_textures_count, gpuID());
        m_textures_count += 1u;
        break;*/
      case GL_SAMPLER_2D:
        m_uniforms[name] = std::make_unique<GLSampler2D>(name, m_textures_count, gpuID());
        m_textures_count += 1u;
        break;
        /*case GL_SAMPLER_CUBE:
        m_uniforms[name] = std::make_unique<GLSampler3D>(name, m_textures_count, gpuID());
        m_textures_count += 1u;
        break;*/
      default:
        std::string msg = "Uniform '" + std::string(name) + "' type is not managed";
        LOGE("%s", msg.c_str());
        m_error_msg += '\n' + msg;
        break;
      }
  }

  //------------------------------------------------------------------
  //! \brief Locate the uniform variable by its name and its type T.
  //! \return the uniform instance if found else throw the exception
  //! std::out_of_range
  //------------------------------------------------------------------

  template<class T>
  inline IGLUniform<T>& getUniform(const char *name)
  {
    if (unlikely(!compiled()))
      {
        begin();
        // TODO: check if now compiled() == true
      }

    if (unlikely(nullptr == name))
      {
        throw std::invalid_argument("nullptr passed to getUniform");
      }

    auto ptr = m_uniforms[name].get();
    if (unlikely(nullptr == ptr))
      {
        // TODO: create the variable: call addNewUniform
        // TODO: http://www.cplusplus.com/forum/general/21246/#msg112085
        throw std::out_of_range("GLUniform '" + std::string(name) +
                                "' does not exist");
      }

    IGLUniform<T> *uniform_ptr = dynamic_cast<IGLUniform<T>*>(ptr);
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
    if (unlikely(!compiled()))
      {
        begin();
      }

    if (unlikely(nullptr == name))
      {
        throw std::invalid_argument("nullptr passed to getAttribute");
      }

    auto ptr = m_attributes[name].get();
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
    LOGD("Prog::detachAllshaders");
    for (auto &it: m_shaders)
      {
        if (m_handle == it.attached())
          {
            glCheck(glDetachShader(m_handle, it.gpuID()));
            it.attached(0);
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
        std::vector<char> log(static_cast<size_t>(length));
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

  map_t                  m_attributes;
  map_t                  m_uniforms;
  std::vector<GLShader>  m_shaders;
  GLVAO                 *m_vao = nullptr;
  std::string            m_error_msg;
  uint32_t               m_textures_count = 0u;
  bool                   m_compiled = false;
  bool                   m_binded = false;
};

#endif /* GLPROGRAM_HPP_ */
