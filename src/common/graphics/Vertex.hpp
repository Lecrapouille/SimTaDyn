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
