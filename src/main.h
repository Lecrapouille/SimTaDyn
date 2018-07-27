//=====================================================================
// SimTaDyn: A GIS in a spreadsheet.
// Copyright 2017 Quentin Quadrat <lecrapouille@gmail.com>
// Copyright 2004 Quentin Quadrat <lecrapouille@gmail.com>,
//                Minh-Long Nguyen <>,
//                Benoit Marcot <>
//
// This file is part of SimTaDyn.
//
// SimTaDyn is free software: you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with GNU Emacs.  If not, see <http://www.gnu.org/licenses/>.
//=====================================================================

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
