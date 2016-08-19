#ifndef COLOR_HPP_
#  define COLOR_HPP_

#  include "simtypes.hpp"

class Color
{
public:
  Color(float32_t red, float32_t green, float32_t blue, float32_t alpha = 1.0f)
  {
    r = red; g = green; b = blue; a = alpha;
  }
  Color()
  {
    r = 0; g = 0; b = 0; a = 1.0f;
  }

  uint32_t toInteger() const
  {
    return ((uint32_t) (a * 255.0f)) |
      (((uint32_t) (r * 255.0f)) << 24) |
      (((uint32_t) (g * 255.0f)) << 16) |
      (((uint32_t) (b * 255.0f)) << 8);
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

  float32_t r;
  float32_t g;
  float32_t b;
  float32_t a;
};

#endif /* COLOR_HPP_ */
