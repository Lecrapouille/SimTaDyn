#include "boundingbox.hpp"

int main(void)
{
  AABB a1;
  AABB a2(Vector3D(1,1,1), Vector3D(2,2,2));
  AABB a3 = AABB::AABB_UNIT_SCALE;
  AABB a4 = AABB::AABB_ZERO;
  std::string msg = "";

  std::cout << "AABB size " << sizeof (AABB) << std::endl;
  std::cout << a2 << a3 << a4 << std::endl;
  std::cout << a4.isWellFormed(msg) << std::endl;

  try
    {
      AABB a5(Vector3D(2,2,2), Vector3D(1,1,1));
    }
  catch (const std::exception &e)
    {
      std::cout << "OK execption attrapee" << std::endl;
    };

  std::cout << a2 * 4 + 3 << std::endl;
  std::cout << a2.centerPoint() << " " << a2.dimension() << " " << a2.volume() << std::endl;

  AABB a5 = a2 * 4;
  std::cout << a5.volume() << " " << (a5 > a2) << std::endl;
  std::cout << a2.contains(a2.centerPoint()) << std::endl;

  std::cout << a2.contains(a2.centerPoint() * 3) << std::endl;
  std::cout << a2.collides(a4) << std::endl;
  std::cout << a2.collides(AABB(Vector3D(2,2,2), Vector3D(3,3,3))) << std::endl;

  std::cout << a3.intersection(a3) << std::endl;
  std::cout << a3.merge(a3) << std::endl;
  std::cout << a2.intersection(a3) << a2.intersection(a3).isWellFormed(msg) << std::endl;
  std::cout << a2.merge(a3) << std::endl;

  std::cout << a4.collides(a4) << std::endl;
  std::cout << a4.collides(a2) << std::endl;

  return 0;
}

#if 0
#include "vector3D.hpp"

class Cell: public Vector3D
{
public:
  Cell(const float32_t fx, const float32_t fy, const float32_t fz = 0.0f)
    : Vector3D(fx, fy, fz)
  {
    name = "toto";
  }
  std::string name;
};

int main(void)
{
  int32_t coordinate[3] = { 4, 5, 6 };

  Vector3D v1;
  Vector3D v2(1.0f, 2.0f, 3.0f);
  Vector3D v3(1.0f, 2.0f);
  Vector3D v4(coordinate);
  Vector3D v5(1, 2, 3);

  std::cout << "Vector size " << sizeof (Vector3D) << std::endl;
  std::cout << v2 << v3 << v4 << v5 << std::endl;

  v2.swap(v4);
  std::cout << v2 << v4 << std::endl;
  v2.swap(v4);
  std::cout << v2 << v4 << std::endl;

  std::cout << (v2 == v3) << std::endl;
  std::cout << (v2 == v5) << std::endl;

  std::cout << (v2 + v5 + v3) << std::endl;
  std::cout << (v5 * 2) << std::endl;
  std::cout << (-v5 * 2) << std::endl;
  v1 = v5;
  std::cout << (+v1) << std::endl;
  v1 = 42;
  std::cout << (v1 / 2.0f) << std::endl;
  std::cout << v2.midPoint(v5) << std::endl;
  std::cout << v2.distance(v5) << std::endl;
  std::cout << v2.distance(v4) << std::endl;

  std::cout << Vector3D::UNIT_X.length() << std::endl;
  std::cout << Vector3D::UNIT_X.distance(Vector3D::UNIT_Y) << std::endl;
  std::cout << Vector3D::UNIT_X.dotProduct(Vector3D::UNIT_Y) << std::endl;
  std::cout << Vector3D::UNIT_X.crossProduct(Vector3D::UNIT_Y) << std::endl;
  std::cout << v1.normalise() << std::endl;
  std::cout << v1 << std::endl;

  std::cout << "*********************" << std::endl;
  Cell c1(1.0f, 2.0f, 3.0f);
  std::cout << c1 << std::endl;

  return 0;
}
#endif
