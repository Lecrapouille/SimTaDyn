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

#ifndef HASH_H
#define HASH_H

#include <stdio.h>
#include <string.h>
#include "memoire.h"
#include "mot.h"
#include "header.h"
#include "store.h"

void print_etat(t_etat etat);
t_hash nouveau_hash();
unsigned int fct_hash(char * key);
bool etat_est(t_hash *hash, int i, t_etat etat);
bool ajouter_donnee_hash(t_hash *hash, void* donnee, char* cle);
int chercher_donnee_hash(t_hash *hash, char *elt);
t_cfa chercher_mot_hash(t_hash *hash, char * cle);
void supprimer_mot_hash(t_hash *hash, char * cle);
void destroy_hash(t_hash * hash);
bool ajouter_mot_hash_et_gtk(t_hash *hash, void* donnee, char* cle);
bool ajouter_mot_hash(t_hash *hash, void* donnee, char* cle);
void forcer_ajout_hash(t_hash *hash, void* donnee, char* cle);
void afficher_hash(t_hash *hash);
void reinit_hash(t_hash *hash);
void ieme_reinit_hash(t_hash *hash, int i);

#endif /* HASH_H */
