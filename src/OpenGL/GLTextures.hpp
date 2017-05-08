#ifndef GLTEXTURES_HPP_
#  define GLTEXTURES_HPP_

#  include "GLObject.hpp"
#  include <SOIL/SOIL.h>

// **************************************************************
//! FIXME: should inherit from a GPUdata
// **************************************************************
class GLTexture: public GLObject
{
public:

  GLTexture(const GLenum target)
    : GLObject()
  {
    m_target = target;
  }

  GLTexture(std::string const& name, const GLenum target)
    : GLObject(name)
  {
    m_target = target;
  }

  void interpolation(const float min_filter, const float  mag_filter)
  {
    m_min_filter = min_filter;
    m_mag_filter = mag_filter;
    m_need_setup = true;
  }

  void interpolation(const float min_filter)
  {
    m_min_filter = min_filter;
    m_mag_filter = min_filter;
    m_need_setup = true;
  }

  void wrapping(const float wrap)
  {
    m_wrapping = wrap;
    m_need_setup = true;
  }

protected:

  virtual void create() override
  {
    glCheck(glGenTextures(1U, &m_handle));
  }

  virtual void release() override
  {
    if (isActivable())
      {
        glCheck(glDeleteTextures(1U, &m_handle));
      }
  }

  virtual void activate() override
  {
    glCheck(glBindTexture(m_target, m_handle));
    if (needSetup())
      {
        setup();
      }
  }

  virtual void deactivate() override
  {
    glCheck(glBindTexture(m_target, 0U));
  }

  virtual void setup() override
  {
    glCheck(glTexParameterf(m_target, GL_TEXTURE_MIN_FILTER, m_min_filter));
    glCheck(glTexParameterf(m_target, GL_TEXTURE_MAG_FILTER, m_mag_filter));
    glCheck(glTexParameterf(m_target, GL_TEXTURE_WRAP_S, m_wrapping));
    glCheck(glTexParameterf(m_target, GL_TEXTURE_WRAP_T, m_wrapping));
    glCheck(glTexParameterf(m_target, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));
    m_need_setup = false;
  }

  float m_min_filter = GL_NEAREST;
  float m_mag_filter = GL_NEAREST;
  float m_wrapping = GL_CLAMP_TO_EDGE;
};

// **************************************************************
//!
// **************************************************************
class GLTexture2D: public GLTexture
{
public:

  GLTexture2D()
    : GLTexture(GL_TEXTURE_2D)
  {
  }

  GLTexture2D(std::string const& name)
    : GLTexture(name, GL_TEXTURE_2D)
  {
  }

  inline void load(std::string const& filename, const bool rename = true)
  {
    load(filename.c_str(), rename);
  }

  // FIXME: m_buffer should be a container for automatic update
  void load(const char *const filename, const bool rename = true)
  {
    int width, height;

    LOGI("Loading texture '%s'", filename);
    if (nullptr != m_buffer)
      {
        SOIL_free_image_data(m_buffer);
      }
    m_buffer = SOIL_load_image(filename, &width, &height, 0, SOIL_LOAD_RGB);

    // Success
    if (nullptr != m_buffer)
      {
        m_width = width;
        m_height = height;
        if (rename || m_name.empty())
          {
            m_name = filename;
          }
        LOGI("Successfuly load picture file '%s'", filename);
      }
    else
      {
        LOGES("Failed loading picture file '%s'", filename);
      }
    m_need_update = (nullptr != m_buffer);
  }

  inline uint32_t width() const
  {
    return m_width;
  }

  inline uint32_t height() const
  {
    return m_height;
  }

protected:

  virtual void setup() override
  {
    GLTexture::setup();
    glCheck(glBindTexture(m_target, m_handle));
    glCheck(glTexImage2D(m_target, 0, m_gpu_format, m_width, m_height,
                         0, m_cpu_format, m_type, nullptr));
    m_need_setup = false;
  }

  virtual void update() override
  {
    if (isActivable())
      {
        glCheck(glBindTexture(m_target, m_handle));
        glCheck(glTexSubImage2D(m_target, 0, 0, 0, m_width, m_height,
                                m_cpu_format, m_type, nullptr));
        m_need_update = false;
      }
  };

  uint32_t m_width = 0;
  uint32_t m_height = 0;
  GLint m_cpu_format = GL_RGB;
  GLenum m_gpu_format = GL_RGB;
  GLenum m_type = GL_UNSIGNED_BYTE;
  unsigned char* m_buffer = nullptr;
};

// **************************************************************
//!
// **************************************************************
class GLTextureDepth2D: public GLTexture2D
{
  GLTextureDepth2D()
    : GLTexture2D()
  {
    m_gpu_format = GL_DEPTH_COMPONENT;
    m_cpu_format = GL_DEPTH_COMPONENT;
  }

  GLTextureDepth2D(std::string const& name)
    : GLTexture2D(name)
  {
    m_gpu_format = GL_DEPTH_COMPONENT;
    m_cpu_format = GL_DEPTH_COMPONENT;
  }
};

#endif /* GLTEXTURES_HPP_ */
