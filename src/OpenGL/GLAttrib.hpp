#ifndef GLATTRIB_HPP_
#  define GLATTRIB_HPP_

#  include "GLShader.hpp"

class GLAttrib: public GLObject
{
public:

  //! \brief
  enum AttribType { Attribute, Uniform, Unknown };

  //! \brief Empty constructor without name
  GLAttrib()
    : GLObject()
  {
  }

  //! \brief Constructor with the object name
  GLAttrib(std::string const& name)
    : GLObject(name)
  {
  }

  //! \brief Constructor with the object name
  GLAttrib(const char *name)
    : GLObject(name)
  {
  }

  //! \brief Setup Uniform
  void setup(GLShader& program, std::string const& identifier)
  {
    setup(program, identifier.c_str());
  }

  //! \brief Setup Uniform
  void setup(GLShader& program, const char *identifier)
  {
    m_program = &program;
    m_identifier = identifier;

    // FIXME: look into the shader code to determine it
    m_gender = Uniform;

    //
    m_need_setup = true;
    private_setup();
  }

  //! \brief Setup attributes
  void setup(GLShader& program, std::string const& identifier, const GLint size, const GLenum type, const GLboolean normalized = GL_FALSE)
  {
    setup(program, identifier.c_str(), size, type, normalized);
  }

  //! \brief Setup attributes
  void setup(GLShader& program, const char *identifier, const GLint size, const GLenum type, const GLboolean normalized = GL_FALSE)
  {
    m_program = &program;
    m_identifier = identifier;
    m_normalized = normalized;

    // FIXME: look into the shader code (ie. vec3 ==> size = 3 type = float) and only for attributes
    m_size = size;
    m_type = type;
    // FIXME: look into the shader code to determine it
    m_gender = Attribute;

    //
    m_need_setup = true;
    private_setup(); // FIXME: yes or not ?
  }

  ~GLAttrib()
  {
    destroy();
  }

  inline GLint size() const
  {
    return m_size;
  }

protected:

  virtual bool isActivable() override
  {
    return ((GLint) m_handle) != -1;
  }
  virtual void create() override
  {
  }
  virtual void release() override
  {
  }
  virtual void activate() override
  {
    if (isActivable() && (Attribute == m_gender))
      {
        glCheck(glEnableVertexAttribArray((GLint) m_handle));
        glCheck(glVertexAttribPointer((GLint) m_handle, m_size, m_type, m_normalized, 0, nullptr));
      }
  }
  virtual void deactivate() override
  {
    if (isActivable())
      {
        glCheck(glEnableVertexAttribArray(0));
      }
  }
  virtual void setup() override
  {
    m_need_setup = !private_setup();
  }

  virtual void update() override
  {
  }

private:

  bool private_setup()
  {
    if (nullptr == m_program)
      return false;

    m_handle = (GLenum) m_program->locate(m_identifier.c_str());
    if (!isActivable())
      return false;

    // FIXME: look into the shader code (ie. vec3 ==> size = 3 type =
    // float) and only for attributes and replace param given in the
    // constructor.
    //if ('a' != m_identifier[0]) return true;
    //if (!m_program.getTypeOf(m_identifier.c_str(), &m_size, &m_type)) {
    //  m_handle = (GLenum) -1; return false; }
    return true;
  }

protected:

  GLShader *m_program = nullptr;
  std::string m_identifier;
  GLint m_size;
  GLenum m_type;
  GLboolean m_normalized;
  enum AttribType m_gender = Unknown;
};

#endif /* GLATTRIB_HPP_ */
