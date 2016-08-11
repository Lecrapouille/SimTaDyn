#ifndef OPGL_H_
#  define OPGL_H_

#  include "gui.h"

#  define DISTANCE_CLIPPING 100000.0f
#  define THICKNESS 5.0f

void opgl_realize(GtkWidget *widget, gpointer user_data);
gboolean opgl_reshape(GtkWidget         *widget,
                      GdkEventConfigure *event,
                      gpointer           user_data);
gboolean opgl_draw(GtkWidget       *widget,
                   GdkEventExpose  *event,
                   gpointer         user_data);

#endif /* OPGL_H_ */
