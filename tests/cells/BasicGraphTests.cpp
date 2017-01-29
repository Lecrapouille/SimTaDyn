#include "BasicGraphTests.hpp"

// Register the test suite
CPPUNIT_TEST_SUITE_REGISTRATION(BasicGraphTests);

//--------------------------------------------------------------------------
void BasicGraphTests::setUp()
{
}

//--------------------------------------------------------------------------
void BasicGraphTests::tearDown()
{
}

//--------------------------------------------------------------------------
void BasicGraphTests::test()
{
  // Empty graph
  BasicGraph<BasicNode, BasicArc> *g1 = new BasicGraph<BasicNode, BasicArc>();
  CPPUNIT_ASSERT_EQUAL(1U, g1->m_nodes.allocated());
  CPPUNIT_ASSERT_EQUAL(0U, g1->m_nodes.occupation());
  CPPUNIT_ASSERT_EQUAL(true, g1->m_nodes.empty());
  CPPUNIT_ASSERT_EQUAL(false, g1->m_nodes.full());
  CPPUNIT_ASSERT_EQUAL(1U, g1->m_arcs.allocated());
  CPPUNIT_ASSERT_EQUAL(0U, g1->m_arcs.occupation());
  CPPUNIT_ASSERT_EQUAL(true, g1->m_arcs.empty());
  CPPUNIT_ASSERT_EQUAL(false, g1->m_arcs.full());
  CPPUNIT_ASSERT_EQUAL(false, g1->hasNode(5));
  CPPUNIT_ASSERT_EQUAL(false, g1->hasArc(0));

  // Add one node
  g1->addNode(5);
  CPPUNIT_ASSERT_EQUAL(true, g1->hasNode(5));
  CPPUNIT_ASSERT_EQUAL(1U, g1->howManyNodes());
  CPPUNIT_ASSERT_EQUAL(0U, g1->howManyArcs());
  CPPUNIT_ASSERT_EQUAL(5U, g1->getNode(5)->id());
  CPPUNIT_ASSERT_EQUAL(1U + g1->getNode(5)->id() / g1->m_nodes.poolSizeAllocation(),
                       g1->m_nodes.allocated());
  CPPUNIT_ASSERT_EQUAL(1U, g1->m_nodes.occupation());
  CPPUNIT_ASSERT_EQUAL(false, g1->m_nodes.empty());
  CPPUNIT_ASSERT_EQUAL(false, g1->m_nodes.full());
  CPPUNIT_ASSERT_EQUAL(1U, g1->m_arcs.allocated());
  CPPUNIT_ASSERT_EQUAL(0U, g1->m_arcs.occupation());
  CPPUNIT_ASSERT_EQUAL(true, g1->m_arcs.empty());
  CPPUNIT_ASSERT_EQUAL(false, g1->m_arcs.full());

  // Add again the same node
  g1->addNode(5);
  CPPUNIT_ASSERT_EQUAL(true, g1->hasNode(5));
  CPPUNIT_ASSERT_EQUAL(1U, g1->howManyNodes());
  CPPUNIT_ASSERT_EQUAL(0U, g1->howManyArcs());
  CPPUNIT_ASSERT_EQUAL(5U, g1->getNode(5)->id());
  CPPUNIT_ASSERT_EQUAL(1U + g1->getNode(5)->id() / g1->m_nodes.poolSizeAllocation(),
                       g1->m_nodes.allocated());
  CPPUNIT_ASSERT_EQUAL(1U, g1->m_nodes.occupation());
  CPPUNIT_ASSERT_EQUAL(false, g1->m_nodes.empty());
  CPPUNIT_ASSERT_EQUAL(false, g1->m_nodes.full());
  CPPUNIT_ASSERT_EQUAL(1U, g1->m_arcs.allocated());
  CPPUNIT_ASSERT_EQUAL(0U, g1->m_arcs.occupation());
  CPPUNIT_ASSERT_EQUAL(true, g1->m_arcs.empty());
  CPPUNIT_ASSERT_EQUAL(false, g1->m_arcs.full());

  // Degree
  BasicNode *n1 = g1->getNode(5);
  CPPUNIT_ASSERT_EQUAL(true, nullptr != n1);
  CPPUNIT_ASSERT_EQUAL(0U, n1->degree());

  // Add arc between the node already created and a one not
  // defined
  g1->addArc(5U, 2U);
  //g1->debugNodes();
  //g1->debugArcs();
  CPPUNIT_ASSERT_EQUAL(1U, n1->degree());
  CPPUNIT_ASSERT_EQUAL(true, g1->hasNode(5U));
  CPPUNIT_ASSERT_EQUAL(true, g1->hasNode(2U));
  CPPUNIT_ASSERT_EQUAL(true, g1->hasArc(0U));
  CPPUNIT_ASSERT_EQUAL(2U, g1->howManyNodes());
  CPPUNIT_ASSERT_EQUAL(1U, g1->howManyArcs());
  CPPUNIT_ASSERT_EQUAL(5U, g1->getNode(5)->id());
  CPPUNIT_ASSERT_EQUAL(1U, g1->m_arcs.allocated());
  CPPUNIT_ASSERT_EQUAL(1U, g1->m_arcs.occupation());
  CPPUNIT_ASSERT_EQUAL(false, g1->m_arcs.empty());
  CPPUNIT_ASSERT_EQUAL(false, g1->m_arcs.full());

  // Access to correct arc
  BasicArc *a1 = g1->getArc(5U, 2U);
  CPPUNIT_ASSERT_EQUAL(0U, a1->id());
  CPPUNIT_ASSERT_EQUAL(5U, a1->from().id());
  CPPUNIT_ASSERT_EQUAL(2U, a1->to().id());

  // Neighbors
  BasicArc *a2 = n1->neighbor(0);
  CPPUNIT_ASSERT_EQUAL(0U, a2->id());
  CPPUNIT_ASSERT_EQUAL(5U, a2->from().id());
  CPPUNIT_ASSERT_EQUAL(2U, a2->to().id());
  const std::vector<BasicArc*> *neighbors = g1->neighbors(5);
  CPPUNIT_ASSERT_EQUAL(true, 1 == neighbors->size());

  // Access to erroneous arc
  a1 = g1->getArc(2U, 5U);
  CPPUNIT_ASSERT_EQUAL(true, nullptr == a1);

  //
  BasicNode n2(8);
  g1->addNode(n2);
  CPPUNIT_ASSERT_EQUAL(true, g1->hasNode(8));
  CPPUNIT_ASSERT_EQUAL(true, g1->hasNode(5U));
  CPPUNIT_ASSERT_EQUAL(true, g1->hasNode(2U));
  CPPUNIT_ASSERT_EQUAL(3U, g1->howManyNodes());

  //
  g1->addArc(5U, 8U);
  CPPUNIT_ASSERT_EQUAL(true, g1->hasNode(8));
  CPPUNIT_ASSERT_EQUAL(true, g1->hasNode(5U));
  CPPUNIT_ASSERT_EQUAL(true, g1->hasNode(2U));
  CPPUNIT_ASSERT_EQUAL(3U, g1->howManyNodes());
  CPPUNIT_ASSERT_EQUAL(true, g1->hasArc(0U));
  CPPUNIT_ASSERT_EQUAL(true, g1->hasArc(1U));
  CPPUNIT_ASSERT_EQUAL(2U, g1->howManyArcs());
  CPPUNIT_ASSERT_EQUAL(2U, n1->degree());

  //std::cout << n1 << std::endl;
  //n1->removeArc(0);
  //std::cout << n1 << std::endl;

  //
  //g1->removeNode(5U);
  /*std::cout << "Avant\n";
  g1->debugNodes();
  g1->debugArcs();

  std::cout << "============================\n";
  std::cout << "\nRemoving Arc 0\n\n";
  g1->removeArc(0U);
  g1->debugNodes();
  std::cout << "\n\nRemoving Arc 1\n";
  g1->removeArc(1U);
  g1->debugNodes();
  g1->debugArcs();*/

  // Delete graph

  std::cout << "============================\n";
  std::cout << "============================\n";
  g1->debugNodes();
  g1->debugArcs();
  delete g1;
}
