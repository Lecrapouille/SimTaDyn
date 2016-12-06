#ifndef COLOR_HPP_
#  define COLOR_HPP_

#  include "simtypes.hpp"

#  define CONSTRAIN(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))

class Color
{
public:

  // Values: 0.0f .. 1.0f
  explicit Color(const float32_t red, const float32_t green, const float32_t blue, const float32_t alpha = 1.0f)
  {
    r = CONSTRAIN(red, 0.0f, 1.0f);
    g = CONSTRAIN(green, 0.0f, 1.0f);
    b = CONSTRAIN(blue, 0.0f, 1.0f);
    a = CONSTRAIN(alpha, 0.0f, 1.0f);
  }

  // Values: 0 .. 255
  /*explicit Color(const uint8_t red, const uint8_t green, const uint8_t blue, const uint8_t alpha = 255U)
  {
    r = ((float32_t) red) / 255.0f;
    g = ((float32_t) green) / 255.0f;
    b = ((float32_t) blue) / 255.0f;
    a = ((float32_t) alpha) / 255.0f;

    constrain();
    }*/

   // Values: 0 .. 255
  explicit Color(const uint32_t red, const uint32_t green, const uint32_t blue, const uint32_t alpha = 255U)
  {
    r = ((float32_t) red) / 255.0f;
    g = ((float32_t) green) / 255.0f;
    b = ((float32_t) blue) / 255.0f;
    a = ((float32_t) alpha) / 255.0f;

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

  inline Color& operator=(const float32_t scalar)
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

  inline const Color operator+(const float32_t scalar) const
  {
    return Color(r + scalar, g + scalar, b + scalar, a + scalar);
  }

  inline const Color operator+(const uint8_t scalar) const
  {
    const float32_t ns = ((float32_t) scalar) / 255.0f;
    return Color(r + ns, g + ns, b + ns, a + ns);
  }

  inline const Color operator-(const Color& other) const
  {
    return Color(r - other.r, g - other.g, b - other.b, a - other.a);
  }

  inline const Color operator-(const float32_t scalar) const
  {
    return Color(r - scalar, g - scalar, b - scalar, a - scalar);
  }

  inline const Color operator-(const uint8_t scalar) const
  {
    const float32_t ns = ((float32_t) scalar) / 255.0f;
    return Color(r - ns, g - ns, b - ns, a - ns);
  }

  inline const Color operator*(const float32_t scalar) const
  {
    return Color(r * scalar, g * scalar, b * scalar, a * scalar);
  }

  inline const Color operator*(const uint8_t scalar) const
  {
    const float32_t ns = ((float32_t) scalar) / 255.0f;
    return Color(r * ns, g * ns, b * ns, a * ns);
  }

  inline const Color operator/(const float32_t scalar) const
  {
    if (0.0f == scalar)
      return Color::Black;

    const float32_t finv = 1.0f / scalar;
    return Color(r * finv, g * finv, b * finv, a * finv);
  }

  inline const Color operator/(const uint8_t scalar) const
  {
    if (0U == scalar)
      return Color::Black;

    const float32_t finv = 255.0f / scalar;
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

inline std::ostream& operator<<(std::ostream& os, const Color& c)
{
  os << "Color(" << c.r << ", " << c.g << ", " << c.b << ", " << c.a << ")";
  return os;
}

#endif /* COLOR_HPP_ */
