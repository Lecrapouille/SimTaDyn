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
// along with SimTaDyn.  If not, see <http://www.gnu.org/licenses/>.
//=====================================================================

#ifndef CAMERA_HPP_
#  define CAMERA_HPP_

#  include <cstdint>
#  include <iostream>
#  include <cmath>

#  define MIN_ZOOM (0.05f)

class Camera2D
{
public:
  // *************************************************************************************************
  // Construct a camera with parameters the zone of the world we want to observe: x, y is the center
  // of view and width, height the view dimension. By default the view is displayed on full range on
  // the screen region (window).
  // *************************************************************************************************
 Camera2D(const float x, const float y, const float width, const float height)
  {
    // World observation
    lookAt(x, y, width, height);
    m_zoom = 1.0f;

    // Screen ratio
    displayAt(0.0f, 0.0f, 1.0f, 1.0f);
  }

  Camera2D(Camera2D const& other)
  {
    m_look_at_x = other.m_look_at_x;
    m_look_at_y = other.m_look_at_y;
    m_look_at_width = other.m_look_at_width;
    m_look_at_height = other.m_look_at_height;
  }

  Camera2D()
  {
    // World observation
    lookAt(400, 300, 800, 600);
    m_zoom = 1.0f;

    // Screen ratio
    displayAt(0.0f, 0.0f, 1.0f, 1.0f);
  }

  // *************************************************************************************************
  // Change the position on the window screen (values are ratio of the screen size)
  // *************************************************************************************************
  void displayAt(const float x, const float y, const float width, const float height)
  {
    if ((x > 1.0f) || (y > 1.0f) || (width > 1.0f) || (height > 1.0f))
      {
        std::cerr << "Error: parameters for displayAt(x, y, width, height) are ratio of the screen size." << std::endl;
      }
    else
      {
        m_display_at_x = x;
        m_display_at_y = y;
        m_display_at_width = width;
        m_display_at_height = height;
      }
  }

  // *************************************************************************************************
  // Change the world observation: x, y is the center of view and width, height the view dimension.
  // *************************************************************************************************
  void lookAt(const float x, const float y, const float width, const float height)
  {
    m_look_at_x = x;
    m_look_at_y = y;
    m_look_at_width = width;
    m_look_at_height = height;
  }

  // *************************************************************************************************
  // Change the world observation: x, y is the center of view.
  // *************************************************************************************************
  void lookAt(const float x, const float y)
  {
    m_look_at_x = x;
    m_look_at_y = y;
  }

  // *************************************************************************************************
  // Deplace the word observation from an offset
  // *************************************************************************************************
  void moveOffset(const float offsetX, const float offsetY)
  {
    m_look_at_x += offsetX;
    m_look_at_y += offsetY;
  }

  // *************************************************************************************************
  // Change dimension of the observed world
  // *************************************************************************************************
  void setDimension(const float width, const float height)
  {
    m_look_at_width = width;
    m_look_at_height = height;
  }

  // *************************************************************************************************
  // Zoom on the observed world
  // *************************************************************************************************
  void zoom(const float factor)
  {
    if (factor < MIN_ZOOM)
      {
        m_zoom = MIN_ZOOM;
      }
    else
      {
        m_zoom = factor;
      }
    std::cout << "New zoom " << m_zoom << "\n";
  }

  // *************************************************************************************************
  // Zoom on the observed world
  // *************************************************************************************************
  void zoomOffset(const float factor)
  {
    m_zoom = m_zoom * (1.0f + factor);
  }

  // *************************************************************************************************
  // Look at given location and zoom
  // *************************************************************************************************
  void zoomAt(const float x, const float y, const float factor)
  {
    lookAt(x, y);
    zoomOffset(factor);
  }

  // *************************************************************************************************
  //
  // *************************************************************************************************
  void zoomFit(const float window_width, const float window_height,
               const float image_width, const float image_height)
  {
    lookAt(window_width / 2.0f, window_height / 2.0f);
    std::cout << window_width << "/" << image_width << ", "
              << window_height << "/" << image_height << "==> min("
              << window_width / image_width << ", " << window_height / image_height
              << ")\n";

    zoom(std::min(window_width / image_width, window_height / image_height));
  }

  // *************************************************************************************************
  //
  // *************************************************************************************************
  float getZoom() const
  {
    return m_zoom;
  }

  // *************************************************************************************************
  //
  // *************************************************************************************************
  const float* getTransform();

  //private:
  float m_zoom;
  float m_look_at_x;          // in pixel
  float m_look_at_y;          // in pixel
  float m_look_at_width;      // in pixel
  float m_look_at_height;     // in pixel

  float m_display_at_x;       // screen ratio
  float m_display_at_y;       // screen ratio
  float m_display_at_width;   // screen ratio
  float m_display_at_height;  // screen ratio

  float m_matrix[16];
};

inline std::ostream& operator<<(std::ostream& os, const Camera2D& c)
{
  os << "Camera2D((x: " << c.m_look_at_x << ", y: " << c.m_look_at_y
     << "), (W: " << c.m_look_at_width << ", H: " << c.m_look_at_height
     << "))";
  return os;
}

#endif /* CAMERA_HPP_ */
