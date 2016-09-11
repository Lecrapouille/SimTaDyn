#include "Fonts.hpp"

void Font::buildFont()
{
  base_ = glGenLists(95);

  glBindTexture(GL_TEXTURE_2D, id_);
  for (uint32_t loop = 0; loop < 95; loop++)
     {
       float32_t cx = (float32_t) (loop % 16) / 16.0f;
       float32_t cy = (float32_t) (loop / 16) / 8.0f;

       glNewList(base_ + loop,GL_COMPILE);
       glBegin(GL_QUADS);
       glTexCoord2f(cx,           1.0f - cy - 0.120f); glVertex2i(0,  0);
       glTexCoord2f(cx + 0.0625f, 1.0f - cy - 0.120f); glVertex2i(16, 0);
       glTexCoord2f(cx + 0.0625f, 1.0f - cy);          glVertex2i(16,16);
       glTexCoord2f(cx,           1.0f - cy);          glVertex2i(0, 16);
       glEnd();
       glTranslated(10,0,0);
       glEndList();
     }
}
/*
void Font::print2D(const GLint x, const GLint y, const GLfloat scale, const char *txt, ...)
{
  char    text[256];
  va_list ap;

  if ((NULL == txt) || (!loaded_))
    return ;

  va_start(ap, txt);
  vsprintf(text, txt, ap);
  va_end(ap);

  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  glOrtho(0, guts.width, 0, guts.height, -1, 1);
  glMatrixMode(GL_MODELVIEW);

  glBindTexture(GL_TEXTURE_2D, id_);
  glPushMatrix();
  glLoadIdentity();
  glTranslated(x, y, 0);
  glScaled(scale, scale, scale);
  glListBase(base_ - 32);
  glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);
  glPopMatrix();

  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);
  }*/

void Font::print3D(const GLfloat x, const GLfloat y, const GLfloat z, const GLfloat scale, const char *txt, ...)
{
  char    text[256];
  va_list ap;

  if ((NULL == txt) || (!loaded_))
    return ;

  va_start(ap, txt);
  vsprintf(text, txt, ap);
  va_end(ap);

  glEnable(GL_TEXTURE_2D);
  glEnable(GL_BLEND);
  glBindTexture(GL_TEXTURE_2D, id_);
  glPushMatrix();
  glLoadIdentity();
  glTranslatef(x, y, z);
  glScaled(scale, scale, scale);
  glListBase(base_ - 32);
  glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);
  glPopMatrix();
  glDisable(GL_BLEND);
  glDisable(GL_TEXTURE_2D);
}
