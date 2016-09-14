#ifndef DRAWGRAPH_HPP_
#  define DRAWGRAPH_HPP_

#  include "Fonts.hpp"
#  include "SimTaDynGraphs.hpp"

class DrawGraph
{
public:
  enum DrawStyle
    {
      FAST,
      MEDIUM,
      LOW
    };

  DrawGraph()
  {
    style_ = FAST;
    display_list[0] = node3D();
    font_list[0].loadTexture("../data/Font.tga");
  }

  ~DrawGraph()
  {
    glDeleteLists(display_list[0], 1);
  }

  void setDrawStyle(const DrawStyle style)
  {
    style_ = style;
  }

  void draw(SimTaDynGraph& graph)
  {
    std::map<Key, SimTaDynCell*>::iterator it;
    Position3D p;

    font_list[0].setText("SimTaDyn");

    glPushMatrix();
    glTranslated(0, 0, 0.0f);
    glScaled(0.0, 0.0, -1.0);
    font_list[0].draw();
    glPopMatrix();

    switch (style_)
      {
      case MEDIUM:
      case FAST:
        // Draw all zones
        // Draw all arcs
        // Draw all nodes
        for (it = graph.nodes_.begin(); it != graph.nodes_.end(); ++it)
          {
            //font_list[0].setText(it->second->name.c_str());
            p = it->second->getPosition();

            glPushMatrix();
            glTranslated(p.x, p.y, 0.0f);
            //font_list[0].draw();
            glRecti(1.0f, -1.0f, -1.0f, 1.0f);
            glPopMatrix();
          }
        break;
      case LOW:
        for (it = graph.nodes_.begin(); it != graph.nodes_.end(); ++it)
          {
            //font_list[0].setText(it->second->name.c_str());
            p = it->second->getPosition();

            glPushMatrix();
            glTranslated(p.x, p.y, 0.0f);
            //font_list[0].draw();
            glCallList(display_list[0]);
            glPopMatrix();
          }
        break;
      default:
        break;
      }
  }
protected:
  GLuint node3D(void)
  {
    GLuint index;
    GLUquadricObj *qobj;

    qobj = gluNewQuadric();
    gluQuadricDrawStyle(qobj, GLU_FILL);
    gluQuadricNormals(qobj, GLU_NONE); // GLU_FLAT GLU_SMOOTH);

    index = glCheck(glGenLists(1));
    if (0 != index)
      {
        glCheck(glNewList(index, GL_COMPILE));
        glCheck(glScalef(10.0f, 10.0f, 10.0f));
        gluSphere(qobj, 1.0f, 40.0f, 40.0f);
        glCheck(glEndList());
      }
    else
      {
        std::cerr << "DrawGraph::node3D(): Failed when creating a display list" << std::endl;
      }

    gluDeleteQuadric(qobj);
    return index;
  }

  GLuint display_list[1];
  Font font_list[1];
  DrawStyle style_;
};

#endif /* DRAWGRAPH_HPP_ */
