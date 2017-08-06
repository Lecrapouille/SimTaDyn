//=====================================================================
// SimTaDyn: A GIS in a spreadsheet.
// Copyright 2017 Quentin Quadrat <lecrapouille@gmail.com>
//
// This file is part of SimTaDyn.
//
// SimTaDyn is free software: you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with GNU Emacs.  If not, see <http://www.gnu.org/licenses/>.
//=====================================================================

#ifndef GLTEXTURES_HPP_
#  define GLTEXTURES_HPP_

#  include "GLObject.hpp"
#  include "PendingData.hpp"
#  include <SOIL/SOIL.h>

// **************************************************************
//
// **************************************************************
class GLTexture: public GLObject, protected PendingData
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

  //! \brief Destructor: release data from the GPU and CPU memory.
  virtual ~GLTexture()
  {
    destroy();
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

  virtual bool create() override
  {
    glCheck(glGenTextures(1U, &m_handle));
    return false;
  }

  virtual void release() override
  {
    glCheck(glDeleteTextures(1U, &m_handle));
  }

  virtual void activate() override
  {
    glCheck(glBindTexture(m_target, m_handle));
  }

  virtual void deactivate() override
  {
    glCheck(glBindTexture(m_target, 0U));
  }

  virtual bool setup() override
  {
    activate();
    glCheck(glTexParameterf(m_target, GL_TEXTURE_MIN_FILTER, m_min_filter));
    glCheck(glTexParameterf(m_target, GL_TEXTURE_MAG_FILTER, m_mag_filter));
    glCheck(glTexParameterf(m_target, GL_TEXTURE_WRAP_S, m_wrapping));
    glCheck(glTexParameterf(m_target, GL_TEXTURE_WRAP_T, m_wrapping));
    glCheck(glTexParameterf(m_target, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));
    return false;
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

  inline bool load(std::string const& filename, const bool rename = false)
  {
    return load(filename.c_str(), rename);
  }

  bool load(const char *const filename, const bool rename = true)
  {
    int width, height;
    bool res;

    LOGI("Loading texture '%s'", filename);
    if (nullptr != m_buffer)
      {
        SOIL_free_image_data(m_buffer);
      }
    m_buffer = SOIL_load_image(filename, &width, &height, 0, SOIL_LOAD_RGBA);

    // Success
    if (nullptr != m_buffer)
      {
        m_width = width;
        m_height = height;
        if (rename || m_name.empty())
          {
            m_name = filename;
          }
        PendingData::addPendingData(0U, m_width * m_height);
        LOGI("Successfuly load picture file '%s'", filename);
        res = true;
      }
    else
      {
        LOGES("Failed loading picture file '%s'", filename);
        res = false;
      }
    m_need_update = (nullptr != m_buffer);
    return res;
  }

  inline unsigned char& operator[](size_t nth)
  {
    //TBD ?
    //if (nth > m_width * m_height)
    //  {
    //    reserve(nth);
    //  }
    PendingData::addPendingData(nth);
    return m_buffer[nth];
  }

  inline const unsigned char& operator[](size_t nth) const
  {
    return m_buffer[nth];
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

  virtual bool setup() override
  {
    bool b = GLTexture::setup();
    glCheck(glTexImage2D(m_target, 0, m_gpu_format, m_width, m_height,
                         0, m_cpu_format, m_type, m_buffer));
    return b;
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

    // FIXME: TODO pendingData --> x,y,width,height
    const uint32_t x = 0U;
    const uint32_t y = 0U;
    const uint32_t width = m_width;
    const uint32_t height = m_height;

    glCheck(glBindTexture(m_target, m_handle));
    glCheck(glTexSubImage2D(m_target, 0, x, y, width, height,
                            m_cpu_format, m_type, m_buffer));

    PendingData::clearPending();
    return false;
  };

  uint32_t m_width = 0;
  uint32_t m_height = 0;
  GLint m_cpu_format = GL_RGBA;
  GLenum m_gpu_format = GL_RGBA;
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
