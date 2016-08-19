#include "draw.hpp"

#if 0
// Create a display list of a 3D handle
static GLuint poignee3D(void)
{
  GLuint index;
  GLUquadricObj *qobj;

  qobj = gluNewQuadric();
  gluQuadricDrawStyle(qobj, GLU_FILL);
  gluQuadricNormals(qobj, GLU_NONE); // GLU_FLAT GLU_SMOOTH);

  index = glGenLists(1);
  if (0 != index)
    {
      glNewList(index, GL_COMPILE);
      glScalef(10.0f, 10.0f, 10.0f);
      gluSphere(qobj, 1.0f, 40.0f, 40.0f);
      glEndList();
    }
  else
    {
      fprintf(stderr, "%s(): Failed when creating a display list\n", __func__);
    }

  gluDeleteQuadric(qobj);
  return index;
}

/**************************************************************************
 *
 **************************************************************************/
void opgl_3d_mode(GtkWidget *widget)
{
  GLfloat aspect;

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  if (widget->allocation.width > widget->allocation.height)
    {
      aspect = widget->allocation.width / widget->allocation.height;
      glFrustum -aspect, aspect, -1.0f, 1.0f, 5.0f, DISTANCE_CLIPPING);
    }
  else
    {
      aspect = widget->allocation.height / widget->allocation.width;
      glFrustum(-1.0f, 1.0f, -aspect, aspect, 5.0f, DISTANCE_CLIPPING);
    }
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  //gluLookAt (camera.position.x, camera.position.y, camera.position.z,
  //         camera.cible.x, camera.cible.y, camera.cible.z,
  //         camera.orientation.x, camera.orientation.y, camera.orientation.z);
}
#endif

/**************************************************************************
 *
 **************************************************************************/
static void opgl_2d_mode(GtkWidget *widget)
{
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0.0f, widget->allocation.width, 0.0f,
          widget->allocation.height, -1.0f,
          DISTANCE_CLIPPING);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

// Create a display list of a 2D handle
static GLuint poignee2D(void)
{
  GLuint index;

  index = glGenLists(1);
  if (0 != index)
    {
      glNewList(index, GL_COMPILE);
      glRecti(THICKNESS, -THICKNESS, -THICKNESS, THICKNESS);
      glEndList();
    }
  else
    {
      fprintf(stderr, "%s(): Failed when creating a display list\n", __func__);
    }
  return index;
}

// Draw a display list (handle) at given position
static void draw_displaylist_at(GLfloat x, GLfloat y, GLfloat z, GLuint l)
{
  glPushMatrix();
  glTranslated(x, y, z);
  glCallList(l);
  glPopMatrix();
}
