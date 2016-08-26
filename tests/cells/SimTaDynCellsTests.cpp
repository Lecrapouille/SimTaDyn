#include "SimTaDynCellsTests.hpp"

// Register the test suite
CPPUNIT_TEST_SUITE_REGISTRATION(SimTaDynCellsTests);

static SimTaDynCell* c1;
static SimTaDynCell* c2;
static SimTaDynCell* c3;

//--------------------------------------------------------------------------
void SimTaDynCellsTests::setUp()
{
  c1 = new SimTaDynCell();
  c2 = new SimTaDynCell("c2", "1 1 + .", NULL);
  c3 = new SimTaDynCell();
}

//--------------------------------------------------------------------------
void SimTaDynCellsTests::tearDown()
{
  delete c1;
  delete c2;
  delete c3;
}

//--------------------------------------------------------------------------
void SimTaDynCellsTests::testCreator()
{
  CPPUNIT_ASSERT_EQUAL(3U, SimTaDynCell::howMany());
  CPPUNIT_ASSERT_EQUAL(0U, c1->privateId());
  CPPUNIT_ASSERT_EQUAL(1U, c2->privateId());
  CPPUNIT_ASSERT_EQUAL(2U, c3->privateId());

  CPPUNIT_ASSERT_EQUAL(0, c1->name.compare(std::string("Cell_0")));
  CPPUNIT_ASSERT_EQUAL(0, c2->name.compare(std::string("c2")));
  CPPUNIT_ASSERT_EQUAL(0, c3->name.compare(std::string("Cell_2")));
}

//--------------------------------------------------------------------------
void SimTaDynCellsTests::testID()
{
  CPPUNIT_ASSERT_EQUAL(true, (c1 == c1));
  CPPUNIT_ASSERT_EQUAL(true, (c1 != c2));
  CPPUNIT_ASSERT_EQUAL(false, (c1 == c2));

  CPPUNIT_ASSERT_EQUAL(true, (c1 <= c2));
  CPPUNIT_ASSERT_EQUAL(true, (c2 <= c3));

  CPPUNIT_ASSERT_EQUAL(true, (*c1 == 0U));
  CPPUNIT_ASSERT_EQUAL(true, (*c2 == 1U));
  CPPUNIT_ASSERT_EQUAL(true, (*c3 == 2U));
}

//--------------------------------------------------------------------------
void SimTaDynCellsTests::testNeighbors()
{
  CPPUNIT_ASSERT_EQUAL(0U, c1->nbExplicitNeighbors());
  c1->addExplicitNeighbor(c3);
  c1->addExplicitNeighbor(c2);
  CPPUNIT_ASSERT_EQUAL(2U, c1->nbExplicitNeighbors());
  CPPUNIT_ASSERT_EQUAL(true, c1->IsExplicitNeighborOf(c2));
  CPPUNIT_ASSERT_EQUAL(true, c1->IsExplicitNeighborOf(c3));
  CPPUNIT_ASSERT_EQUAL(false, c1->IsExplicitNeighborOf(c1));

  /* no double allowed */
  c1->addExplicitNeighbor(c3);
  CPPUNIT_ASSERT_EQUAL(2U, c1->nbExplicitNeighbors());
}
