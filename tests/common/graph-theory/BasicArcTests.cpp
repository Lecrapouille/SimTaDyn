//=====================================================================
// SimTaDyn: A GIS in a spreadsheet.
// Copyright 2017 Quentin Quadrat <lecrapouille@gmail.com>
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

#include "BasicArcTests.hpp"

// Register the test suite
CPPUNIT_TEST_SUITE_REGISTRATION(BasicArcTests);

//--------------------------------------------------------------------------
void BasicArcTests::setUp()
{
  // Reset node/arcs unique identifiers to 0
  BasicNode::resetID();
  BasicArc::resetID();
}

//--------------------------------------------------------------------------
void BasicArcTests::tearDown()
{
  // Reset node/arcs unique identifiers to 0
  BasicNode::resetID();
  BasicArc::resetID();
}

//--------------------------------------------------------------------------
void BasicArcTests::test()
{
  // Check if the unique identifier is auto incrementing
  BasicArc a0;
  BasicArc a1;
  BasicArc a2;

  CPPUNIT_ASSERT_EQUAL(0U, a0.id());
  CPPUNIT_ASSERT_EQUAL(true, 0U == a0);

  CPPUNIT_ASSERT_EQUAL(1U, a1.id());
  CPPUNIT_ASSERT_EQUAL(true, 1U == a1);
  CPPUNIT_ASSERT_EQUAL(2U, a2.id());
  CPPUNIT_ASSERT_EQUAL(true, 2U == a2);

  // Check if the unique identifier can be reseted to 0
  BasicArc::resetID();
  BasicArc a3;
  BasicArc a4;

  CPPUNIT_ASSERT_EQUAL(0U, a3.id());
  CPPUNIT_ASSERT_EQUAL(true, 0U == a3);
  CPPUNIT_ASSERT_EQUAL(1U, a4.id());
  CPPUNIT_ASSERT_EQUAL(true, 1U == a4);

  // Check cloning
  BasicArc* a = a2.clone();
  CPPUNIT_ASSERT_EQUAL(2U, a->id());
  CPPUNIT_ASSERT_EQUAL(true, 2U == *a);
  CPPUNIT_ASSERT_EQUAL(true, a2 == *a);
  CPPUNIT_ASSERT_EQUAL(true, a3 != *a);
  delete a;

  // Check constructor by copy
  a = new BasicArc(a1);
  CPPUNIT_ASSERT_EQUAL(1U, a->id());
  CPPUNIT_ASSERT_EQUAL(true, 1U == *a);
  delete a;

  // Check an arc is well created
  BasicNode n0, n1;
  CPPUNIT_ASSERT_EQUAL(0U, n0.id());
  CPPUNIT_ASSERT_EQUAL(1U, n1.id());
  BasicArc a5(42, n0, n1);
  CPPUNIT_ASSERT_EQUAL(42U, a5.id());
  CPPUNIT_ASSERT_EQUAL(true, 42U == a5);
  CPPUNIT_ASSERT_EQUAL(0U, a5.m_fromNode->id());
  CPPUNIT_ASSERT_EQUAL(1U, a5.m_toNode->id());
  CPPUNIT_ASSERT_EQUAL(true, 0U == a5.from());
  CPPUNIT_ASSERT_EQUAL(true, 1U == a5.to());
  CPPUNIT_ASSERT_EQUAL(true, 0U == a5.from().id());
  CPPUNIT_ASSERT_EQUAL(true, 1U == a5.to().id());
  CPPUNIT_ASSERT_EQUAL(true, n0 == a5.from());
  CPPUNIT_ASSERT_EQUAL(true, n1 == a5.to());

  // Node which will replace nodes of the arc
  BasicNode n2;
  CPPUNIT_ASSERT_EQUAL(2U, n2.id());
  CPPUNIT_ASSERT_EQUAL(true, n2 != a5.from());
  CPPUNIT_ASSERT_EQUAL(true, n2 != a5.to());
  CPPUNIT_ASSERT_EQUAL(false, n0 == n2);

  // Replace the tail node by a new node.
  a5.from(n2);
  CPPUNIT_ASSERT_EQUAL(n2.id(), a5.from().id());
  CPPUNIT_ASSERT_EQUAL(true, n2 == a5.from());
  CPPUNIT_ASSERT_EQUAL(true, n0.id() != a5.from().id());
  CPPUNIT_ASSERT_EQUAL(true, n0 != n2);
  CPPUNIT_ASSERT_EQUAL(true, n2 != a5.to());

  // Replace the head node by a new node.
  a5.to(n2);
  CPPUNIT_ASSERT_EQUAL(true, n1 != a5.to());
  CPPUNIT_ASSERT_EQUAL(true, n2 == a5.from());
  CPPUNIT_ASSERT_EQUAL(true, n2 == a5.to());

  // Change the id of the node. Check if references changed.
  n2.m_id = 55U;
  CPPUNIT_ASSERT_EQUAL(true, 55U == a5.from());
  CPPUNIT_ASSERT_EQUAL(true, 55U == a5.to());
  CPPUNIT_ASSERT_EQUAL(true, n2 == a5.from());
  CPPUNIT_ASSERT_EQUAL(true, n2 == a5.to());

  // Check auto-reference is allowed
  BasicArc a6(46, n0, n0);
  CPPUNIT_ASSERT_EQUAL(46U, a6.id());
  CPPUNIT_ASSERT_EQUAL(true, a6.from() == a6.to());
}
