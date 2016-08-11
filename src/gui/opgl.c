#include "opgl.h"

static GLuint poignee;

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
static void opgl_3d_mode(GtkWidget *widget)
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

/**************************************************************************
 *
 **************************************************************************/
void opgl_realize(GtkWidget *widget, gpointer user_data)
{
  (void) user_data;
  GdkGLContext *glcontext = gtk_widget_get_gl_context(widget);
  GdkGLDrawable *gldrawable = gtk_widget_get_gl_drawable(widget);

  if (!gdk_gl_drawable_gl_begin(gldrawable, glcontext))
    {
      g_print("opgl_realize: failed !\n");
      return;
    }

  //guts.width = widget->allocation.width;
  //guts.height = widget->allocation.height;
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  glClearDepth(1.0f);

  glEnable(GL_LINE_SMOOTH);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glHint(GL_LINE_SMOOTH_HINT, GL_DONT_CARE);
  glLineWidth(1.5f);
  glEnable(GL_ALPHA_TEST);
  opgl_2d_mode(widget);

  poignee = poignee2D();

  gdk_gl_drawable_gl_end(gldrawable);
}

/**************************************************************************
 * New window size or exposure
 **************************************************************************/
gboolean opgl_reshape(GtkWidget         *widget,
                      GdkEventConfigure *event,
                      gpointer           user_data)
{
  (void) event;
  (void) user_data;

  GdkGLContext *glcontext = gtk_widget_get_gl_context(widget);
  GdkGLDrawable *gldrawable = gtk_widget_get_gl_drawable(widget);

  //guts.width = widget->allocation.width;
  //guts.height = widget->allocation.height;

  if (!gdk_gl_drawable_gl_begin(gldrawable, glcontext))
    {
      g_print("opgl_reshape: failed !\n");
      return FALSE;
    }

  glViewport(0.0f, 0.0f, widget->allocation.width, widget->allocation.height);
  opgl_2d_mode(widget);

  gdk_gl_drawable_gl_end (gldrawable);
  return TRUE;
}

/**************************************************************************
 * New window size or exposure
 **************************************************************************/
gboolean opgl_draw(GtkWidget       *widget,
                   GdkEventExpose  *event,
                   gpointer         user_data)
{
  (void) event;
  (void) user_data;

  GdkGLContext *glcontext = gtk_widget_get_gl_context (widget);
  GdkGLDrawable *gldrawable = gtk_widget_get_gl_drawable (widget);

  if (!gdk_gl_drawable_gl_begin (gldrawable, glcontext))
    {
      g_print("opgl_draw failed !\n");
      return FALSE;
    }

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();

  //dessiner_la_carte();
  draw_displaylist_at(300.0f, 300.0f, 0.0f, poignee);
  draw_displaylist_at(400.0f, 300.0f, 0.0f, poignee);
  draw_displaylist_at(300.0f, 400.0f, 0.0f, poignee);

  /* Swap buffers */
  if (gdk_gl_drawable_is_double_buffered(gldrawable))
    gdk_gl_drawable_swap_buffers (gldrawable);
  else
    glFlush ();

  gdk_gl_drawable_gl_end (gldrawable);
  return TRUE;
}
