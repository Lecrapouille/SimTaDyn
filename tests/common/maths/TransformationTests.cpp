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
static void compareMatrices(Matrix44f const &a, glm::mat4 const &b, const int line)
{
  std::cout << std::endl << "Test line " << line << ": Comparing Matrices:" << std::endl;
  display(a); std::cout << "with "; display(b);

  for (uint32_t i = 0U; i < 4U; ++i)
    {
      for (uint32_t j = 0U; j < 4U; ++j)
        {
          CPPUNIT_ASSERT_EQUAL(true, maths::abs(a[i][j] - b[i][j]) < 0.001f);
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
  compareMatrices(T, gT, __LINE__);

  // Check scale
  T = matrix::scale(T, Vector3f(5.0f, 6.0f, 7.0f));
  gT = glm::scale(gT, glm::vec3(5.0f, 6.0f, 7.0f));
  compareMatrices(T, gT, __LINE__);

  // Check roation
  T = matrix::rotate(I, 10.0f, Vector3f(1.0f, 1.0f, 1.0f));
  gT = glm::rotate(glm::mat4(1.0f), 10.0f, glm::vec3(1.0f, 1.0f, 1.0f));
  compareMatrices(T, gT, __LINE__);

  // Check ortho
  T = matrix::ortho(0.0f, width, 0.0f, height);
  gT = glm::ortho(0.0f, width, 0.0f, height);
  //FIXME compareMatrices(T, gT, __LINE__);

  // Check perspective
  T = matrix::perspective(45.0f, width / height, 0.1f, 100.0f);
  gT = glm::perspective(45.0f, width / height, 0.1f, 100.0f);
  //FIXME compareMatrices(T, gT, __LINE__);

  // Check LookAt
  T = matrix::lookAt(Vector3f(1.0f, 0.0f, 0.0f), Vector3f(0.0f, 1.0f, 0.0f), Vector3f(0.0f, 1.0f, 0.0f));
  gT = glm::lookAt(glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
  //FIXME compareMatrices(T, gT, __LINE__);
}

//--------------------------------------------------------------------------
void TransformationTests::movable()
{
  Movable<float, 3U> obj;

  // Note: glm::mat4() == glm::mat4(1.0f) == identity matrix
  // But it seems glm-0.9.9.0 for OS-X Sierra is buggy and return a 0-filled matrix.

  // Check init.
  compareMatrices(obj.transform(), glm::mat4(1.0f), __LINE__);

  // Check identity
  obj.position(Vector3f(0.0f));
  compareMatrices(obj.transform(), glm::mat4(1.0f), __LINE__);

  // Check identity
  obj.scale(Vector3f(1.0f));
  compareMatrices(obj.transform(), glm::mat4(1.0f), __LINE__);

  // Check identity
  obj.rotate(0.0f, Vector3f(1.0f));
  compareMatrices(obj.transform(), glm::mat4(1.0f), __LINE__);

  // Change scale factor
  obj.scalefactor(Vector3f(4.0f));
  compareMatrices(obj.transform(), glm::scale(glm::mat4(1.0f), glm::vec3(4.0f, 4.0f, 4.0f)), __LINE__);

  // Check reset
  obj.reset();
  compareMatrices(obj.transform(), glm::mat4(1.0f), __LINE__);

  // Change origin
  obj.position(Vector3f(1.0f));
  compareMatrices(obj.transform(), glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f)), __LINE__);
  obj.origin(Vector3f(1.0f));
  compareMatrices(obj.transform(), glm::mat4(1.0f), __LINE__);
}
