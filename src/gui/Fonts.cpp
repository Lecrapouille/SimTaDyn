#include "Fonts.hpp"
#include <stdarg.h>

void Font::buildFont()
{
  std::cout << "Loading Font" << std::endl;
  base_ = glCheck(glGenLists(95));
  if (0 == base_)
    {
       std::cerr << "Font::buildFont(): Failed when creating a display list" << std::endl;
    }

  glCheck(glBindTexture(GL_TEXTURE_2D, id_));
  for (uint32_t loop = 0; loop < 95; loop++)
     {
       float32_t cx = (float32_t) (loop % 16) / 16.0f;
       float32_t cy = (float32_t) (loop / 16) / 8.0f;

       glCheck(glNewList(base_ + loop, GL_COMPILE));
       glCheck(glBegin(GL_QUADS));
       glCheck(glTexCoord2f(cx,           1.0f - cy - 0.120f)); glCheck(glVertex2i(0,  0));
       glCheck(glTexCoord2f(cx + 0.0625f, 1.0f - cy - 0.120f)); glCheck(glVertex2i(16, 0));
                                                                glCheck(glTexCoord2f(cx + 0.0625f, 1.0f - cy));          glCheck(glVertex2i(16,16));
                                                                glCheck(glTexCoord2f(cx,           1.0f - cy));          glCheck(glVertex2i(0, 16));
       glCheck(glEnd());
       glCheck(glTranslated(10,0,0));
       glCheck(glEndList());
     }
}

void Font::setText(const char *txt, ...)
{
  va_list ap;
  va_start(ap, txt);
  vsprintf(text_, txt, ap);
  va_end(ap);
  length_ = strlen(text_);
}

void Font::draw() const //Render& target) const
{
  if ((0 == length_) || (!loaded_) || (0 == base_))
    {
      //std::cout << "Font not loaded" << std::endl;
      return ;
    }

  glCheck(glEnable(GL_TEXTURE_2D));
  glCheck(glEnable(GL_BLEND));
  glCheck(glBindTexture(GL_TEXTURE_2D, id_));
  //glCheck(glPushMatrix());
  //glCheck(glLoadIdentity());
  //glCheck(glTranslatef(x, y, z));
  //glCheck(glScaled(scale, scale, scale));
  glCheck(glListBase(base_ - 32));
  glCheck(glCallLists(length_, GL_UNSIGNED_BYTE, text_));
  //glCheck(glPopMatrix());
  glCheck(glDisable(GL_BLEND));
  glCheck(glDisable(GL_TEXTURE_2D));
}
