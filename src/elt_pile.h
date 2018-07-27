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

#ifndef CELLULE_H
#define CELLULE_H

#include "header.h"
#include "memoire.h"

void afficher_type(t_type type);
void afficher_elt(t_cf elt);
void afficher_elt_buffer(t_cf elt, GtkTextBuffer *bf);
t_cf int_to_elt(int val);
t_cf float_to_elt(float val);
t_cf ad_to_elt(void * val, t_type type);
gchar* elt_to_string(t_cf elt);
#if 0
t_cf zone_to_elt(int val);
t_cf sommet_to_elt(int val);
t_cf arc_to_elt(int val);
bool est_cellule(t_cf elt);
#endif

#endif /* CELLULE_H */
