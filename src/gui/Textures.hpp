#ifndef TEXTURES_HPP_
#  define TEXTURES_HPP_

#  include "OpenGL.hpp"
#  include "simtypes.hpp"

class Texture
{
public:
  Texture(std::string const& filename)
  {
    loaded_ = loadFromFile(filename);
  }

  Texture()
  {
    loaded_ = false;
  }

  ~Texture()
  {
    glDeleteTextures(1, &id_);
  }

  bool loadFromFile(std::string const& filename)
  {
    bool loaded = false;

    std::string::size_type idx = filename.rfind('.');
    if (std::string::npos != idx)
      {
        std::string extension = filename.substr(idx + 1);

        if ("tga" == extension)
          {
            loaded = LoadTGA(filename);
          }
        else
          {
            std::cerr << "[ERROR]: loading '" << extension << "' files is not yet managed" << std::endl;
            return false;
          }
      }
    else
      {
        std::cerr << "[ERROR]: the file '" << filename << "' need an extension" << std::endl;
        return false;
      }

    if (!loaded)
      {
        std::cerr << "[ERROR]: Failed loading texture from '" << filename << "'" << std::endl;
      }
    return loaded;
  }

  inline bool isLoaded() const
  {
    return loaded_;
  }

  inline GLuint getId() const
  {
    std::cerr << "[WARN]: Trying to get texture id from a non loaded texture" << std::endl;
    return id_;
  }

protected:
  bool LoadTGA(std::string const& filename);

  GLuint id_;
  bool loaded_;
};

#endif /* TEXTURE_HPP_ */
