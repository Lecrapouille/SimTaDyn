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
#include "font.h"
#include "memoire.h"

void creer_poignet_3D();
void creer_poignet_2D();
void dessiner_poignet(float x, float y, float z, GLuint l);
void dessine_tous_sommets();
void dessine_tous_arcs();
void dessine_toutes_zones();
void dessine_tous_arcs2();
void dessine_toutes_zones2();
void dessine_toutes_normal();
void dessiner_la_carte();
t_pos point_commun(int ,int );
void dessine_tous_les_triangles();
#endif
