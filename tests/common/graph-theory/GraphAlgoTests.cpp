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

#include "GraphAlgoTests.hpp"

// Register the test suite
CPPUNIT_TEST_SUITE_REGISTRATION(GraphAlgoTests);

//--------------------------------------------------------------------------
void GraphAlgoTests::setUp()
{
  std::cout << "*** GraphAlgoTests ***************************************" << std::endl;
}

//--------------------------------------------------------------------------
void GraphAlgoTests::tearDown()
{
}

//--------------------------------------------------------------------------
static void checkVisited(BasicGraph<BasicNode, BasicArc> &g, Key nodeID)
{
  g.markNode(nodeID);
  CPPUNIT_ASSERT_EQUAL(true, g.markedNode(nodeID));
  g.unmarkNode(nodeID);
  CPPUNIT_ASSERT_EQUAL(false, g.markedNode(nodeID));
}

//--------------------------------------------------------------------------
void GraphAlgoTests::test()
{
  const char *name = "MonGraphe";
  BasicGraph<BasicNode, BasicArc> g(name, false);

  // Check if graph is empty
  CPPUNIT_ASSERT_EQUAL(0, g.m_name.compare(name));
  CPPUNIT_ASSERT_EQUAL(false, g.directed());
  CPPUNIT_ASSERT_EQUAL(true, g.empty());
  CPPUNIT_ASSERT_EQUAL(0U, g.howManyNodes());
  CPPUNIT_ASSERT_EQUAL(0U, g.howManyArcs());

  // Build a graph like described in wikipedia (french pages)
  g.addArc('A', 'B');
  g.addArc('A', 'C');
  g.addArc('A', 'E');
  g.addArc('B', 'D');
  g.addArc('B', 'F');
  g.addArc('C', 'G');
  g.addArc('E', 'F');

  // Check number of elements in the graph
  CPPUNIT_ASSERT_EQUAL(false, g.empty());
  CPPUNIT_ASSERT_EQUAL(7U, g.howManyNodes());
  CPPUNIT_ASSERT_EQUAL(7U, g.howManyArcs());

  // Check if elements are present
  for (char i = 'A'; i < 'H'; ++i)
    {
      CPPUNIT_ASSERT_EQUAL(true, g.hasNode(i));
      std::cout << g.getNode(i) << std::endl;
    }
  for (uint32_t i = 0; i < g.howManyArcs(); ++i)
    {
      CPPUNIT_ASSERT_EQUAL(true, g.hasArc(i));
      std::cout << g.getArc(i) << std::endl;
    }

  // Check degrees
  CPPUNIT_ASSERT_EQUAL(3U, g.getNode('A').degree());
  CPPUNIT_ASSERT_EQUAL(3U, g.getNode('B').degree());
  CPPUNIT_ASSERT_EQUAL(2U, g.getNode('C').degree());
  CPPUNIT_ASSERT_EQUAL(1U, g.getNode('D').degree());
  CPPUNIT_ASSERT_EQUAL(2U, g.getNode('E').degree());
  CPPUNIT_ASSERT_EQUAL(2U, g.getNode('F').degree());

  // Check visited marks
  for (char i = 'A'; i < 'H'; ++i)
    {
      checkVisited(g, i);
    }

  // Check Depth First Search algorithm
  GraphAlgorithm gA;
  Key expected_nodes[7U] = { 'A', 'B', 'D', 'F', 'E', 'C', 'G' };
  gA.depthFirstSearch(g, 'A');
  CPPUNIT_ASSERT_EQUAL(7U, g.howManyNodes());
  CPPUNIT_ASSERT_EQUAL(true, g.howManyNodes() == static_cast<uint32_t>(gA.m_result.capacity()));
  for (uint32_t i = 0; i < g.howManyArcs(); ++i)
    {
      //FIXME CPPUNIT_ASSERT_EQUAL(expected_nodes[i], gA.m_result[i]);
    }
}
