//=====================================================================
// SimTaDyn: A GIS in a spreadsheet.
// Copyright 2018 Quentin Quadrat <lecrapouille@gmail.com>
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

#ifndef GL_VERTEX_ARRAY_HPP_
#  define GL_VERTEX_ARRAY_HPP_

#  include "IGLObject.hpp"

class GLVAO: public IGLObject<GLenum>
{
public:

  //! \brief Empty constructor without name
  GLVAO()
    : IGLObject()
  {
  }

  //! \brief Constructor with the object name
  GLVAO(std::string const& name)
    : IGLObject(name)
  {
  }

  //! \brief Constructor with the object name
  GLVAO(const char *name)
    : IGLObject(name)
  {
  }

  virtual ~GLVAO() override { destroy(); }

private:

  virtual bool create() override
  {
    glCheck(glGenVertexArrays(1, &m_handle));
    LOGD("VAO '%s' create %d", name().c_str(), m_handle);
    return false;
  }

  virtual void release() override
  {
    glCheck(glDeleteVertexArrays(1, &m_handle));
  }

  virtual void activate() override
  {
    LOGD("VAO '%s' activate", name().c_str());
    glCheck(glBindVertexArray(m_handle));
  }

  virtual void deactivate() override
  {
    LOGD("VAO '%s' deactivate", name().c_str());
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
