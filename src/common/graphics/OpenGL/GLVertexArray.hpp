#ifndef GL_VERTEX_ARRAY_HPP_
#  define GL_VERTEX_ARRAY_HPP_

#  include "GLObject.hpp"

class GLVertexArray: public GLObject
{
public:

  //! \brief Empty constructor without name
  GLVertexArray()
    : GLObject()
  {
  }

  //! \brief Constructor with the object name
  GLVertexArray(std::string const& name)
    : GLObject(name)
  {
  }

  //! \brief Constructor with the object name
  GLVertexArray(const char *name)
    : GLObject(name)
  {
  }

  //! \brief Destructor: release data from the GPU and CPU memory.
  virtual ~GLVertexArray()
  {
    destroy();
  }

protected:

  virtual bool create() override
  {
    glCheck(glGenVertexArrays(1, &m_handle));
    return false;
  }

  virtual void release() override
  {
    glCheck(glDeleteVertexArrays(1, &m_handle));
  }

  virtual void activate() override
  {
    glCheck(glBindVertexArray(m_handle));
  }

  virtual void deactivate() override
  {
    glCheck(glBindVertexArray(0U));
  }

  virtual bool setup() override
  {
    return false;
  }

  virtual bool update() override
  {
    return false;
  }
};

#endif /* GL_VERTEX_ARRAY_HPP_ */
