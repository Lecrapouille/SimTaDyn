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
  glCheck(glLineWidth(10));
  glCheck(glEnable(GL_ALPHA_TEST));

  // Models, textures and fonts
  display_list_[Node3D] = create3DModelNode();
  font_list_[0].loadTexture("../data/Font.tga");
}

Renderer::~Renderer()
{
  glDeleteLists(display_list_[Node2D], nolist_);
}

void Renderer::applyViewport(Camera2D& camera)
{
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

void Renderer::draw(const RTreeNode& root) const
{

}

void Renderer::draw(const SimTaDynGraph& graph) const
{
  std::map<Key, SimTaDynCell*>::const_iterator it;

  const Vector3D screen(getScreenWidth() / 2.0f, getScreenHeight() / 2.0f);
  const Vector3D mouse(current_camera_.look_at_x_ / 2.0f, current_camera_.look_at_y_ / 2.0f);
  const Vector3D center = graph.bbox.centerPoint();
  Position3D position;
  AABB bbox = graph.bbox;

  // bbox
  glPushMatrix();
  bbox.bbmin = screen + (bbox.bbmin - center) * current_camera_.getZoom();
  bbox.bbmax = screen + (bbox.bbmax - center) * current_camera_.getZoom();
  glColor3f(0.1f, 0.0f, 0.0f);
  glRectf(bbox.bbmin.x, bbox.bbmin.y, bbox.bbmax.x, bbox.bbmax.y);
  glPopMatrix();

  // Draw all zones
  // Draw all arcs
  // Draw all nodes
  glColor3f(0.0f, 0.0f, 1.0f);
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
    }
}
