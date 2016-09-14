#ifndef FONTS_HPP_
#  define FONTS_HPP_

#  include "Renderer.hpp"
#  include "Color.hpp"
#  include "Textures.hpp"

class Font: public Texture//, public Drawable
{
public:
  Font(std::string const& filename)
    : Texture(filename)
  {
    length_ = 0;
    if (loaded_)
      {
        buildFont();
      }
  }

  Font()
  {
    base_ = 0;
    length_ = 0;
    loaded_ = false;
  }

  bool loadTexture(std::string const& filename)
  {
    loaded_ = loadFromFile(filename);
    if (loaded_)
      {
        buildFont();
      }
    return loaded_;
  }

  void setText(const char *txt, ...);
  void draw() const; //Render& target) const;

protected:
  void buildFont();

  GLuint base_;
  char   text_[256];
  uint32_t length_;
};

#endif /* FONTS_HPP_ */
