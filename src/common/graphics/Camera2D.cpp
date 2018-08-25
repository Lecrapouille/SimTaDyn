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

#include "Camera2D.hpp"

const float* Camera2D::getTransform()
{
  float rotation = 0.0f;
  float zoom = 1.0f;
  // Rotation components
  float angle  = rotation * 3.141592654f / 180.f;
  float cosine = static_cast<float>(std::cos(angle));
  float sine   = static_cast<float>(std::sin(angle));
  float tx     = -m_look_at_x * cosine - m_look_at_y * sine + m_look_at_x;
  float ty     =  m_look_at_x * sine - m_look_at_y * cosine + m_look_at_y;

  // Projection components
  float a = 2.f / m_look_at_width * zoom;
  float b = -2.f / m_look_at_height * zoom;
  float c = -a * m_look_at_x;
  float d = -b * m_look_at_y;

  // Rebuild the projection m_matrix
  m_matrix[0] = a * cosine; m_matrix[4] = a * sine;   m_matrix[8]  = 0.f;  m_matrix[12] = a * tx + c;
  m_matrix[1] = -b * sine;  m_matrix[5] = b * cosine; m_matrix[9]  = 0.f;  m_matrix[13] = b * ty + d;
  m_matrix[2] = 0.0f;       m_matrix[6] = 0.0f;       m_matrix[10] = 1.0f; m_matrix[14] = 0.0f;
  m_matrix[3] = 0.0f;       m_matrix[7] = 0.0f;       m_matrix[11] = 0.0f; m_matrix[15] = 1.0f;

  return m_matrix;
}
