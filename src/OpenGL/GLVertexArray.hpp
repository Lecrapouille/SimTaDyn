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

  //! \brief Destructor. The program (if loaded) is removed from the GPU.
  ~GLVertexArray()
  {
    destroy();
  }

protected:

  virtual void create() override
  {
    glCheck(glGenVertexArrays(1, &m_handle));
  }

  virtual void release() override
  {
    if (isActivable())
      {
        glCheck(glDeleteVertexArrays(1, &m_handle));
      }
  }

  virtual void activate() override
  {
    if (isActivable())
      {
        glCheck(glBindVertexArray(m_handle));
      }
  }

  virtual void deactivate() override
  {
    if (isActivable())
      {
        glCheck(glBindVertexArray(0U));
      }
  }

  virtual void setup() override
  {
  }

  virtual void update() override
  {
  }
};

#endif /* GL_VERTEX_ARRAY_HPP_ */
