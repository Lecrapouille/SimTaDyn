#include "opgl.h"

static GList *pixmaps_directories = NULL;

/**************************************************************************
 *
 **************************************************************************/
static GtkWidget* lookup_widget(GtkWidget *widget, const gchar *widget_name)
{
  GtkWidget *parent, *found_widget;

  for (;;)
    {
      if (GTK_IS_MENU(widget))
        parent = gtk_menu_get_attach_widget(GTK_MENU (widget));
      else
        parent = widget->parent;
      if (!parent)
        parent = (GtkWidget*) g_object_get_data(G_OBJECT(widget), "GladeParentKey");
      if (parent == NULL)
        break;
      widget = parent;
    }

  found_widget = (GtkWidget*) g_object_get_data(G_OBJECT (widget), widget_name);
  if (!found_widget)
    g_warning("Widget not found: %s", widget_name);

  return found_widget;
}

/**************************************************************************
 *
 **************************************************************************/
static void add_pixmap_directory(const gchar *directory)
{
  pixmaps_directories = g_list_prepend(pixmaps_directories, g_strdup (directory));
}

#if 0
/**************************************************************************
 *
 **************************************************************************/
static gchar* find_pixmap_file(const gchar *filename)
{
  GList *elem;

  /* We step through each of the pixmaps directory to find it. */
  elem = pixmaps_directories;
  while (elem)
    {
      gchar *pathname = g_strdup_printf ("%s%s%s", (gchar*)elem->data,
                                         G_DIR_SEPARATOR_S, filename);
      if (g_file_test (pathname, G_FILE_TEST_EXISTS))
        return pathname;
      g_free (pathname);
      elem = elem->next;
    }
  return NULL;
}

/**************************************************************************
 *
 **************************************************************************/
GtkWidget* create_pixmap(GtkWidget *widget, const gchar *filename)
{
  gchar *pathname = NULL;
  GtkWidget *pixmap;

  if (!filename || !filename[0])
      return gtk_image_new ();

  pathname = find_pixmap_file (filename);

  if (!pathname)
    {
      g_warning (_("Couldn't find pixmap file: %s"), filename);
      return gtk_image_new ();
    }

  pixmap = gtk_image_new_from_file (pathname);
  g_free (pathname);
  return pixmap;
}

/**************************************************************************
 *
 **************************************************************************/
GdkPixbuf* create_pixbuf(const gchar *filename)
{
  gchar *pathname = NULL;
  GdkPixbuf *pixbuf;
  GError *error = NULL;

  if (!filename || !filename[0])
      return NULL;

  pathname = find_pixmap_file (filename);

  if (!pathname)
    {
      g_warning (_("Couldn't find pixmap file: %s"), filename);
      return NULL;
    }

  pixbuf = gdk_pixbuf_new_from_file (pathname, &error);
  if (!pixbuf)
    {
      fprintf (stderr, "Failed to load pixbuf file: %s: %s\n",
               pathname, error->message);
      g_error_free (error);
    }
  g_free (pathname);
  return pixbuf;
}

/**************************************************************************
 *
 **************************************************************************/
void glade_set_atk_action_description(AtkAction       *action,
                                      const gchar     *action_name,
                                      const gchar     *description)
{
  gint n_actions, i;

  n_actions = atk_action_get_n_actions (action);
  for (i = 0; i < n_actions; i++)
    {
      if (!strcmp (atk_action_get_name (action, i), action_name))
        atk_action_set_description (action, i, description);
    }
}
#endif

/**************************************************************************
 *
 **************************************************************************/
static GtkWidget* create_main_window(void)
{
  /* Main frame */
  GtkWidget *win_main = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_widget_set_size_request(win_main, 800, 600);
  gtk_window_set_title(GTK_WINDOW(win_main), _("SimTaDyn"));
  gtk_window_set_position(GTK_WINDOW(win_main), GTK_WIN_POS_CENTER);

  /* hpaned1 left side */
  GtkWidget *vbox_drawing = gtk_vbox_new(FALSE, 0);
  gtk_container_add(GTK_CONTAINER(win_main), vbox_drawing);

  GtkWidget *drawingarea = gtk_drawing_area_new();
  gtk_box_pack_start(GTK_BOX(vbox_drawing), drawingarea, TRUE, TRUE, 0);
  /* Store pointers to all widgets, for use by lookup_widget(). */
  GLADE_HOOKUP_OBJECT(win_main, drawingarea, "drawingarea");

  GtkWidget *button = gtk_button_new_with_label ("Quit");
  gtk_box_pack_start(GTK_BOX(vbox_drawing), button, FALSE, FALSE, 0);
  g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(gtk_main_quit), NULL);

  return win_main;
}

/**************************************************************************
 *
 **************************************************************************/
void start_gui(int argc, char *argv[])
{
  gint major, minor;

  gtk_set_locale();
  gtk_init(&argc, &argv);
  gtk_gl_init(&argc, &argv);
  add_pixmap_directory("../data/pixmaps");

  /* Configure OpenGL-capable visual. */
  gdk_gl_query_version (&major, &minor);
  g_print("\nOpenGL extension version - %d.%d\n", major, minor);

  /* Try double-buffered visual */
  GdkGLConfig *glconfig = gdk_gl_config_new_by_mode(GDK_GL_MODE_RGB    |
                                                    GDK_GL_MODE_DEPTH  |
                                                    GDK_GL_MODE_DOUBLE);
  if (NULL == glconfig)
    {
      g_print("*** Cannot find the double-buffered visual.\n");
      g_print("*** Trying single-buffered visual.\n");

      /* Try single-buffered visual */
      glconfig = gdk_gl_config_new_by_mode(GDK_GL_MODE_RGB   |
                                           GDK_GL_MODE_DEPTH);
      if (NULL == glconfig)
        {
          g_print("*** No appropriate OpenGL-capable visual found.\n");
          exit (1);
        }
    }
  //examine_gl_config_attrib(glconfig);

  GtkWidget *win_main = create_main_window();
  GtkWidget *drawingarea = lookup_widget(GTK_WIDGET(win_main), "drawingarea");

  /* Set OpenGL-capability to the widget */
  gtk_widget_set_gl_capability(drawingarea, glconfig, NULL, TRUE, GDK_GL_RGBA_TYPE);
  gtk_widget_add_events(drawingarea,
                        GDK_BUTTON1_MOTION_MASK    |
                        GDK_BUTTON2_MOTION_MASK    |
                        GDK_BUTTON3_MOTION_MASK    |
                        GDK_BUTTON_RELEASE_MASK    |
                        GDK_BUTTON_PRESS_MASK);
  /* Signals */
  g_signal_connect(G_OBJECT(win_main), "delete_event", G_CALLBACK(gtk_main_quit), NULL);
  g_signal_connect((gpointer) drawingarea, "realize", G_CALLBACK(opgl_realize), NULL);
  g_signal_connect((gpointer) drawingarea, "configure_event", G_CALLBACK(opgl_reshape), NULL);
  //g_signal_connect((gpointer) drawingarea, "draw", G_CALLBACK(opgl_draw), NULL);
  g_signal_connect((gpointer) drawingarea, "expose_event", G_CALLBACK(opgl_draw), NULL);
  //g_signal_connect((gpointer) drawingarea, "button_press_event", G_CALLBACK(on_drawingarea_button_press_event), NULL);
  //g_signal_connect((gpointer) drawingarea, "motion_notify_event", G_CALLBACK(on_drawingarea_motion_notify), NULL);
  //g_signal_connect((gpointer) drawingarea, "button_release_event", G_CALLBACK(on_drawingarea_button_release), NULL);
  //g_signal_connect_swapped((gpointer) win_main, "key_press_event", G_CALLBACK(on_drawingarea_key_press_event), drawingarea);
  GLADE_HOOKUP_OBJECT(win_main, drawingarea, "drawingarea");

  gtk_widget_show_all(win_main);
  gtk_main();
}
