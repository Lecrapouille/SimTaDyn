#ifndef TEXTURES_HPP_
#  define TEXTURES_HPP_

#  include "OpenGL.hpp"
#  include <cstring>
#  include <iostream>

class Texture
{
public:
  // *************************************************************************************************
  // Constructor: load a texture from a file (tga ...)
  // *************************************************************************************************
  Texture(std::string const& filename)
    : texture_loaded_(false)
  {
    open(filename);
  }

  // *************************************************************************************************
  // Constructor
  // *************************************************************************************************
  Texture()
    : texture_loaded_(false)
  {
  }

  // *************************************************************************************************
  // Destructor
  // *************************************************************************************************
  ~Texture()
  {
    dropTexture();
  }

  // *************************************************************************************************
  // Constructor: load a texture from a file (tga ...)
  // TODO: tester 2 chargement textures a la suite avec/sans echec
  // *************************************************************************************************
  virtual bool open(std::string const& filename)
  {
    std::string::size_type idx = filename.rfind('.');
    if (std::string::npos != idx)
      {
        std::string extension = filename.substr(idx + 1);

        if ("tga" == extension)
          {
            dropTexture();
            texture_loaded_ = LoadTGA(filename);
          }
        else
          {
            std::cerr << "[ERROR]: loading '" << extension << "' files is not yet managed" << std::endl;
          }
      }
    else
      {
        std::cerr << "[ERROR]: the file '" << filename << "' need an extension" << std::endl;
      }

    if (!texture_loaded_)
      {
        std::cerr << "[ERROR]: Failed loading texture from '" << filename << "'" << std::endl;
      }
    return texture_loaded_;
  }

  // *************************************************************************************************
  // Return if a font has been succesfully loaded
  // *************************************************************************************************
  virtual inline bool isLoaded() const
  {
    return texture_loaded_;
  }

  // *************************************************************************************************
  // FIXME: useful ? Never used !
  // *************************************************************************************************
  virtual inline GLuint getId() const
  {
    if (!texture_loaded_)
      {
        std::cerr << "[WARN]: Trying to get texture id from a non loaded texture. " << std::endl;
      }
    return texture_id_;
  }

protected:

  // *************************************************************************************************
  // Delete OpenGL display lists
  // *************************************************************************************************
  inline void dropTexture()
  {
    if (texture_loaded_)
      {
        glDeleteTextures(1, &texture_id_);
        texture_loaded_ = false;
      }
  }

  // *************************************************************************************************
  //
  // *************************************************************************************************
  bool LoadTGA(std::string const& filename);

  GLuint texture_id_;
  bool texture_loaded_;
};

#endif /* TEXTURE_HPP_ */
