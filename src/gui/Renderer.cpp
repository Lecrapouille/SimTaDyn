#include "Renderer.hpp"

void Renderer::initialize()
{
  // Camera
  default_camera_.cameraFitScreen(0.0f, 0.0f,
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
  float32_t width  = static_cast<float32_t>(getScreenWidth());
  float32_t height = static_cast<float32_t>(getScreenHeight());
  uint32_t rect[4];

  rect[0] = static_cast<int>(0.5f + width  * camera.display_at_x_);
  rect[1] = static_cast<int>(0.5f + height * camera.display_at_y_);
  rect[2] = static_cast<int>(0.5f + width  * camera.display_at_width_);
  rect[3] = static_cast<int>(0.5f + height * camera.display_at_height_);

  glCheck(glViewport(rect[0], rect[1], rect[2], rect[3]));
  glCheck(glMatrixMode(GL_PROJECTION));
  glCheck(glLoadMatrixf(camera.getTransform()));
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

void Renderer::draw(const RTreeNode& root) const
{

}

void Renderer::draw(const SimTaDynGraph& graph) const
{
  std::map<Key, SimTaDynCell*>::const_iterator it;
  Position3D p;
  Vector3D dim = graph.bbox.dimension();
  float32_t scale = std::min(getScreenWidth() / dim.x, getScreenHeight() / dim.y);
  std::cout << "scale " << scale << std::endl;

  glPushMatrix();
  glTranslated(getScreenWidth() / 2, getScreenHeight() / 2, 0.0f);
  glScaled(scale, scale, 0);
  switch (style_)
    {
    case Medium2D:
    case Fast2D:
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
    case Low2D:
      for (it = graph.nodes_.begin(); it != graph.nodes_.end(); ++it)
        {
          //font_list[0].setText(it->second->name.c_str());
          p = it->second->getPosition();

          glPushMatrix();
          glTranslated(p.x, p.y, 0.0f);
          //font_list[0].draw();
          glCallList(display_list_[0]);
          glPopMatrix();
        }
      break;
    default:
      break;
    }

  glPopMatrix();
}

//void viewPort();
