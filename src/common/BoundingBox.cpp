#include "BoundingBox.hpp"

const AABB AABB::AABB_ZERO(Vector3D::ZERO, Vector3D::ZERO);
const AABB AABB::AABB_UNIT_SCALE(Vector3D::ZERO, Vector3D::UNIT_SCALE);
const AABB AABB::AABB_INFINITE(std::numeric_limits<float32_t>::lowest(),
                               std::numeric_limits<float32_t>::lowest(),
                               std::numeric_limits<float32_t>::lowest(),
                               std::numeric_limits<float32_t>::max(),
                               std::numeric_limits<float32_t>::max(),
                               std::numeric_limits<float32_t>::max());
