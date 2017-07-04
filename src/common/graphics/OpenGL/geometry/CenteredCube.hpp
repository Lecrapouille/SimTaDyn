#ifndef CUBE_HPP
#define CUBE_HPP

#  include "Mesh.hpp"

class MeshCube: public Mesh
{
public:

  MeshCube()
    : Mesh()
  {
    const GLfloat vertexData[] = {
      //  X     Y     Z
      // bottom
      -0.5f,-0.5f,-0.5f,
      0.5f,-0.5f,-0.5f,
      -0.5f,-0.5f, 0.5f,
      0.5f,-0.5f,-0.5f,
      0.5f,-0.5f, 0.5f,
      -0.5f,-0.5f, 0.5f,

      // top
      -0.5f, 0.5f,-0.5f,
      -0.5f, 0.5f, 0.5f,
      0.5f, 0.5f,-0.5f,
      0.5f, 0.5f,-0.5f,
      -0.5f, 0.5f, 0.5f,
      0.5f, 0.5f, 0.5f,

      // front
      -0.5f,-0.5f, 0.5f,
      0.5f,-0.5f, 0.5f,
      -0.5f, 0.5f, 0.5f,
      0.5f,-0.5f, 0.5f,
      0.5f, 0.5f, 0.5f,
      -0.5f, 0.5f, 0.5f,

      // back
      -0.5f,-0.5f,-0.5f,
      -0.5f, 0.5f,-0.5f,
      0.5f,-0.5f,-0.5f,
      0.5f,-0.5f,-0.5f,
      -0.5f, 0.5f,-0.5f,
      0.5f, 0.5f,-0.5f,

      // left
      -0.5f,-0.5f, 0.5f,
      -0.5f, 0.5f,-0.5f,
      -0.5f,-0.5f,-0.5f,
      -0.5f,-0.5f, 0.5f,
      -0.5f, 0.5f, 0.5f,
      -0.5f, 0.5f,-0.5f,

      // right
      0.5f,-0.5f, 0.5f,
      0.5f,-0.5f,-0.5f,
      0.5f, 0.5f,-0.5f,
      0.5f,-0.5f, 0.5f,
      0.5f, 0.5f,-0.5f,
      0.5f, 0.5f, 0.5f,
    };

    const GLfloat textureData[] = {
      //  U     V
      // bottom
      0.0f, 0.0f,
      1.0f, 0.0f,
      0.0f, 1.0f,
      1.0f, 0.0f,
      1.0f, 1.0f,
      0.0f, 1.0f,

      // top
      0.0f, 0.0f,
      0.0f, 1.0f,
      1.0f, 0.0f,
      1.0f, 0.0f,
      0.0f, 1.0f,
      1.0f, 1.0f,

      // front
      1.0f, 0.0f,
      0.0f, 0.0f,
      1.0f, 1.0f,
      0.0f, 0.0f,
      0.0f, 1.0f,
      1.0f, 1.0f,

      // back
      0.0f, 0.0f,
      0.0f, 1.0f,
      1.0f, 0.0f,
      1.0f, 0.0f,
      0.0f, 1.0f,
      1.0f, 1.0f,

      // left
      0.0f, 1.0f,
      1.0f, 0.0f,
      0.0f, 0.0f,
      0.0f, 1.0f,
      1.0f, 1.0f,
      1.0f, 0.0f,

      // right
      1.0f, 1.0f,
      1.0f, 0.0f,
      0.0f, 0.0f,
      1.0f, 1.0f,
      0.0f, 0.0f,
      0.0f, 1.0f
    };

    m_pos.add(vertexData, ARRAY_SIZE(vertexData));
    m_tex.add(textureData, ARRAY_SIZE(textureData));
  }

  inline void draw() { Mesh::draw(GL_TRIANGLES); }
};

#endif
