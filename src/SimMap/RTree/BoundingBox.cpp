#include "BoundingBox.hpp"

const AABB AABB::DUMMY(Vector3D::DUMMY, Vector3D::DUMMY);
const AABB AABB::ZERO(Vector3D::ZERO, Vector3D::ZERO);
const AABB AABB::UNIT_SCALE(Vector3D::NEGATIVE_UNIT_SCALE / 2.0f, Vector3D::UNIT_SCALE / 2.0f);
const AABB AABB::INFINITE(std::numeric_limits<float32_t>::lowest(),
                               std::numeric_limits<float32_t>::lowest(),
                               std::numeric_limits<float32_t>::lowest(),
                               std::numeric_limits<float32_t>::max(),
                               std::numeric_limits<float32_t>::max(),
                               std::numeric_limits<float32_t>::max());
