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

#ifndef PILE_H
#define PILE_H

#include "memoire.h"
#include "header.h"
#include "string_util.h"
#include "elt_pile.h"

t_pile nouvelle_pile(void);
bool pile_vide(t_pile * pile);
void reinit_pile(t_pile * pile);
void reallouer_pile(t_pile * pile, unsigned int taille);
t_cf sommet(t_pile * pile);
void depiler(t_pile * pile);
void empiler(t_pile * pile, t_cf elt);
int nb_elt_pile(t_pile * pile);
void afficher_pile(t_pile pile);
void afficher_pile_buffer(t_pile pile, GtkTextBuffer * bf);
void reinit_pile_gtk(GtkListStore * store);
void depiler_gtk(GtkListStore * store);
void empiler_gtk(GtkListStore * store, t_cf elt, GtkTreeIter * iter);
void recopier_pile_gtk(t_pile * pile, GtkListStore * store, GtkTreeIter * iter);

#endif /* PILE_H */
