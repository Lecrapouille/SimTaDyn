#ifndef FONTS_HPP_
#  define FONTS_HPP_

#  include "gui.hpp"
#  include "Texture.hpp"

class Font: public Texture
{
public:
  Font(std::string const& filename)
    : Texture(filename)
  {
    if (loaded_)
      {
        buildFont();
      }
  }

  //void print2D(const GLint x, const GLint y, const GLfloat scale, const char *txt, ...);
  void print3D(const GLfloat x, const GLfloat y, const GLfloat z, const GLfloat scale, const char *txt, ...);

protected:
  void buildFont();

  GLuint base_;
};

#endif /* FONTS_HPP_ */
