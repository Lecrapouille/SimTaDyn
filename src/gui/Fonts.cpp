#include "Fonts.hpp"
#include <stdarg.h>

// *************************************************************************************************
// A texture mapped text is a texture where caracters are aligned like a matrix.
// So split the texture for each ascii caracter abd store it in a display list
// *************************************************************************************************
void SimTaDynFont::buildMappedTexture()
{
  GLsizei nb_fonts;

  nb_fonts = 95;
  base_ = glCheck(glGenLists(nb_fonts));
  if (0 == base_)
    {
       std::cerr << "Font::buildMappedTexture(): Failed when creating a display list" << std::endl;
    }
  else
    {
      glCheck(glBindTexture(GL_TEXTURE_2D, texture_id_));
      for (GLsizei loop = 0; loop < nb_fonts; loop++)
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
      nb_fonts_ = nb_fonts;
    }
}

// https://openclassrooms.com/forum/sujet/opengl-gerer-un-score-86107
// *************************************************************************************************
// Open a Unix/X system font. Routine only available on Unix/X systems.
// *************************************************************************************************
bool SimTaDynFont::loadXFont(std::string const& fontname)
{
  Display* display;
  XFontStruct* font_info;
  Font id;
  int first;
  int last;

  // Need an X Display before calling any Xlib routines
  display = XOpenDisplay(NULL);
  if (NULL == display)
    {
      std::cerr << "Font::loadXFont(): XOpenDisplay() failed" << std::endl;
      return false;
    }

  // Load the font
  font_info = XLoadQueryFont(display, fontname.c_str());
  if (NULL == font_info)
    {
      std::cerr << "Font::loadXFont(): XLoadQueryFont() failed" << std::endl;
      return false;
    }

  // Tell GLX which font & glyphs to use.
  id = font_info->fid;
  first = 32;//font_info->min_char_or_byte2;
  last  = 126;//font_info->max_char_or_byte2;

  base_ = glCheck(glGenLists((GLuint) last + 1));
  if (0 == base_)
    {
      std::cerr << "Font::loadXFont(): Failed when creating a display list" << std::endl;
      return false;
    }

  // Create bitmap display lists from an X font
  glXUseXFont(id, first, last - first + 1, base_ + first);

  nb_fonts_ = last - first + 1;
  XCloseDisplay(display);
  return true;
}

// *************************************************************************************************
//
// *************************************************************************************************
inline Vector3D SimTaDynFont::centerPoint() const
{
  return Vector3D(static_cast<float32_t>(length_) * 10.0f / 2.0f, 16.0f / 2.0f);
}

// *************************************************************************************************
//
// *************************************************************************************************
void SimTaDynFont::draw(std::string const& text) //const
{
  length_ = text.size();

  if ((0 == length_) || (0 == base_))
    return ;

  if (Texture::isLoaded())
    {
      // Texture Mapped Text
      glCheck(glEnable(GL_TEXTURE_2D));
      glCheck(glEnable(GL_BLEND));
      glCheck(glBindTexture(GL_TEXTURE_2D, Texture::texture_id_));
      glCheck(glPushAttrib(GL_LIST_BIT));
      glCheck(glListBase(base_ - 32));
      glCheck(glCallLists(length_, GL_UNSIGNED_BYTE, (GLubyte *) text.c_str()));
      glCheck(glPopAttrib());
      glCheck(glDisable(GL_BLEND));
      glCheck(glDisable(GL_TEXTURE_2D));
    }
  else
    {
      // System font
      glCheck(glPushAttrib(GL_LIST_BIT));
      glCheck(glListBase(base_));
      glCheck(glCallLists(length_, GL_UNSIGNED_BYTE, (GLubyte *) text.c_str()));
      glCheck(glPopAttrib());
    }
}
