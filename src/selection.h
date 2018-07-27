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

#ifndef DESSIN_H
#define DESSIN_H

#include "header.h"
#include "opgl.h"
#include "memoire.h"
#include "callbacks.h"
#include "pile.h"

void selec_et_empile_sommet();
void selec_et_empile_arete();
void selec_et_empile_zone();
int selection_som();
void deselection_som();
int selection_arete();
void deselection_arete();
int selection_zone();
void deselection_zone();
void selection_forth(char *);

#endif
