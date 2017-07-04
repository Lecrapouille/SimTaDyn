#ifndef GLVERTEX_BUFFER_HPP_
#  define GLVERTEX_BUFFER_HPP_

#  include "PendingData.hpp"
#  include "GLAttrib.hpp"
#  include <vector>

// **************************************************************
//! \brief Buffer objects are OpenGL objects that store an array of
//! unformatted memory allocated by the OpenGL context (aka: the
//! GPU). These can be used to store vertex data, pixel data retrieved
//! from images or the framebuffer, and a variety of other things.
// **************************************************************
template<typename T>
class GLBuffer: public GLObject, protected PendingData
{
public:

  //! \brief Constructor with the object name
  GLBuffer(std::string const& name, const GLenum target, const GLenum usage = GL_DYNAMIC_DRAW)
    : GLObject(name), PendingData(), m_attrib(name)
  {
    GLObject::m_target = target;
    m_usage = usage;
  }

  //! \brief Constructor with the object name
  GLBuffer(const char *name, const GLenum target, const GLenum usage = GL_DYNAMIC_DRAW)
    : GLObject(name), PendingData(), m_attrib(name)
  {
    GLObject::m_target = target;
    m_usage = usage;
  }

  //! \brief Destructor: release data from the GPU and CPU memory.
  virtual ~GLBuffer()
  {
    GLObject::destroy();
  }

  //! if nth is lower than the current size. The nth is updated.
  //! Else (if nth is greater than the current size) two possible
  //! cases:
  //! -- Either the container size has not been reserved into the GPU
  //! (ie, if the method start() have not been called): the container
  //! size will change if In the case.
  //! -- the method start() have not been called, and so the container
  //! size cannot increase. A out_of_range exception is called.
  void reserve(size_t nth)
  {
    if (GLObject::m_need_create)
      {
        uint32_t s = m_data.capacity();
        m_data.reserve(nth - s + 1U);
        while (s--)
          {
            m_data.push_back(m_data[0]);
          }
      }
    else
      {
        const uint32_t s = m_data.capacity();
        std::string msg = "GLBuffer " + m_name + "["
          + std::to_string(nth) + "], max elt: "
          + std::to_string(s);
        throw std::out_of_range(msg.c_str());
      }
  }

  //! \brief Insert or change element at the given position.
  inline T& operator[](size_t nth)
  {
    if (nth > m_data.capacity())
      {
        reserve(nth);
      }
    PendingData::addPendingData(nth);
    return m_data[nth];
  }

  inline const T& operator[](size_t nth) const
  {
    return m_data[nth];
  }

  //! \brief Return the number of elements a block can store
  inline uint32_t size() const
  {
    return m_data.size();
  }

  inline uint32_t capacity() const
  {
    return m_data.capacity();
  }

  //! \brief Append elements from a given vector.
  //! \param vect
  void add(std::vector<T> &vect)
  {
    uint32_t size = vect.size();
    if (size > m_data.capacity())
      {
        reserve(size);
      }

    uint32_t offset = m_data.size();
    PendingData::addPendingData(offset, offset + size);

    while (size--)
      {
        m_data.push_back(vect[size]);
      }
  }

  void add(const T *array, const uint32_t size)
  {
    if (size > m_data.capacity())
      {
        reserve(size);
      }

    uint32_t offset = m_data.size();
    PendingData::addPendingData(offset, offset + size);

    uint32_t s = size;
    while (s--)
      {
        m_data.push_back(array[s]);
      }
  }

  virtual void setup(GLShader& program, const GLint size, const GLenum type, const GLboolean normalized = GL_FALSE)
  {
    LOGI("VBO::config");
    m_attrib.setup(program, size, type, normalized);
  }

  virtual void start() override
  {
    GLObject::start();
    m_attrib.start();
  }

  virtual void stop() override
  {
    GLObject::stop();
    m_attrib.stop();
  }

protected:

  virtual bool create() override
  {
    LOGI("VBO::create");
    const uint32_t bytes = m_data.capacity() * sizeof (T);
    glCheck(glGenBuffers(1, &m_handle));
    activate();
    glCheck(glBufferData(m_target, bytes, nullptr, m_usage));
    //deactivate();
    return false;
  }

  virtual void release() override
  {
    glCheck(glDeleteBuffers(1, &m_handle));
  }

  virtual void activate() override
  {
    LOGI("VBO::activate");
    glCheck(glBindBuffer(m_target, m_handle));
  }

  virtual void deactivate() override
  {
    LOGI("VBO::deactive");
    glCheck(glBindBuffer(m_target, 0));
  }

  virtual bool setup() override
  {
    LOGI("VBO::setup");
    return false;
  }

  virtual inline bool needUpdate() const override
  {
    return PendingData::hasPendingData();
  }

  virtual bool update() override
  {
    uint32_t pos_start;
    uint32_t pos_end;
    PendingData::getPendingData(pos_start, pos_end);

    uint32_t offset = sizeof (T) * pos_start;
    uint32_t nbytes = sizeof (T) * (pos_end - pos_start + 1U);
    glCheck(glBufferSubData(m_target, offset, nbytes, &m_data[0]));
    PendingData::clearPending();
    return false;
  }

protected:

  GLAttrib m_attrib;
  GLenum m_usage;
  std::vector<T> m_data;
};

// **************************************************************
//! \brief Buffer for vertex attribute data.
// **************************************************************
template<typename T>
class GLVertexBuffer: public GLBuffer<T>
{
public:

  //! \brief Constructor with the object name
  GLVertexBuffer(std::string const& name, const GLenum usage = GL_DYNAMIC_DRAW)
    : GLBuffer<T>(name, GL_ARRAY_BUFFER, usage)
  {
  }

  //! \brief Constructor with the object name
  GLVertexBuffer(const char *name, const GLenum usage = GL_DYNAMIC_DRAW)
    : GLBuffer<T>(name, GL_ARRAY_BUFFER, usage)
  {
  }
};

// **************************************************************
//! \brief Buffer for index data.
// **************************************************************
template<typename T>
class GLIndexBuffer: public GLBuffer<T>
{
public:

  //! \brief Constructor with the object name
  GLIndexBuffer(std::string const& name, const GLenum usage = GL_DYNAMIC_DRAW)
    : GLBuffer<T>(name, GL_ELEMENT_ARRAY_BUFFER, usage)
  {
  }

  //! \brief Constructor with the object name
  GLIndexBuffer(const char *name, const GLenum usage = GL_DYNAMIC_DRAW)
    : GLBuffer<T>(name, GL_ELEMENT_ARRAY_BUFFER, usage)
  {
  }
};

#endif /* GLVERTEX_BUFFER_HPP_ */
