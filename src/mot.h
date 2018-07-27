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

#ifndef MOT_H
#define MOT_H

#include <stdio.h>
#include <string.h>
#include "memoire.h"
#include "erreur.h"
#include "header.h"
#include "hash.h"
#include "elt_pile.h"

/*
** PRECEDENCE BIT = mot immediat (8eme bit)
** LONGUEUR MOT (du 1er bit --> 7eme bit)
*/

#define PRIMITIVE_BIT 0x40
#define PRECEDENCE_BIT 0x80
#define EMPTY_WORD 0x00

/*
** change le caractere immediat de 'entete'
*/
void change_masque(octet * info, octet masque);
bool est_immediat(octet info);
bool est_primitif(octet info);
octet longueur(octet info);
void change_longueur(octet nb, octet * info);

void detruire_mot(t_cfa mot);
t_cfa adresse_fin_mot(t_cfa mot);
bool nouveau_mot_courant(void);
t_nfa nouveau_nfa(gchar * nom, octet masque, FONCTION_C);
t_cfa nouveau_mot(gchar * nom, octet masque, FONCTION_C);
void ajouter_adresse(t_cfa mot, void * ad, t_type type); //A SUPP
void ajouter_elt_dans_mot(t_cfa mot, t_cf elt);
void afficher_mot(t_cfa mot);

#endif /* MOT_H */
