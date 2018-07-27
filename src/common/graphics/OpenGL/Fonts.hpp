//=====================================================================
// SimTaDyn: A GIS in a spreadsheet.
// Copyright 2018 Quentin Quadrat <lecrapouille@gmail.com>
//
// This file is part of SimTaDyn.
//
// SimTaDyn is free software: you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with GNU Emacs.  If not, see <http://www.gnu.org/licenses/>.
//=====================================================================

#ifndef FONTS_HPP_
#  define FONTS_HPP_

#  include "Color.hpp"
#  include "Textures.hpp"
#  include "Vector3D.hpp"

class SimTaDynFont: public Texture
{
public:

  // *************************************************************************************************
  // How to load a font (from a texture, system)
  // *************************************************************************************************
  enum LoadMode { MappedTexture, SystemFont };

  // *************************************************************************************************
  // Constructor
  // *************************************************************************************************
  SimTaDynFont(const LoadMode mode, std::string const& name)
    : nb_fonts_(0), base_(0)
  {
    open(mode, name);
  }

  // *************************************************************************************************
  // Constructor
  // *************************************************************************************************
  SimTaDynFont()
    : nb_fonts_(0), base_(0)
  {
  }

  // *************************************************************************************************
  // Destructor
  // *************************************************************************************************
  ~SimTaDynFont()
  {
    dropFont();
  }

  // *************************************************************************************************
  // Load a font (from a texture, system)
  // @param name if load from system then give the font name else the texture name.
  // type `xlsfonts` command in a bash to find a suitable font.
  // *************************************************************************************************
  virtual bool open(const LoadMode mode, std::string const& name)
  {
    bool res;

    dropFont();
    switch (mode)
      {
      case MappedTexture:
        res = loadMappedTexture(name);
        break;
      case SystemFont:
        std::cout << "openXfont ";
        res = loadXFont(name);
        break;
      default:
        res = false;
        break;
      }
    std::cout << res << "\n\n";
    return res;
  }

  // *************************************************************************************************
  // Return if a font has been succesfully loaded
  // *************************************************************************************************
  virtual inline bool isLoaded() const
  {
    return 0 != base_;
  }

  // *************************************************************************************************
  //
  // *************************************************************************************************
  void draw(std::string const& text);// const;

  // *************************************************************************************************
  //
  // *************************************************************************************************
  inline Vector3D centerPoint() const;

  //protected:

  // *************************************************************************************************
  // Delete OpenGL display lists
  // *************************************************************************************************
  inline void dropFont()
  {
    dropTexture();
    if (base_)
      {
        glDeleteLists(base_, nb_fonts_);
        base_ = 0;
      }
    nb_fonts_ = 0;
  }

  // *************************************************************************************************
  // Open a Unix/X system font. Routine only available on Unix/X systems.
  // @param fontname: the name of the font
  // @return if font has been succesfully loaded.
  // *************************************************************************************************
  bool loadXFont(std::string const& fontname);

  // *************************************************************************************************
  // Open a Texture Mapped Text
  // @param filenmae: the texture file name (path).
  // @return if font has been succesfully loaded.
  // *************************************************************************************************
  bool loadMappedTexture(std::string const& filename)
  {
    dropFont();
    Texture::open(filename);
    if (Texture::texture_loaded_)
      {
        buildMappedTexture();
      }
    return (Texture::isLoaded()) && (SimTaDynFont::isLoaded());
  }

  void buildMappedTexture();

  uint32_t length_;
  GLsizei nb_fonts_;
  GLuint base_;
};

#endif /* FONTS_HPP_ */
