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

#ifndef STORE_H
#define STORE_H

#include "support.h"
#include "header.h"
#include "pile.h"

void ajouter_colonne_visualdico(GtkTreeView *vue);
GtkTreeModel *creer_visualdico(void);
void ajouter_mot_dans_visualdico(GtkTreeModel * store, t_cfa mot);

void ajouter_nomdeschamps_magazin_champ(GtkTreeStore * store, const gchar* nom);
void ajouter_col_visual_champ(GtkTreeView *vue);

void ajouter_col_visual_pile_donnees(GtkTreeView *vue);
void ajouter_col_visual_pile_retour(GtkTreeView *vue);
void ajouter_id_cell_visual_pile(GtkListStore * store, int i);

void create_popup_menu(GtkWidget *widget, GtkWidget ** menu);
void nouveau_item_popup(gchar * chaine, GtkWidget ** menu);

#endif /* STORE_H */
