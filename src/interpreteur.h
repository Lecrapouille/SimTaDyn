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

#ifndef INTERPRETEUR_H
#define INTERPRETEUR_H

#include "erreur.h"
#include "string_util.h"
#include "pile.h"
#include "mot.h"
#include "header.h"
#include "pile.h"
#include "elt_pile.h"

void (*execute_mot)(int); //pointeur sur fct
void execute_mot_avec_debuggeur(int nb);
void execute_mot_sans_debuggeur(int nb);
t_erreur parser_tampon(gchar ** tampon);

#endif /* INTERPRETEUR_H */
