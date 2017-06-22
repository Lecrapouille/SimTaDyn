#include "Camera2D.hpp"

const float* Camera2D::getTransform()
{
  float rotation_ = 0.0f;
  float zoom_ = 1.0f;
  // Rotation components
  float angle  = rotation_ * 3.141592654f / 180.f;
  float cosine = static_cast<float>(std::cos(angle));
  float sine   = static_cast<float>(std::sin(angle));
  float tx     = -look_at_x_ * cosine - look_at_y_ * sine + look_at_x_;
  float ty     =  look_at_x_ * sine - look_at_y_ * cosine + look_at_y_;

  // Projection components
  float a = 2.f / look_at_width_ * zoom_;
  float b = -2.f / look_at_height_ * zoom_;
  float c = -a * look_at_x_;
  float d = -b * look_at_y_;

  // Rebuild the projection matrix
  matrix[0] = a * cosine; matrix[4] = a * sine;   matrix[8]  = 0.f;  matrix[12] = a * tx + c;
  matrix[1] = -b * sine;  matrix[5] = b * cosine; matrix[9]  = 0.f;  matrix[13] = b * ty + d;
  matrix[2] = 0.0f;       matrix[6] = 0.0f;       matrix[10] = 1.0f; matrix[14] = 0.0f;
  matrix[3] = 0.0f;       matrix[7] = 0.0f;       matrix[11] = 0.0f; matrix[15] = 1.0f;

  return matrix;
}
