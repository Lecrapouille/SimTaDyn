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
// along with SimTaDyn.  If not, see <http://www.gnu.org/licenses/>.
//=====================================================================

#ifndef VERTICES_HPP_
#  define VERTICES_HPP_

#  include "Color.hpp"
#  include "Vector.tpp"

class Vertex
{
public:
  Vertex() { }

  Vertex(Vector3f const& position,
         Color const& color = Color::White,
         float const size = 5.0f)
    : m_position(position),
      m_color(color),
      m_size(size)
  {
  }

  Vector3f  m_position;
  Color     m_color;
  float m_size;
  //Vector2f  m_texCoord;
};

#endif /* VERTICES_HPP_ */
