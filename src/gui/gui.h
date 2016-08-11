#ifndef GUI_H_
#  define GUI_H_

#  ifdef __APPLE__
#    include <GLUT/glut.h>
#    include <OpenGL/gl.h>
#    include <OpenGL/glu.h>
#  else
#    include <GL/glut.h>
#    include <GL/gl.h>
#    include <GL/glu.h>
#  endif

#  ifdef HAVE_CONFIG_H
#    include <config.h>
#  endif

#  include <gdk/gdkkeysyms.h>
#  include <gtk/gtk.h>
#  include <gtk/gtkgl.h>
#  include <gdk/gdkgl.h>

#define GLADE_HOOKUP_OBJECT(component,widget,name) \
  g_object_set_data_full (G_OBJECT (component), name, \
    gtk_widget_ref (widget), (GDestroyNotify) gtk_widget_unref)

#define GLADE_HOOKUP_OBJECT_NO_REF(component,widget,name) \
  g_object_set_data (G_OBJECT (component), name, widget)

/* Standard gettext macros. */
#ifdef ENABLE_NLS
#  include <libintl.h>
#  undef _
#  define _(String) dgettext (PACKAGE, String)
#  ifdef gettext_noop
#    define N_(String) gettext_noop (String)
#  else
#    define N_(String) (String)
#  endif
#else
#  define textdomain(String) (String)
#  define gettext(String) (String)
#  define dgettext(Domain,Message) (Message)
#  define dcgettext(Domain,Message,Type) (Message)
#  define bindtextdomain(Domain,Directory) (Domain)
#  define _(String) (String)
#  define N_(String) (String)
#endif

void start_gui(int argc, char *argv[]);

#endif /* GUI_H_ */
