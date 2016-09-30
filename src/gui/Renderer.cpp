#include "Renderer.hpp"

void Renderer::initialize()
{
  // Camera
  default_camera_.lookAt(static_cast<float32_t>(getScreenWidth()) / 2.0f,
                         static_cast<float32_t>(getScreenHeight()) / 2.0f,
                         static_cast<float32_t>(getScreenWidth()),
                         static_cast<float32_t>(getScreenHeight()));
  current_camera_ = default_camera_;

  // Configure OpenGL
  glCheck(glClearDepth(1.0f));
  glCheck(glEnable(GL_LINE_SMOOTH));
  glCheck(glEnable(GL_BLEND));
  glCheck(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
  glCheck(glHint(GL_LINE_SMOOTH_HINT, GL_DONT_CARE));
  glCheck(glLineWidth(1.0f));
  glCheck(glEnable(GL_ALPHA_TEST));

  // Models, textures and fonts
  //display_list_[Node3D] = create3DModelNode();
  font_list_[0].open(SimTaDynFont::MappedTexture, "../data/Font.tga");
  //font_list_[0].open(SimTaDynFont::SystemFont, "fixed");
}

Renderer::~Renderer()
{
  glDeleteLists(display_list_[Node2D], nolist_);
}

void Renderer::applyViewport(Camera2D& camera)
{/*
int w  = getScreenWidth();
int h  = getScreenHeight();

   GLdouble size;
   GLdouble aspect;

   // Use the whole window.
   glViewport(0, 0, w, h);

   // We are going to do some 2-D orthographic drawing.
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   size = (GLdouble)((w >= h) ? w : h) / 2.0;
   if (w <= h) {
      aspect = (GLdouble)h/(GLdouble)w;
      glOrtho(-size, size, -size*aspect, size*aspect, -100000.0, 100000.0);
   }
   else {
      aspect = (GLdouble)w/(GLdouble)h;
      glOrtho(-size*aspect, size*aspect, -size, size, -100000.0, 100000.0);
   }

   // Make the world and window coordinates coincide so that 1.0 in
   // model space equals one pixel in window space.
   glScaled(aspect, aspect, 1.0);

   // Now determine where to draw things.
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
 */

  const float32_t width  = static_cast<float32_t>(getScreenWidth());
  const float32_t height = static_cast<float32_t>(getScreenHeight());

  glCheck(glViewport(static_cast<int>(0.5f + width  * camera.display_at_x_),
                     static_cast<int>(0.5f + height * camera.display_at_y_),
                     static_cast<int>(0.5f + width  * camera.display_at_width_),
                     static_cast<int>(0.5f + height * camera.display_at_height_)));

  glCheck(glMatrixMode(GL_PROJECTION));
  //glCheck(glLoadMatrixf(camera.getTransform()));
  glCheck(glLoadIdentity());
  glCheck(glOrtho(0.0f, width, 0.0f, height, -1.0f, 1.0f));
  glCheck(glMatrixMode(GL_MODELVIEW));
  glCheck(glLoadIdentity());
}

void Renderer::applyViewport()
{
  applyViewport(current_camera_);
}

void Renderer::selectCamera2D(const Camera2D& camera)
{
  current_camera_ = camera;
}

Camera2D& Renderer::getCamera2D()
{
  return current_camera_;
}

void Renderer::restoreCamera2D()
{
  current_camera_ = default_camera_;
}

void Renderer::clearScreen() const
{
  glCheck(glClearColor(background_color_.r, background_color_.g,
                       background_color_.b, background_color_.a));
  glCheck(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

void Renderer::setBackgroundColor(const Color& color)
{
  background_color_ = color;
}

void Renderer::setRenderStyle(const RenderStyle style)
{
  style_ = style;
}

GLuint Renderer::create3DModelNode(void) const
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

void Renderer::zoomFitPage(const SimTaDynGraph& graph)
{
  const Vector3D graph_dim = graph.bbox.dimension();

  std::cout << "Renderer::zoomFitPage\n";
  current_camera_.zoomFit(static_cast<float32_t>(getScreenWidth()),
                          static_cast<float32_t>(getScreenHeight()),
                          graph_dim.x, graph_dim.y);
  //current_camera_.lookAt(static_cast<float32_t>(getScreenWidth()) / 2.0f,
  //                       static_cast<float32_t>(getScreenHeight()) / 2.0f);
}

void Renderer::draw(const AABB& bbox, const Vector3D& center) const
{
  //glBegin(GL_LINES);
  const Vector3D screen(getScreenWidth() / 2.0f, getScreenHeight() / 2.0f);
  const Vector3D mouse(current_camera_.look_at_x_ / 2.0f, current_camera_.look_at_y_ / 2.0f);
  //const Vector3D center = graph.bbox.centerPoint();

  AABB box(screen + (bbox.bbmin - center) * current_camera_.getZoom(),
           screen + (bbox.bbmax - center) * current_camera_.getZoom());

glRectf(box.bbmin.x, box.bbmin.y, box.bbmax.x, box.bbmax.y);
/*
  glVertex3f(box.bbmin.x, box.bbmin.y, box.bbmin.z);
  glVertex3f(box.bbmax.x, box.bbmin.y, box.bbmin.z);
  glVertex3f(box.bbmin.x, box.bbmin.y, box.bbmin.z);
  glVertex3f(box.bbmin.x, box.bbmax.y, box.bbmin.z);
  glVertex3f(box.bbmax.x, box.bbmax.y, box.bbmin.z);
  glVertex3f(box.bbmax.x, box.bbmin.y, box.bbmin.z);
  glVertex3f(box.bbmax.x, box.bbmax.y, box.bbmin.z);
  glVertex3f(box.bbmin.x, box.bbmax.y, box.bbmin.z);

  glVertex3f(box.bbmin.x, box.bbmin.y, box.bbmin.z);
  glVertex3f(box.bbmin.x, box.bbmin.y, box.bbmax.z);
  glVertex3f(box.bbmin.x, box.bbmax.y, box.bbmin.z);
  glVertex3f(box.bbmin.x, box.bbmax.y, box.bbmax.z);
  glVertex3f(box.bbmax.x, box.bbmin.y, box.bbmin.z);
  glVertex3f(box.bbmax.x, box.bbmin.y, box.bbmax.z);
  glVertex3f(box.bbmax.x, box.bbmax.y, box.bbmin.z);
  glVertex3f(box.bbmax.x, box.bbmax.y, box.bbmax.z);

  glVertex3f(box.bbmin.x, box.bbmin.y, box.bbmax.z);
  glVertex3f(box.bbmax.x, box.bbmin.y, box.bbmax.z);
  glVertex3f(box.bbmin.x, box.bbmin.y, box.bbmax.z);
  glVertex3f(box.bbmin.x, box.bbmax.y, box.bbmax.z);
  glVertex3f(box.bbmax.x, box.bbmax.y, box.bbmax.z);
  glVertex3f(box.bbmax.x, box.bbmin.y, box.bbmax.z);
  glVertex3f(box.bbmax.x, box.bbmax.y, box.bbmax.z);
  glVertex3f(box.bbmin.x, box.bbmax.y, box.bbmax.z);
*/
  //glEnd();
  //std::cout << "LOL2 " << bbox << "  " << box << std::endl;
}

void Renderer::draw(const RTreeNode* root, const Vector3D& center) const
{
  RTreeNode *node;
  uint32_t i;

  for (i = 0; i < RTREE_MAX_NODES; i++)
    {
      glColor3f(0.0f, 1.0f - root->level / 5.0f, 0.0f);
      draw(root->branch[i].box, center);
    }

  if (!IS_A_RTREE_LEAF(root->level))
    {
      for (i = 0; i < RTREE_MAX_NODES; i++)
        {
          node = root->branch[i].child;
          if (NULL != node)
            {
              draw(node, center);
            }
        }
    }
}

void Renderer::draw(/*const*/ SimTaDynGraph& graph) //const
{
  std::map<Key, SimTaDynCell*>::const_iterator it;

  const Vector3D screen(getScreenWidth() / 2.0f, getScreenHeight() / 2.0f);
  const Vector3D mouse(current_camera_.look_at_x_ / 2.0f, current_camera_.look_at_y_ / 2.0f);
  const Vector3D center = graph.bbox.centerPoint();
  Position3D position;

  AABB bbox = graph.bbox;

  // Map
  //bbox.bbmin = screen + (bbox.bbmin - center) * current_camera_.getZoom();
  //bbox.bbmax = screen + (bbox.bbmax - center) * current_camera_.getZoom();
  //glColor3f(0.1f, 0.0f, 0.0f);
  //glRectf(bbox.bbmin.x, bbox.bbmin.y, bbox.bbmax.x, bbox.bbmax.y);


  /*SimTaDynCell *c = graph.getNode(1);
  bbox.bbmin = screen + (c->box_.bbmin - center) * current_camera_.getZoom();
  bbox.bbmax = screen + (c->box_.bbmax - center) * current_camera_.getZoom();
  glColor3f(0.0f, 1.0f, 1.0f);
  glRectf(bbox.bbmin.x, bbox.bbmin.y, bbox.bbmax.x, bbox.bbmax.y);*/

  //glColor3f(0.0f, 1.0f, 0.0f);
  //position = screen + (c->getPosition() - center) * current_camera_.getZoom();
  //glRectf(position.x -1.0f, position.y -1.0f, position.x +1.0f, position.y + 1.0f);

  glColor3f(0.0f, 1.0f, 1.0f);
  //glBegin(GL_LINES);
  draw(graph.rtree, center);
  //glEnd();

  // Draw all nodes
  glColor3f(1.0f, 0.0f, 0.0f);
  for (it = graph.nodes_.begin(); it != graph.nodes_.end(); ++it)
    {
      position = screen + (it->second->getPosition() - center) * current_camera_.getZoom();
      glRectf(position.x -1.0f, position.y -1.0f, position.x +1.0f, position.y + 1.0f);
      //glPushMatrix();
      //glTranslatef(position.x, position.y, 0.0f);
      //font_list_[0].draw(it->second->name);
      //glPopMatrix();
    }

  // Graph name
  font_list_[0].draw(graph.name);
  //std::cout << "-----------------------------------------------\n";
  //std::cout << *(graph.rtree) << std::endl;


  // Draw all zones
  // Draw all arcs
  // Draw all nodes
 /* glColor3f(0.0f, 0.0f, 1.0f);
  for (it = graph.nodes_.begin(); it != graph.nodes_.end(); ++it)
    {
      //font_list[0].setText(it->second->name.c_str());
      position = screen + (it->second->getPosition() - center) * current_camera_.getZoom();

      glPushMatrix();
      glTranslated(position.x, position.y, 0.0f);
      switch (style_)
        {
        case Low2D:
          glCallList(display_list_[0]);
          break;
        default:
          glRectf(1.0f, -1.0f, -1.0f, 1.0f);
          break;
        }
      glPopMatrix();
      }*/
}
