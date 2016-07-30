#ifndef MAIN_H
#define MAIN_H

#include <stdlib.h>
#include <stdio.h>

#include <gtk/gtk.h>
#include <gtk/gtkgl.h>

#include <math.h>
#include "opgl.h"
#include "memoire.h"
#include "store.h"
#include "affichage.h"
#include "interface.h"
#include "support.h"
#include "header.h"
#include "mot.h"
#include "string_util.h"
#include "hash.h"
#include "pile.h"
#include "dictionnaire.h"
#include "interpreteur.h"
#include "fichier.h"

void
on_drawingarea_realize                 (GtkWidget       *widget,
                                        gpointer         user_data);

gboolean
on_drawingarea_configure_event         (GtkWidget       *widget,
                                        GdkEventConfigure *event,
                                        gpointer         user_data);

gboolean
on_drawingarea_expose_event            (GtkWidget       *widget,
                                        GdkEventExpose  *event,
                                        gpointer         user_data);
gboolean
on_drawingarea_button_press_event            (GtkWidget       *widget,
					      GdkEventButton  *event,
					      gpointer         user_data);
gboolean on_drawingarea_key_press_event (GtkWidget   *widget,
					 GdkEventKey *event,
					 gpointer     user_data);
gboolean on_drawingarea_motion_notify (GtkWidget   *widget,
				       GdkEventMotion *event,
				       gpointer     user_data);
gboolean on_drawingarea_button_release (GtkWidget   *widget,
					GdkEventButton *event,
					gpointer     user_data);

void
selection_changed (GtkTreeSelection *selection);

#endif /* MAIN_H */
