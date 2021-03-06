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

#include "GLVAOTests.hpp"

//--------------------------------------------------------------------------
void GLVAOTests::setUp()
{
  std::cout << "*** GLVAOTests ***************************************" << std::endl;
}

//--------------------------------------------------------------------------
void GLVAOTests::tearDown()
{
}

//--------------------------------------------------------------------------
void GLVAOTests::tests()
{
  // VAO with no name
  GLVAO vao0("");
  CPPUNIT_ASSERT_EQUAL(true, vao0.name() == "");
  CPPUNIT_ASSERT(GL_ARRAY_BUFFER == vao0.target());

  // VAO with name
  GLVAO vao("vao");
  CPPUNIT_ASSERT_EQUAL(true, vao.name() == "vao");
  CPPUNIT_ASSERT(GL_ARRAY_BUFFER == vao.target());

  // Check there is no VBOs
  std::vector<std::string> vbo_names = vao.VBONames();
  CPPUNIT_ASSERT_EQUAL(0_z, vbo_names.size());
  CPPUNIT_ASSERT_EQUAL(false, vao.hasVBOs());
  CPPUNIT_ASSERT_EQUAL(false, vao.hasVBO(""));

  // Add the first VBO. Check it has been inserted.
  CPPUNIT_ASSERT_EQUAL(true, vao.createVBO<float>("vbo1", 0_z, BufferUsage::DYNAMIC_DRAW));
  vbo_names = vao.VBONames();
  CPPUNIT_ASSERT_EQUAL(1_z, vbo_names.size());
  CPPUNIT_ASSERT_EQUAL(true, vbo_names[0] == "vbo1");
  CPPUNIT_ASSERT_EQUAL(true, vao.hasVBO("vbo1"));
  CPPUNIT_ASSERT_EQUAL(false, vao.hasVBO(""));

  // FIXME: is it a wanted behavior ?
  // Add another VBO: same name but different type.
  // Check it has not been inserted.
  CPPUNIT_ASSERT_EQUAL(false, vao.createVBO<int>("vbo1", 0_z, BufferUsage::DYNAMIC_DRAW));
  vbo_names = vao.VBONames();
  CPPUNIT_ASSERT_EQUAL(1_z, vbo_names.size());
  CPPUNIT_ASSERT_EQUAL(true, vbo_names[0] == "vbo1");
  CPPUNIT_ASSERT_EQUAL(true, vao.hasVBO("vbo1"));
  CPPUNIT_ASSERT_EQUAL(false, vao.hasVBO("vbo2"));

  // Add the second VBO. Check it has been inserted.
  CPPUNIT_ASSERT_EQUAL(true, vao.createVBO<float>("vbo2", 0_z, BufferUsage::DYNAMIC_DRAW));
  vbo_names = vao.VBONames();
  std::sort(vbo_names.begin(), vbo_names.end());
  CPPUNIT_ASSERT_EQUAL(2_z, vbo_names.size());
  CPPUNIT_ASSERT_EQUAL(true, vbo_names[0] == "vbo1");
  CPPUNIT_ASSERT_EQUAL(true, vbo_names[1] == "vbo2");
  CPPUNIT_ASSERT_EQUAL(true, vao.hasVBO("vbo1"));
  CPPUNIT_ASSERT_EQUAL(true, vao.hasVBO("vbo2"));
  CPPUNIT_ASSERT_EQUAL(false, vao.hasVBO(""));

  // Get a VBO
  GLVertexBuffer<float>& vbof = vao.VBO<float>("vbo2");
  CPPUNIT_ASSERT_EQUAL(true, vbof.name() == "vbo2");
  CPPUNIT_ASSERT_EQUAL(true, vbof.needCreate());
  CPPUNIT_ASSERT_EQUAL(true, vbof.needSetup());
  CPPUNIT_ASSERT_EQUAL(false, vbof.needUpdate());
  CPPUNIT_ASSERT_EQUAL(false, vbof.canBeReleased());

  // Get a VBO with the type not inserted
  try {
    GLVertexBuffer<int>& vboi = vao.VBO<int>("vbo1");
    std::cout << vboi.name() << std::endl;
    CPPUNIT_FAIL("Unknown VBO Exception not caught");
  }
  catch (OpenGLException) { }

  // Nullptr exception
  try
    {
      GLVertexBuffer<int>& vbo = vao.VBO<int>(nullptr);
      std::cout << vbo.name() << std::endl;
      CPPUNIT_FAIL("Nullptr Exception not caught");
    }
  catch (OpenGLException) { }

  // Unknown VBO
  try
    {
      GLVertexBuffer<int>& vbo = vao.VBO<int>("");
      std::cout << vbo.name() << std::endl;
      CPPUNIT_FAIL("Unknown VBO Exception not caught");
    }
  catch (OpenGLException) { }

  // Bad cast VBO
  try
    {
      GLVertexBuffer<double>& vbo = vao.VBO<double>("vbo1");
      std::cout << vbo.name() << std::endl;
      CPPUNIT_FAIL("Bad casted VBO Exception not caught");
    }
  catch (OpenGLException) { }
}
