#include "camera2D.hpp"

const float32_t* Camera2D::getTransform()
{
  // Rotation components
  float32_t angle  = rotation_ * 3.141592654f / 180.f;
  float32_t cosine = static_cast<float32_t>(std::cos(angle));
  float32_t sine   = static_cast<float32_t>(std::sin(angle));
  float32_t tx     = -look_at_x_ * cosine - look_at_y_ * sine + look_at_x_;
  float32_t ty     =  look_at_x_ * sine - look_at_y_ * cosine + look_at_y_;

  // Projection components
  float32_t a =  2.f / look_at_width_;
  float32_t b = -2.f / look_at_height_;
  float32_t c = -a * look_at_x_;
  float32_t d = -b * look_at_y_;

  // Rebuild the projection matrix
  matrix[0] = a * cosine; matrix[4] = a * sine;   matrix[8]  = 0.f;  matrix[12] = a * tx + c;
  matrix[1] = -b * sine;  matrix[5] = b * cosine; matrix[9]  = 0.f;  matrix[13] = b * ty + d;
  matrix[2] = 0.0f;       matrix[6] = 0.0f;       matrix[10] = 1.0f; matrix[14] = 0.0f;
  matrix[3] = 0.0f;       matrix[7] = 0.0f;       matrix[11] = 0.0f; matrix[15] = 1.0f;

  return matrix;
}
