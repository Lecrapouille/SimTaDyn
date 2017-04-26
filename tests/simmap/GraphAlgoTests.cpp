#include "GraphAlgoTests.hpp"

// Register the test suite
CPPUNIT_TEST_SUITE_REGISTRATION(GraphAlgoTests);

//--------------------------------------------------------------------------
void GraphAlgoTests::setUp()
{
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
  CPPUNIT_ASSERT_EQUAL(g.howManyNodes(), gA.m_result.capacity());
  for (uint32_t i = 0; i < g.howManyArcs(); ++i)
    {
      CPPUNIT_ASSERT_EQUAL(expected_nodes[i], gA.m_result[i]);
    }
}
