#ifndef FONT_H
#define FONT_H

#include "header.h"
#include "opgl.h"

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
GLvoid glPrint3D(GLfloat x, GLfloat y, GLfloat z, GLfloat f, const char *string, ...);

#endif /* FONT_H */
