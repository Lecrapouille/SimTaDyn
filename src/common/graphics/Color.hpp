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

#ifndef COLOR_HPP_
#  define COLOR_HPP_

#  include <cstdint>
#  include <utility>
#  include <ostream>

#  define CONSTRAIN(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))

class Color
{
public:

  // Values: 0.0f .. 1.0f
  explicit Color(const float red, const float green, const float blue, const float alpha = 1.0f)
  {
    r = CONSTRAIN(red, 0.0f, 1.0f);
    g = CONSTRAIN(green, 0.0f, 1.0f);
    b = CONSTRAIN(blue, 0.0f, 1.0f);
    a = CONSTRAIN(alpha, 0.0f, 1.0f);
  }

  // Values: 0 .. 255
  /*explicit Color(const uint8_t red, const uint8_t green, const uint8_t blue, const uint8_t alpha = 255U)
  {
    r = ((float) red) / 255.0f;
    g = ((float) green) / 255.0f;
    b = ((float) blue) / 255.0f;
    a = ((float) alpha) / 255.0f;

    constrain();
    }*/

   // Values: 0 .. 255
  explicit Color(const uint32_t red, const uint32_t green, const uint32_t blue, const uint32_t alpha = 255U)
  {
    r = ((float) red) / 255.0f;
    g = ((float) green) / 255.0f;
    b = ((float) blue) / 255.0f;
    a = ((float) alpha) / 255.0f;

    constrain();
  }

  Color()
  {
    r = 0.0f;
    g = 0.0f;
    b = 0.0f;
    a = 1.0f;
  }

  inline void swap(Color& other)
  {
    std::swap(r, other.r);
    std::swap(g, other.g);
    std::swap(b, other.b);
    std::swap(a, other.a);
  }

  inline bool operator==(const Color& c) const
  {
    return (c.r == r) && (c.g == g) && (c.b == b) && (c.a == a);
  }

  inline bool operator!=(const Color& other) const
  {
    return !(*this == other);
  }

  inline Color& operator=(const Color& other)
  {
    if (this != &other)
      {
        r = other.r;
        g = other.g;
        b = other.b;
        a = other.a;
      }
    return *this;
  }

  inline Color& operator=(const float scalar)
  {
    r = scalar;
    g = scalar;
    b = scalar;
    // do not change alpha ?
    return *this;
  }

  inline const Color operator+(const Color& other) const
  {
    return Color(r + other.r, g + other.g, b + other.b, a + other.a);
  }

  inline const Color operator+(const float scalar) const
  {
    return Color(r + scalar, g + scalar, b + scalar, a + scalar);
  }

  inline const Color operator+(const uint8_t scalar) const
  {
    const float ns = ((float) scalar) / 255.0f;
    return Color(r + ns, g + ns, b + ns, a + ns);
  }

  inline const Color operator-(const Color& other) const
  {
    return Color(r - other.r, g - other.g, b - other.b, a - other.a);
  }

  inline const Color operator-(const float scalar) const
  {
    return Color(r - scalar, g - scalar, b - scalar, a - scalar);
  }

  inline const Color operator-(const uint8_t scalar) const
  {
    const float ns = ((float) scalar) / 255.0f;
    return Color(r - ns, g - ns, b - ns, a - ns);
  }

  inline const Color operator*(const float scalar) const
  {
    return Color(r * scalar, g * scalar, b * scalar, a * scalar);
  }

  inline const Color operator*(const uint8_t scalar) const
  {
    const float ns = ((float) scalar) / 255.0f;
    return Color(r * ns, g * ns, b * ns, a * ns);
  }

  inline const Color operator/(const float scalar) const
  {
    if (0.0f == scalar)
      return Color::Black;

    const float finv = 1.0f / scalar;
    return Color(r * finv, g * finv, b * finv, a * finv);
  }

  inline const Color operator/(const uint8_t scalar) const
  {
    if (0U == scalar)
      return Color::Black;

    const float finv = 255.0f / scalar;
    return Color(r * finv, g * finv, b * finv, a * finv);
  }

  inline void constrain()
  {
    r = CONSTRAIN(r, 0.0f, 1.0f);
    g = CONSTRAIN(g, 0.0f, 1.0f);
    b = CONSTRAIN(b, 0.0f, 1.0f);
    a = CONSTRAIN(a, 0.0f, 1.0f);
  }

  static const Color& constrain(Color& color)
  {
    color.r = CONSTRAIN(color.r, 0.0f, 1.0f);
    color.g = CONSTRAIN(color.g, 0.0f, 1.0f);
    color.b = CONSTRAIN(color.b, 0.0f, 1.0f);
    color.a = CONSTRAIN(color.a, 0.0f, 1.0f);
    return color;
  }

  uint32_t toARGB() const
  {
    uint32_t ur = static_cast<uint32_t>(r * 255.0f);
    uint32_t ug = static_cast<uint32_t>(g * 255.0f);
    uint32_t ub = static_cast<uint32_t>(b * 255.0f);
    uint32_t ua = static_cast<uint32_t>(a * 255.0f);

    return (ua << 24) | (ur << 16) | (ug << 8) | (ub << 0);
  }

  uint32_t toABGR() const
  {
    uint32_t ur = static_cast<uint32_t>(r * 255.0f);
    uint32_t ug = static_cast<uint32_t>(g * 255.0f);
    uint32_t ub = static_cast<uint32_t>(b * 255.0f);
    uint32_t ua = static_cast<uint32_t>(a * 255.0f);

    return (ua << 24) | (ub << 16) | (ug << 8) | (ur << 0);
  }

  uint32_t toRGBA() const
  {
    uint32_t ur = static_cast<uint32_t>(r * 255.0f);
    uint32_t ug = static_cast<uint32_t>(g * 255.0f);
    uint32_t ub = static_cast<uint32_t>(b * 255.0f);
    uint32_t ua = static_cast<uint32_t>(a * 255.0f);

    return (ur << 24) | (ug << 16) | (ub << 8) | (ua << 0);
  }

  static const Color Black;
  static const Color White;
  static const Color Red;
  static const Color Green;
  static const Color Blue;
  static const Color Yellow;
  static const Color Magenta;
  static const Color Cyan;
  static const Color Transparent;

  float r;
  float g;
  float b;
  float a;
};

inline std::ostream& operator<<(std::ostream& os, const Color& c)
{
  os << "Color(" << c.r << ", " << c.g << ", " << c.b << ", " << c.a << ")";
  return os;
}

#endif /* COLOR_HPP_ */
