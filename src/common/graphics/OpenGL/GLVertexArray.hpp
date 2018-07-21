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
