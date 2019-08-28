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

#include "GLProgramTests.hpp"

//--------------------------------------------------------------------------
void GLProgramTests::setUp()
{
  std::cout << "*** GLProgramTests ***************************************" << std::endl;
}

//--------------------------------------------------------------------------
void GLProgramTests::tearDown()
{
}

//--------------------------------------------------------------------------
void GLProgramTests::tests()
{
  GLProgram prog("prog");

  CPPUNIT_ASSERT_EQUAL(false, prog.isCompiled());
  CPPUNIT_ASSERT_EQUAL(false, prog.isBound());
  CPPUNIT_ASSERT_EQUAL(false, prog.hasErrored());
  CPPUNIT_ASSERT( prog.getError() == "");
  CPPUNIT_ASSERT_EQUAL(0_z, prog.getShaderNames().size());
  CPPUNIT_ASSERT_EQUAL(0_z, prog.getFailedShaders().size());
  CPPUNIT_ASSERT_EQUAL(0_z, prog.getAttributeNames().size());
  CPPUNIT_ASSERT_EQUAL(0_z, prog.getUniformNames().size());

  CPPUNIT_ASSERT_EQUAL(0_z, prog.getUniformNames().size());
  CPPUNIT_ASSERT_EQUAL(false, prog.hasUniforms());
  CPPUNIT_ASSERT_EQUAL(false, prog.hasAttributes());
  CPPUNIT_ASSERT_EQUAL(false, prog.hasUniform(""));
  CPPUNIT_ASSERT_EQUAL(0_z, prog.getUniformNames().size());
  CPPUNIT_ASSERT_EQUAL(false, prog.hasAttribute(""));
  CPPUNIT_ASSERT_EQUAL(false, prog.hasUniform(nullptr));
  CPPUNIT_ASSERT_EQUAL(false, prog.hasAttribute(nullptr));
  CPPUNIT_ASSERT_EQUAL(0_z, prog.getUniformNames().size());

  //---
  try {
    prog.throw_if_not_compiled();
    CPPUNIT_FAIL("Exception should have occured");
  } catch(OpenGLException&) { }

  prog.m_compiled = true;
  try {
    prog.throw_if_not_compiled();
  } catch(OpenGLException&) { CPPUNIT_FAIL("Exception should have not occured"); }

  // ---
  try {
    prog.uniform<int>("");
    CPPUNIT_FAIL("Exception should have occured");
  } catch(OpenGLException&) { }

  try {
    prog.uniform<int>("aaa");
    CPPUNIT_FAIL("Exception should have occured");
  } catch(OpenGLException&) { }

  try {
    prog.uniform<int>(nullptr);
    CPPUNIT_FAIL("Exception should have occured");
  } catch(OpenGLException&) { }

  // TODO: try add name conflict wit different types
  prog.addNewUniform(GL_FLOAT, "u1");
  prog.addNewUniform(GL_FLOAT_VEC2, "u2");
  prog.addNewUniform(GL_FLOAT_VEC3, "u3");
  prog.addNewUniform(GL_FLOAT_VEC4, "u4");
  CPPUNIT_ASSERT_EQUAL(4_z, prog.getUniformNames().size());
  CPPUNIT_ASSERT(prog.hasUniform("u1"));
  CPPUNIT_ASSERT(prog.hasUniform("u2"));
  CPPUNIT_ASSERT(prog.hasUniform("u3"));
  CPPUNIT_ASSERT(prog.hasUniform("u4"));

  //Old API
  //try {
  //  prog.getVBO<int>("");
  //  CPPUNIT_FAIL("Exception should have occured");
  //} catch(OpenGLException&) { }

  GLVAO vao("VAO");
  CPPUNIT_ASSERT_EQUAL(0u, vao.prog);
  prog.m_handle = 42;
  CPPUNIT_ASSERT(prog.bind(vao));
  CPPUNIT_ASSERT(prog.isBound());
  CPPUNIT_ASSERT_EQUAL(42u, vao.prog);
  CPPUNIT_ASSERT(prog.m_vao == &vao);

  //Old API
  //try {
  //  prog.getVBO<int>("");
  //  CPPUNIT_FAIL("Exception should have occured");
  //} catch(OpenGLException&) { }

  // TODO: try add name conflict wit different types
  CPPUNIT_ASSERT_EQUAL(0_z, prog.getAttributeNames().size());
  prog.addNewAttribute(GL_FLOAT, "a1");
  prog.addNewAttribute(GL_FLOAT_VEC2, "a2");
  prog.addNewAttribute(GL_FLOAT_VEC3, "a3");
  prog.addNewAttribute(GL_FLOAT_VEC4, "a4");
  CPPUNIT_ASSERT_EQUAL(4_z, prog.getAttributeNames().size());
  CPPUNIT_ASSERT(prog.hasAttribute("a1"));
  CPPUNIT_ASSERT(prog.hasAttribute("a2"));
  CPPUNIT_ASSERT(prog.hasAttribute("a3"));
  CPPUNIT_ASSERT(prog.hasAttribute("a4"));

  GLVertexShader shader("foo");
  prog.attachShader(shader);
  auto getShaderNames = prog.getShaderNames();
  CPPUNIT_ASSERT_EQUAL(1_z, getShaderNames.size());
  CPPUNIT_ASSERT(getShaderNames.end() !=
            std::find(getShaderNames.begin(), getShaderNames.end(), "foo"));

  GLVAO vao2("VAO2");
  CPPUNIT_ASSERT_EQUAL(0u, vao2.prog);
  prog.initVAO(vao2);
  CPPUNIT_ASSERT_EQUAL(42u, vao2.prog);
  CPPUNIT_ASSERT(vao2.hasVBOs());
  CPPUNIT_ASSERT(vao2.hasVBO("a1"));
  CPPUNIT_ASSERT(vao2.hasVBO("a2"));
  CPPUNIT_ASSERT(vao2.hasVBO("a3"));
  CPPUNIT_ASSERT(vao2.hasVBO("a4"));
  CPPUNIT_ASSERT(prog.m_vao != &vao2);
  CPPUNIT_ASSERT(prog.m_vao == &vao);

  // Restore uninitialized prog else OpenGL will segfault
  prog.m_handle = 0;

  // Bind VAO to the wrong GLProg
  GLProgram prog1("prog1");
  prog1.m_handle = 42;
  prog1.m_compiled = true;
  GLProgram prog2("prog2");
  prog2.m_handle = 43;
  prog2.m_compiled = true;
  GLVAO vao3("VAO3");
  GLVAO vao4("VAO4");

  CPPUNIT_ASSERT_EQUAL(0u, vao3.prog);
  CPPUNIT_ASSERT_EQUAL(0u, vao4.prog);
  CPPUNIT_ASSERT_EQUAL(true, prog1.bind(vao3));
  CPPUNIT_ASSERT_EQUAL(true, prog2.bind(vao4));
  CPPUNIT_ASSERT_EQUAL(42u, vao3.prog);
  CPPUNIT_ASSERT_EQUAL(43u, vao4.prog);
  // Try binding VAO to the wrong GLProg
  CPPUNIT_ASSERT_EQUAL(false, prog1.bind(vao4));
  CPPUNIT_ASSERT_EQUAL(false, prog2.bind(vao3));
}
