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

#ifndef NORMAL_H
#define NORMAL_H

#include "header.h"
#include "opgl.h"
#include "font.h"
#include "memoire.h"
#include "dessine_carte.h"

t_pos Cross(t_pos vVector1, t_pos vVector2);
t_pos Vector(t_pos vPoint1, t_pos vPoint2);
float Magnitude(t_pos vNormal);
t_pos Normalize(t_pos vNormal);
t_pos Normal(int p1,int p2);
t_pos pos2_to_pos (t_pos2);
#endif
