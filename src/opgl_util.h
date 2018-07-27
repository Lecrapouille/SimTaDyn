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

#ifndef OPGL_UTIL_H
#define OPGL_UTIL_H

#include "header.h"

typedef struct
{
     GLubyte    *imageData;
     GLuint     bpp;
     GLuint     width;
     GLuint     height;
     GLuint     texID;
} TextureImage;

TextureImage textures_font[1];

bool LoadTGA(TextureImage *texture, char *filename);
GLvoid BuildFont();
GLvoid glPrint2D(GLint x, GLint y, const char *string, ...);
GLvoid glPrint3D(GLint x, GLint y, GLint z, GLfloat f, GLfloat r, GLfloat g, GLfloat b, const char *string, ...);

#endif /* OPGL_UTIL_H */
