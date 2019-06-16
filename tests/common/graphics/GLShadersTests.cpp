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

#include "GLShadersTests.hpp"

//--------------------------------------------------------------------------
void GLShadersTests::setUp()
{
  std::cout << "*** GLShadersTests ***************************************" << std::endl;
}

//--------------------------------------------------------------------------
void GLShadersTests::tearDown()
{
}

//--------------------------------------------------------------------------
void GLShadersTests::tests()
{
  GLVertexShader shaderNoName1;
  CPPUNIT_ASSERT(shaderNoName1.name() == "VertexShader");
  GLFragmentShader shaderNoName2;
  CPPUNIT_ASSERT(shaderNoName2.name() == "FragmentShader");
  GLGeometryShader shaderNoName3;
  CPPUNIT_ASSERT(shaderNoName3.name() == "GeometryShader");


  GLVertexShader shader1("vert");
  CPPUNIT_ASSERT(shader1.name() == "vert");
  CPPUNIT_ASSERT(GL_VERTEX_SHADER == shader1.target());

  GLFragmentShader shader2("frag");
  CPPUNIT_ASSERT(shader2.name() == "frag");
  CPPUNIT_ASSERT(GL_FRAGMENT_SHADER == shader2.target());

  GLGeometryShader shader3;
  CPPUNIT_ASSERT(shader3.name() == "GeometryShader");
  CPPUNIT_ASSERT(GL_GEOMETRY_SHADER == shader3.target());

  //--- Check initial state
  CPPUNIT_ASSERT(shader1.m_shader_code == "");
  CPPUNIT_ASSERT(shader1.m_error_msg == "");
  CPPUNIT_ASSERT(!shader1.m_compiled);
  CPPUNIT_ASSERT(0 == shader1.m_attached);

  //--- Check initial state
  CPPUNIT_ASSERT(shader2.code() == "");
  CPPUNIT_ASSERT(shader2.getError() == "");
  CPPUNIT_ASSERT(!shader2.hasErrored());
  CPPUNIT_ASSERT(!shader2.isCompiled());
  CPPUNIT_ASSERT(0 == shader2.m_attached);

  //--- Load from string
  shader1.fromString("foobar");
  CPPUNIT_ASSERT(!shader1.hasErrored());
  CPPUNIT_ASSERT(shader1.code() == "foobar");
  CPPUNIT_ASSERT(shader1.loaded());
  CPPUNIT_ASSERT(!shader1.isCompiled());
  try {
    shader1.throw_if_not_loaded();
  } catch(...) { CPPUNIT_FAIL("Exception should have not occured"); }

  try {
    shader1.throw_if_already_compiled();
    CPPUNIT_FAIL("Exception should have occured");
  } catch(...) { }

  shader1.fromString("");
  CPPUNIT_ASSERT(!shader1.hasErrored());
  CPPUNIT_ASSERT(shader1.code() == "");
  CPPUNIT_ASSERT(!shader1.loaded());
  CPPUNIT_ASSERT(!shader1.isCompiled());

  //--- Load from wrong file
  CPPUNIT_ASSERT(shader3.code() == "");
  CPPUNIT_ASSERT(!shader3.fromFile("this_file_does_not_exist"));
  CPPUNIT_ASSERT(!shader3.isCompiled());
  CPPUNIT_ASSERT(shader3.hasErrored());
  CPPUNIT_ASSERT(shader3.code() == "");
  CPPUNIT_ASSERT(!shader3.isCompiled());
  CPPUNIT_ASSERT(shader3.getError() != "");
  CPPUNIT_ASSERT(!shader3.hasErrored());
  try {
    shader3.throw_if_not_loaded();
    CPPUNIT_FAIL("Exception should have occured");
  } catch(...) {  }
  try {
    shader3.throw_if_already_compiled();
  } catch(...) { CPPUNIT_FAIL("Exception should have not occured"); }
  shader3.m_need_setup = false;
  try {
    shader3.throw_if_already_compiled();
    CPPUNIT_FAIL("Exception should have occured");
  } catch(...) { }

  //---
  shader2.attachProg(0);
  CPPUNIT_ASSERT_EQUAL(0u, shader2.attached());
  shader2.attachProg(42);
  CPPUNIT_ASSERT_EQUAL(42u, shader2.attached());

  //--- Try setup
  shader1.m_error_msg = "";
  shader1.fromString(""); shader1.m_compiled = true;
  shader1.setup();
  CPPUNIT_ASSERT(shader1.hasErrored());
  CPPUNIT_ASSERT(shader1.getError() != "");
  shader1.setup();

  //--- Try setup
  shader1.m_error_msg = "";
  shader1.fromString("ff"); shader1.m_compiled = true;
  shader1.setup();
  CPPUNIT_ASSERT(shader1.hasErrored());
  CPPUNIT_ASSERT(shader1.getError() != "");
  shader1.setup();


  // TODO shader1.begin();
}
