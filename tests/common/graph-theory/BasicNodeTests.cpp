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

#include "BasicNodeTests.hpp"

// Register the test suite
CPPUNIT_TEST_SUITE_REGISTRATION(BasicNodeTests);

//--------------------------------------------------------------------------
void BasicNodeTests::setUp()
{
  BasicNode::resetID();
  BasicArc::resetID();
}

//--------------------------------------------------------------------------
void BasicNodeTests::tearDown()
{
  BasicNode::resetID();
  BasicArc::resetID();
}

//--------------------------------------------------------------------------
void BasicNodeTests::dummy()
{
  // Check unique identifier is auto incrementing
  BasicNode n0;
  BasicNode n1;
  BasicNode n2;

  CPPUNIT_ASSERT_EQUAL(0U, n0.id());
  CPPUNIT_ASSERT_EQUAL(true, 0U == n0);
  CPPUNIT_ASSERT_EQUAL(true, n0 == n0);

  CPPUNIT_ASSERT_EQUAL(1U, n1.id());
  CPPUNIT_ASSERT_EQUAL(true, 1U == n1);
  CPPUNIT_ASSERT_EQUAL(true, n1 == n1);
  CPPUNIT_ASSERT_EQUAL(true, n0 != n1);

  CPPUNIT_ASSERT_EQUAL(2U, n2.id());
  CPPUNIT_ASSERT_EQUAL(true, 2U == n2);

  // Check if the unique identifier can be reseted to 0
  BasicNode::resetID();
  BasicNode n3;
  BasicNode n4;

  CPPUNIT_ASSERT_EQUAL(0U, n3.id());
  CPPUNIT_ASSERT_EQUAL(true, 0U == n3);

  CPPUNIT_ASSERT_EQUAL(1U, n4.id());
  CPPUNIT_ASSERT_EQUAL(true, 1U == n4);

  // Check cloning
  BasicNode* n = n2.clone();
  CPPUNIT_ASSERT_EQUAL(2U, n->id());
  CPPUNIT_ASSERT_EQUAL(true, 2U == *n);
  CPPUNIT_ASSERT_EQUAL(true, n2 == *n);
  CPPUNIT_ASSERT_EQUAL(true, n3 != *n);
  delete n;

  // Check constructor by copy
  n = new BasicNode(n1);
  CPPUNIT_ASSERT_EQUAL(1U, n->id());
  CPPUNIT_ASSERT_EQUAL(true, 1U == *n);
  delete n;

  // Check constructor with forced id
  BasicNode n5(42);
  CPPUNIT_ASSERT_EQUAL(42U, n5.id());
  CPPUNIT_ASSERT_EQUAL(true, 42U == n5);
}

//--------------------------------------------------------------------------
void BasicNodeTests::neighbor()
{
  // Partially init arcs
  BasicNode n0;
  BasicArc a0;
  BasicArc a1;
  BasicArc a2;

  CPPUNIT_ASSERT_EQUAL(0U, n0.degree());
  CPPUNIT_ASSERT_EQUAL(true, 0U == a0);
  CPPUNIT_ASSERT_EQUAL(true, 1U == a1);
  CPPUNIT_ASSERT_EQUAL(true, 2U == a2);

  // Add new 3 arcs as neighbors to node 0.
  // Check they were added as neighbors.
  n0.addNeighbor(a0);
  n0.addNeighbor(a1);
  n0.addNeighbor(a2);
  CPPUNIT_ASSERT_EQUAL(3U, n0.degree());
  CPPUNIT_ASSERT_EQUAL(a0.id(), n0.m_arcs[0]->id());
  CPPUNIT_ASSERT_EQUAL(a1.id(), n0.m_arcs[1]->id());
  CPPUNIT_ASSERT_EQUAL(a2.id(), n0.m_arcs[2]->id());
  CPPUNIT_ASSERT_EQUAL(true, a0 == *(n0.m_arcs[0]));
  CPPUNIT_ASSERT_EQUAL(true, a1 == *(n0.m_arcs[1]));
  CPPUNIT_ASSERT_EQUAL(true, a2 == *(n0.m_arcs[2]));
  CPPUNIT_ASSERT_EQUAL(true, a0 == *(n0.neighbor(0)));
  CPPUNIT_ASSERT_EQUAL(true, a1 == *(n0.neighbor(1)));
  CPPUNIT_ASSERT_EQUAL(true, a2 == *(n0.neighbor(2)));

  // Remove one neighbor. Check other neighbors.
  n0.removeNeighbor(a1);
  CPPUNIT_ASSERT_EQUAL(2U, n0.degree());
  CPPUNIT_ASSERT_EQUAL(a0.id(), n0.m_arcs[0]->id());
  CPPUNIT_ASSERT_EQUAL(a2.id(), n0.m_arcs[1]->id());
  CPPUNIT_ASSERT_EQUAL(true, a0 == *(n0.m_arcs[0]));
  CPPUNIT_ASSERT_EQUAL(true, a2 == *(n0.m_arcs[1]));
  CPPUNIT_ASSERT_EQUAL(true, a0 == *(n0.neighbor(0)));
  CPPUNIT_ASSERT_EQUAL(true, a2 == *(n0.neighbor(1)));

  // Remove one neighbor. Check other neighbors.
  n0.removeNthNeighbor(0);
  CPPUNIT_ASSERT_EQUAL(1U, n0.degree());
  CPPUNIT_ASSERT_EQUAL(a2.id(), n0.m_arcs[0]->id());
  CPPUNIT_ASSERT_EQUAL(true, a2 == *(n0.m_arcs[0]));
  CPPUNIT_ASSERT_EQUAL(true, a2 == *(n0.neighbor(0)));

  // Add a new neighbor. Check other neighbors.
  n0.addNeighbor(a1);
  CPPUNIT_ASSERT_EQUAL(2U, n0.degree());
  CPPUNIT_ASSERT_EQUAL(a2.id(), n0.m_arcs[0]->id());
  CPPUNIT_ASSERT_EQUAL(a1.id(), n0.m_arcs[1]->id());
  CPPUNIT_ASSERT_EQUAL(true, a2 == *(n0.m_arcs[0]));
  CPPUNIT_ASSERT_EQUAL(true, a1 == *(n0.m_arcs[1]));
  CPPUNIT_ASSERT_EQUAL(true, a2 == *(n0.neighbor(0)));
  CPPUNIT_ASSERT_EQUAL(true, a1 == *(n0.neighbor(1)));

  // Remove alle neigbhbors. Check the list is empty.
  n0.removeAllNeighbors();
  CPPUNIT_ASSERT_EQUAL(0U, n0.degree());
  CPPUNIT_ASSERT_EQUAL(true, nullptr == n0.neighbor(0));

  // Add new neighbors
  n0.addNeighbor(a0);
  n0.addNeighbor(a1);
  n0.addNeighbor(a2);
  n0.addNeighbor(a0);
  n0.addNeighbor(a1);
  n0.addNeighbor(a2);
  CPPUNIT_ASSERT_EQUAL(6U, n0.degree()); // FIXME: should not be unique neighbors ?

  // Try to remove incorrect neighbor. Check that none was removed.
  n0.removeNthNeighbor(16);
  CPPUNIT_ASSERT_EQUAL(6U, n0.degree());
  CPPUNIT_ASSERT_EQUAL(a0.id(), n0.m_arcs[0]->id());
  CPPUNIT_ASSERT_EQUAL(a1.id(), n0.m_arcs[1]->id());
  CPPUNIT_ASSERT_EQUAL(a2.id(), n0.m_arcs[2]->id());
  CPPUNIT_ASSERT_EQUAL(a0.id(), n0.m_arcs[3]->id());
  CPPUNIT_ASSERT_EQUAL(a1.id(), n0.m_arcs[4]->id());
  CPPUNIT_ASSERT_EQUAL(a2.id(), n0.m_arcs[5]->id());
  CPPUNIT_ASSERT_EQUAL(true, a0 == *(n0.m_arcs[0]));
  CPPUNIT_ASSERT_EQUAL(true, a1 == *(n0.m_arcs[1]));
  CPPUNIT_ASSERT_EQUAL(true, a2 == *(n0.m_arcs[2]));
  CPPUNIT_ASSERT_EQUAL(true, a0 == *(n0.m_arcs[3]));
  CPPUNIT_ASSERT_EQUAL(true, a1 == *(n0.m_arcs[4])); // FIXME: should not be removed ?
  CPPUNIT_ASSERT_EQUAL(true, a2 == *(n0.m_arcs[5]));

  // Remove a specific neighbor. Check the the
  // element to be removed have been swaped with
  // the last element in the list.
  n0.removeNeighbor(a1);
  CPPUNIT_ASSERT_EQUAL(5U, n0.degree());
  CPPUNIT_ASSERT_EQUAL(true, a0 == *(n0.m_arcs[0]));
  CPPUNIT_ASSERT_EQUAL(true, a2 == *(n0.m_arcs[1]));
  CPPUNIT_ASSERT_EQUAL(true, a2 == *(n0.m_arcs[2]));
  CPPUNIT_ASSERT_EQUAL(true, a0 == *(n0.m_arcs[3])); // FIXME: should not be removed ?
  CPPUNIT_ASSERT_EQUAL(true, a1 == *(n0.m_arcs[4]));

  CPPUNIT_ASSERT_EQUAL(true, a0 == *(n0.neighbor(0)));
  CPPUNIT_ASSERT_EQUAL(true, a2 == *(n0.neighbor(1)));
  CPPUNIT_ASSERT_EQUAL(true, a2 == *(n0.neighbor(2)));
  CPPUNIT_ASSERT_EQUAL(true, a0 == *(n0.neighbor(3)));
  CPPUNIT_ASSERT_EQUAL(true, a1 == *(n0.neighbor(4)));

  n0.removeNthNeighbor(2);
  CPPUNIT_ASSERT_EQUAL(4U, n0.degree());
  CPPUNIT_ASSERT_EQUAL(true, a0 == *(n0.m_arcs[0]));
  CPPUNIT_ASSERT_EQUAL(true, a2 == *(n0.m_arcs[1]));
  CPPUNIT_ASSERT_EQUAL(true, a1 == *(n0.m_arcs[2]));
  CPPUNIT_ASSERT_EQUAL(true, a0 == *(n0.m_arcs[3]));

  n0.removeNthNeighbor(3);
  CPPUNIT_ASSERT_EQUAL(3U, n0.degree());
  CPPUNIT_ASSERT_EQUAL(true, a0 == *(n0.m_arcs[0]));
  CPPUNIT_ASSERT_EQUAL(true, a2 == *(n0.m_arcs[1]));
  CPPUNIT_ASSERT_EQUAL(true, a1 == *(n0.m_arcs[2]));

  // Wrong remove. Check nothing have been removed
  BasicArc a3;
  CPPUNIT_ASSERT_EQUAL(true, 3U == a3);
  n0.removeNeighbor(a3);
  CPPUNIT_ASSERT_EQUAL(3U, n0.degree());
  CPPUNIT_ASSERT_EQUAL(true, a0 == *(n0.m_arcs[0]));
  CPPUNIT_ASSERT_EQUAL(true, a2 == *(n0.m_arcs[1]));
  CPPUNIT_ASSERT_EQUAL(true, a1 == *(n0.m_arcs[2]));

  // Try to access to the wrong neighbor
  CPPUNIT_ASSERT_EQUAL(true, nullptr == n0.neighbor(18U));
  CPPUNIT_ASSERT_EQUAL(true, a0 == n0.nthNeighbor(0U));
  CPPUNIT_ASSERT_EQUAL(true, a2 == n0.nthNeighbor(1U));
  CPPUNIT_ASSERT_EQUAL(true, a1 == n0.nthNeighbor(2U));

  // Correct access to neighbors.
  const BasicArc &a4 = n0.nthNeighbor(2U);
  CPPUNIT_ASSERT_EQUAL(true, a1 == a4);
  const BasicArc *a5 = n0.neighbor(2U);
  CPPUNIT_ASSERT_EQUAL(true, a1 == *a5);
  // Incorrect access to neighbors.
  CPPUNIT_ASSERT_EQUAL(true, nullptr == n0.neighbor(18U));

  // Remove all neighbors. Check if the list is empty.
  n0.removeAllNeighbors();
  CPPUNIT_ASSERT_EQUAL(0U, n0.degree());
}
