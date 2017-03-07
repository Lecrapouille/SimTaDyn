#ifndef VERTICES_HPP_
#  define VERTICES_HPP_

#  include "Color.hpp"
#  include "Vector3D.hpp"

class Vertex
{
public:
  Vertex() { }

  Vertex(Vector3D const& position,
         Color const& color = Color::White,
         float32_t const size = 5.0f)
    : m_position(position),
      m_color(color),
      m_size(size)
  {
  }

  Vector3D  m_position;
  Color     m_color;
  float32_t m_size;
  //Vector2D  m_texCoord;
};

#endif /* VERTICES_HPP_ */
