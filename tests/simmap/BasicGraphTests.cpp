#include "BasicGraphTests.hpp"

// Register the test suite
CPPUNIT_TEST_SUITE_REGISTRATION(BasicGraphTests);

//--------------------------------------------------------------------------
void BasicGraphTests::setUp()
{
  BasicNode::reset();
  BasicArc::reset();
}

//--------------------------------------------------------------------------
void BasicGraphTests::tearDown()
{
  BasicNode::reset();
  BasicArc::reset();
}

//--------------------------------------------------------------------------
void BasicGraphTests::test()
{
  // Create a (un)directed graph with preallocated 2^2 elements (nodes
  // and arcs).
  const bool oriented = false;
  BasicGraph<BasicNode, BasicArc> g(2U, 2U, oriented);
  BasicNode n6(6U);

  // Check empty graph with pre-allocated blocks.
  CPPUNIT_ASSERT_EQUAL(true, g.empty());
  CPPUNIT_ASSERT_EQUAL(256U, g.m_nodes.remaining());
  CPPUNIT_ASSERT_EQUAL(0U, g.m_nodes.used());
  CPPUNIT_ASSERT_EQUAL(true, g.m_nodes.blocks() >= 1U);
  CPPUNIT_ASSERT_EQUAL(256U, g.m_arcs.remaining());
  CPPUNIT_ASSERT_EQUAL(0U, g.m_arcs.used());
  CPPUNIT_ASSERT_EQUAL(true, g.m_arcs.blocks() >= 1U);
  CPPUNIT_ASSERT_EQUAL(0U, g.howManyNodes());
  CPPUNIT_ASSERT_EQUAL(0U, g.howManyArcs());

  CPPUNIT_ASSERT_EQUAL(false, g.m_nodes.occupied(0U));
  CPPUNIT_ASSERT_EQUAL(false, g.m_nodes.occupied(1U));
  CPPUNIT_ASSERT_EQUAL(false, g.m_nodes.occupied(2U));
  CPPUNIT_ASSERT_EQUAL(false, g.m_nodes.occupied(3U));

  CPPUNIT_ASSERT_EQUAL(false, g.hasNode(0U));
  CPPUNIT_ASSERT_EQUAL(false, g.hasNode(1U));
  CPPUNIT_ASSERT_EQUAL(false, g.hasNode(2U));
  CPPUNIT_ASSERT_EQUAL(false, g.hasNode(3U));
  CPPUNIT_ASSERT_EQUAL(false, g.hasNode(4U));

  // Check unique node/arcs identifiers are auto incrementing
  for (uint32_t i = 0; i < 4; ++i)
    {
      CPPUNIT_ASSERT_EQUAL(i, g.m_nodes.m_blocks[0]->m_block[i].id());
      CPPUNIT_ASSERT_EQUAL(i, g.m_arcs.m_blocks[0]->m_block[i].id());
    }

  // Add a node. Check graph is no longer empty and contains the
  // correct identifiers.
  g.addNode(0U);
  CPPUNIT_ASSERT_EQUAL(false, g.empty());

  g.addNode(1U);
  g.addNode(2U);
  g.addNode(3U);

  CPPUNIT_ASSERT_EQUAL(4U, g.howManyNodes());
  CPPUNIT_ASSERT_EQUAL(0U, g.howManyArcs());

  CPPUNIT_ASSERT_EQUAL(true, g.m_nodes.occupied(0U));
  CPPUNIT_ASSERT_EQUAL(true, g.m_nodes.occupied(1U));
  CPPUNIT_ASSERT_EQUAL(true, g.m_nodes.occupied(2U));
  CPPUNIT_ASSERT_EQUAL(true, g.m_nodes.occupied(3U));
  CPPUNIT_ASSERT_EQUAL(false, g.m_nodes.occupied(4U));

  CPPUNIT_ASSERT_EQUAL(true, g.hasNode(0U));
  CPPUNIT_ASSERT_EQUAL(true, g.hasNode(1U));
  CPPUNIT_ASSERT_EQUAL(true, g.hasNode(2U));
  CPPUNIT_ASSERT_EQUAL(true, g.hasNode(3U));
  CPPUNIT_ASSERT_EQUAL(false, g.hasNode(4U));
  CPPUNIT_ASSERT_EQUAL(false, g.hasNode(6U));
  CPPUNIT_ASSERT_EQUAL(false, g.hasNode(n6));

  CPPUNIT_ASSERT_EQUAL(0U, g.getNode(0U).id());
  CPPUNIT_ASSERT_EQUAL(1U, g.getNode(1U).id());
  CPPUNIT_ASSERT_EQUAL(2U, g.getNode(2U).id());
  CPPUNIT_ASSERT_EQUAL(3U, g.getNode(3U).id());

  // Add a node outside the scope of the graph. Check a new node has
  // been added.
  uint32_t blocks = g.m_nodes.blocks();
  const Key k = (1U << simtadyn::graph_container_nb_elements) + 1U;
  CPPUNIT_ASSERT_EQUAL(true, g.m_nodes.outofbound(k));
  CPPUNIT_ASSERT_EQUAL(false, g.hasNode(k));
  g.addNode(k);
  CPPUNIT_ASSERT_EQUAL(5U, g.howManyNodes());
  CPPUNIT_ASSERT_EQUAL(0U, g.howManyArcs());
  CPPUNIT_ASSERT_EQUAL(false, g.m_nodes.outofbound(k));
  CPPUNIT_ASSERT_EQUAL(true, g.m_nodes.occupied(k));
  CPPUNIT_ASSERT_EQUAL(true, g.hasNode(k));
  CPPUNIT_ASSERT_EQUAL(true, g.m_nodes.blocks() > blocks);

  // Remove node
  g.removeNode(k);
  CPPUNIT_ASSERT_EQUAL(4U, g.howManyNodes());
  CPPUNIT_ASSERT_EQUAL(0U, g.howManyArcs());
  CPPUNIT_ASSERT_EQUAL(false, g.m_nodes.outofbound(k));
  CPPUNIT_ASSERT_EQUAL(false, g.hasNode(k));
  CPPUNIT_ASSERT_EQUAL(true, g.m_nodes.blocks() > blocks);

  // Garbage memory
  g.garbage();
  CPPUNIT_ASSERT_EQUAL(4U, g.howManyNodes());
  CPPUNIT_ASSERT_EQUAL(0U, g.howManyArcs());
  CPPUNIT_ASSERT_EQUAL(true, g.m_nodes.outofbound(k));
  CPPUNIT_ASSERT_EQUAL(false, g.hasNode(k));
  CPPUNIT_ASSERT_EQUAL(true, g.m_nodes.blocks() == blocks);

  // Add a new arc. Check if it is well formed.
  g.addArc(1U, 3U); // Arc0
  g.debugNodes();
  g.debugArcs();
  CPPUNIT_ASSERT_EQUAL(4U, g.howManyNodes());
  CPPUNIT_ASSERT_EQUAL(1U, g.howManyArcs());
  CPPUNIT_ASSERT_EQUAL(true, g.hasArc(0U));
  CPPUNIT_ASSERT_EQUAL(false, g.hasArc(1U));
  CPPUNIT_ASSERT_EQUAL(false, g.hasArc(10000000U));
  CPPUNIT_ASSERT_EQUAL(0U, g.getArc(0U).id());
  CPPUNIT_ASSERT_EQUAL(1U, g.getArc(0U).from().id());
  CPPUNIT_ASSERT_EQUAL(3U, g.getArc(0U).to().id());
  CPPUNIT_ASSERT_EQUAL(0U, g.getNode(1U).nthNeighbor(0).id()); // Arc0
  CPPUNIT_ASSERT_EQUAL(1U, g.getNode(1U).nthNeighbor(0).from().id());
  CPPUNIT_ASSERT_EQUAL(3U, g.getNode(1U).nthNeighbor(0).to().id());
  CPPUNIT_ASSERT_EQUAL(0U, g.getNode(3U).nthNeighbor(0).id()); // Arc0
  CPPUNIT_ASSERT_EQUAL(1U, g.getNode(3U).nthNeighbor(0).from().id());
  CPPUNIT_ASSERT_EQUAL(3U, g.getNode(3U).nthNeighbor(0).to().id());

  // Add a new arc. Check if it is well formed.
  BasicNode n42(42);
  BasicNode n43(43);
  g.addArc(n42, n43); // Arc1
  g.debugNodes();
  g.debugArcs();
  CPPUNIT_ASSERT_EQUAL(6U, g.howManyNodes());
  CPPUNIT_ASSERT_EQUAL(2U, g.howManyArcs());
  CPPUNIT_ASSERT_EQUAL(true, g.hasArc(0U));
  CPPUNIT_ASSERT_EQUAL(true, g.hasArc(1U));
  CPPUNIT_ASSERT_EQUAL(false, g.hasArc(2U));
  CPPUNIT_ASSERT_EQUAL(false, g.hasArc(10000000U));
  CPPUNIT_ASSERT_EQUAL(0U, g.getArc(0U).id());
  CPPUNIT_ASSERT_EQUAL(1U, g.getArc(0U).from().id());
  CPPUNIT_ASSERT_EQUAL(3U, g.getArc(0U).to().id());
  CPPUNIT_ASSERT_EQUAL(0U, g.getNode(1U).nthNeighbor(0).id()); // Arc0
  CPPUNIT_ASSERT_EQUAL(1U, g.getNode(1U).nthNeighbor(0).from().id());
  CPPUNIT_ASSERT_EQUAL(3U, g.getNode(1U).nthNeighbor(0).to().id());
  CPPUNIT_ASSERT_EQUAL(0U, g.getNode(3U).nthNeighbor(0).id()); // Arc0
  CPPUNIT_ASSERT_EQUAL(1U, g.getNode(3U).nthNeighbor(0).from().id());
  CPPUNIT_ASSERT_EQUAL(3U, g.getNode(3U).nthNeighbor(0).to().id());
  CPPUNIT_ASSERT_EQUAL(1U, g.getArc(1U).id());
  CPPUNIT_ASSERT_EQUAL(42U, g.getArc(1U).from().id());
  CPPUNIT_ASSERT_EQUAL(43U, g.getArc(1U).to().id());
  CPPUNIT_ASSERT_EQUAL(1U, g.getNode(42U).nthNeighbor(0).id()); // Arc1
  CPPUNIT_ASSERT_EQUAL(42U, g.getNode(42U).nthNeighbor(0).from().id());
  CPPUNIT_ASSERT_EQUAL(43U, g.getNode(42U).nthNeighbor(0).to().id());
  CPPUNIT_ASSERT_EQUAL(1U, g.getNode(43U).nthNeighbor(0).id()); // Arc1
  CPPUNIT_ASSERT_EQUAL(42U, g.getNode(43U).nthNeighbor(0).from().id());
  CPPUNIT_ASSERT_EQUAL(43U, g.getNode(43U).nthNeighbor(0).to().id());

  // Add a new arc. Check if it is well formed.
  g.addArc(1U, 43U); // Arc2
  CPPUNIT_ASSERT_EQUAL(6U, g.howManyNodes());
  CPPUNIT_ASSERT_EQUAL(3U, g.howManyArcs());
  CPPUNIT_ASSERT_EQUAL(true, g.hasArc(0U));
  CPPUNIT_ASSERT_EQUAL(true, g.hasArc(1U));
  CPPUNIT_ASSERT_EQUAL(true, g.hasArc(2U));
  CPPUNIT_ASSERT_EQUAL(2U, g.getArc(2U).id());
  CPPUNIT_ASSERT_EQUAL(1U, g.getArc(2U).from().id());
  CPPUNIT_ASSERT_EQUAL(43U, g.getArc(2U).to().id());
  CPPUNIT_ASSERT_EQUAL(2U, g.getNode(1U).degree());
  CPPUNIT_ASSERT_EQUAL(0U, g.getNode(1U).nthNeighbor(0).id()); // Arc0
  CPPUNIT_ASSERT_EQUAL(1U, g.getNode(1U).nthNeighbor(0).from().id());
  CPPUNIT_ASSERT_EQUAL(3U, g.getNode(1U).nthNeighbor(0).to().id());
  CPPUNIT_ASSERT_EQUAL(0U, g.getNode(3U).nthNeighbor(0).id()); // Arc0
  CPPUNIT_ASSERT_EQUAL(1U, g.getNode(3U).nthNeighbor(0).from().id());
  CPPUNIT_ASSERT_EQUAL(3U, g.getNode(3U).nthNeighbor(0).to().id());
  CPPUNIT_ASSERT_EQUAL(2U, g.getNode(1U).nthNeighbor(1).id()); // Arc2
  CPPUNIT_ASSERT_EQUAL(1U, g.getNode(1U).nthNeighbor(1).from().id());
  CPPUNIT_ASSERT_EQUAL(43U, g.getNode(1U).nthNeighbor(1).to().id());
  CPPUNIT_ASSERT_EQUAL(2U, g.getNode(1U).nthNeighbor(1).id()); // Arc2
  CPPUNIT_ASSERT_EQUAL(1U, g.getNode(1U).nthNeighbor(1).from().id());
  CPPUNIT_ASSERT_EQUAL(43U, g.getNode(1U).nthNeighbor(1).to().id());


  // Check if auto-counters for nodes/arcs unique identifiers start to 0
  // for a new graph and identifiers are auto incrementing.
  BasicNode::reset();
  BasicArc::reset();
  BasicGraph<BasicNode, BasicArc> g2(2U, 2U, false);
  for (uint32_t i = 0; i < 4; ++i)
    {
      CPPUNIT_ASSERT_EQUAL(i, g2.m_nodes.m_blocks[0]->m_block[i].id());
      CPPUNIT_ASSERT_EQUAL(i, g2.m_arcs.m_blocks[0]->m_block[i].id());
    }
  g.addNode(10U);
  CPPUNIT_ASSERT_EQUAL(7U, g.howManyNodes());
  CPPUNIT_ASSERT_EQUAL(true, g.hasNode(10U));
  CPPUNIT_ASSERT_EQUAL(10U, g.getNode(10U).id());

  // Check if graph has arcs
  CPPUNIT_ASSERT_EQUAL(true, g.hasArc(n42, n43)); // Arc1
  CPPUNIT_ASSERT_EQUAL(true, g.hasArc(42, 43));
  CPPUNIT_ASSERT_EQUAL(true, g.hasArc(1, 3)); // Arc0
  CPPUNIT_ASSERT_EQUAL(false, g.hasArc(n6, n6));
  CPPUNIT_ASSERT_EQUAL(false, g.hasArc(n42, n6));

  // Check access to arcs
  CPPUNIT_ASSERT_EQUAL(0U, g.getArc(1, 3)->id());
  CPPUNIT_ASSERT_EQUAL(1U, g.getArc(n42, n43)->id());
  CPPUNIT_ASSERT_EQUAL(true, nullptr == g.getArc(n42, n6));
  CPPUNIT_ASSERT_EQUAL(true, nullptr == g.getArc(n6, n42));

  // Neighbor of nodes
  CPPUNIT_ASSERT_EQUAL(true, nullptr == g.neighbors(55));
  CPPUNIT_ASSERT_EQUAL(0U, g.neighbors(1)->at(0)->id()); //Arc0
  CPPUNIT_ASSERT_EQUAL(2U, g.neighbors(1)->at(1)->id()); //Arc2

  // Check iterators

  // Mark visited

  // Remove arc. Check if element have been removed.
  CPPUNIT_ASSERT_EQUAL(0U, g.getArc(0U).id()); // Arc0
  CPPUNIT_ASSERT_EQUAL(1U, g.getArc(1U).id()); // Arc1
  g.removeArc(0);
  CPPUNIT_ASSERT_EQUAL(2U, g.howManyArcs());
  CPPUNIT_ASSERT_THROW(g.getArc(0U).id(), std::out_of_range); // Arc0
  CPPUNIT_ASSERT_EQUAL(1U, g.getArc(1U).id()); // Arc1
  CPPUNIT_ASSERT_EQUAL(2U, g.getArc(2U).id()); // Arc2
  g.removeArc(1);
  CPPUNIT_ASSERT_EQUAL(1U, g.howManyArcs());
  CPPUNIT_ASSERT_THROW(g.getArc(0U).id(), std::out_of_range); // Arc0
  CPPUNIT_ASSERT_THROW(g.getArc(1U).id(), std::out_of_range); // Arc1
  CPPUNIT_ASSERT_EQUAL(2U, g.getArc(2U).id()); // Arc2

  g.reset();
  CPPUNIT_ASSERT_EQUAL(true, g.empty());
  CPPUNIT_ASSERT_EQUAL(1U, g.m_nodes.blocks());
  CPPUNIT_ASSERT_EQUAL(1U << simtadyn::graph_container_nb_elements, g.m_nodes.remaining());
  CPPUNIT_ASSERT_EQUAL(0U, g.m_nodes.used());
  CPPUNIT_ASSERT_EQUAL(0U, g.howManyNodes());
  CPPUNIT_ASSERT_THROW(g.getArc(2U).id(), std::out_of_range); // Arc2
}
