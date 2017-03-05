#ifndef VERTICES_HPP_
#  define VERTICES_HPP_

#  include "Color.hpp"
#  include "Vector3D.hpp"

class Vertex
{
public:

  Vector3D  m_position;
  Color     m_color;
  float32_t m_size;
  //Vector2D  m_texCoord;
};

#endif /* VERTICES_HPP_ */
