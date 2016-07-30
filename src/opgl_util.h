#ifndef OPGL_UTIL_H
#define OPGL_UTIL_H

#include "header.h"

typedef struct
{
     GLubyte	*imageData;
     GLuint	bpp;
     GLuint	width;
     GLuint	height;
     GLuint	texID;
} TextureImage;

TextureImage textures_font[1];

bool LoadTGA(TextureImage *texture, char *filename);
GLvoid BuildFont();
GLvoid glPrint2D(GLint x, GLint y, const char *string, ...);
GLvoid glPrint3D(GLint x, GLint y, GLint z, GLfloat f, GLfloat r, GLfloat g, GLfloat b, const char *string, ...);

#endif /* OPGL_UTIL_H */
