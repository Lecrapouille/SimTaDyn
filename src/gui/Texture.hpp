#ifndef TEXTURE_HPP_
#  define TEXTURE_HPP_

#  include "gui.hpp"
#  include "opgl_ext.hpp"

class Texture
{
public:
  Texture(std::string const& filename)
  {
    loaded_ = LoadTGA(filename);
    if (!loaded_)
      {
        std::cerr << "[ERROR]: Failed loaded texture '" << filename << "'" << std::endl;
      }
  }

  ~Texture()
  {
    glDeleteTextures(1, &id_);
  }

  inline bool isLoaded() const
  {
    return loaded_;
  }

  inline GLuint getId() const
  {
    return id_;
  }

protected:
  bool LoadTGA(std::string const& filename);

  GLuint id_;
  bool loaded_;
};

#endif /* TEXTURE_HPP_ */
