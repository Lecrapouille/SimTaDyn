#include "TransformationTests.hpp"
#include <glm/vec3.hpp> // glm::vec3, glm::ivec3
#include <glm/vec4.hpp> // glm::vec4, glm::ivec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective

// Register the test suite
CPPUNIT_TEST_SUITE_REGISTRATION(TransformationTests);

static Matrix44f I(matrix::Identity);
static Matrix44f T;
static glm::mat4 gT;

//--------------------------------------------------------------------------
static void display(glm::mat4 const &a)
{
  std::cout << "GLM:" << std::endl;
  for (uint32_t i = 0U; i < 4U; ++i)
    {
      for (uint32_t j = 0U; j < 4U; ++j)
        {
          // column-major: use transpose
          std::cout << a[j][i] << " ";
        }
      std::cout << std::endl;
    }
}

//--------------------------------------------------------------------------
static void display(Matrix44f const &a)
{
  std::cout << "My Matrix:" << std::endl << matrix::transpose(a) << std::endl;
}

//--------------------------------------------------------------------------
static float fabs(const float v)
{
  if (v < 0.0f) return -v;
  return v;
}

//--------------------------------------------------------------------------
static void compareMatrices(Matrix44f const &a, glm::mat4 const &b)
{
  std::cout << std::endl << "Comparing Matrices:" << std::endl;
  display(a); std::cout << "with "; display(b);

  for (uint32_t i = 0U; i < 4U; ++i)
    {
      for (uint32_t j = 0U; j < 4U; ++j)
        {
          CPPUNIT_ASSERT_EQUAL(true, fabs(a[i][j] - b[i][j]) < 0.001f);
        }
    }
}

//--------------------------------------------------------------------------
void TransformationTests::setUp()
{
}

//--------------------------------------------------------------------------
void TransformationTests::tearDown()
{
}

//--------------------------------------------------------------------------
void TransformationTests::tests()
{
  // Window size
  float width = 800.0f;
  float height = 600.0f;

  // Check translation
  T = matrix::translate(I, Vector3f(2.0f, 3.0f, 4.0f));
  gT = glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, 3.0f, 4.0f));
  compareMatrices(T, gT);

  // Check scale
  T = matrix::scale(T, Vector3f(5.0f, 6.0f, 7.0f));
  gT = glm::scale(gT, glm::vec3(5.0f, 6.0f, 7.0f));
  compareMatrices(T, gT);

  // Check roation
  T = matrix::rotate(I, 10.0f, Vector3f(1.0f, 1.0f, 1.0f));
  gT = glm::rotate(glm::mat4(1.0f), 10.0f, glm::vec3(1.0f, 1.0f, 1.0f));
  compareMatrices(T, gT);

  // Check ortho
  T = matrix::ortho(0.0f, width, 0.0f, height);
  gT = glm::ortho(0.0f, width, 0.0f, height);
  compareMatrices(T, gT);

  // Check perspective
  T = matrix::perspective(45.0f, width / height, 0.1f, 100.0f);
  gT = glm::perspective(45.0f, width / height, 0.1f, 100.0f);
  compareMatrices(T, gT);

  // Check LookAt
  T = matrix::lookAt(Vector3f(1.0f, 0.0f, 0.0f), Vector3f(0.0f, 1.0f, 0.0f), Vector3f(0.0f, 1.0f, 0.0f));
  gT = glm::lookAt(glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
  compareMatrices(T, gT);
}
