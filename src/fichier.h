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

#ifndef FICHIER_H
#define FICHIER_H

#include <stdio.h>
#include <string.h>
#include "header.h"
#include "memoire.h"
#include "string_util.h"
#include "interpreteur.h"

#define TAILLE_TAMPON 1024

gchar* fichier_to_tampon(const gchar * nomfichier);
bool tampon_to_fichier(gchar * tampon, const gchar * nomfichier);
bool charger_fichier_forth(const gchar * nomfichier);

#endif /* FICHIER_H */
