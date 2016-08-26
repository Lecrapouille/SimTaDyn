#include <cppunit/ui/text/TestRunner.h>
#include "VectorTests.hpp"
#include "BoundingBoxTests.hpp"
#include "SimTaDynCellsTests.hpp"

int main(void)
{
  CppUnit::TestSuite* suite1 = new CppUnit::TestSuite("VectorTests");
  suite1->addTest(new CppUnit::TestCaller<VectorTests>("testCreator", &VectorTests::testCreator));
  suite1->addTest(new CppUnit::TestCaller<VectorTests>("testSwap", &VectorTests::testSwap));
  suite1->addTest(new CppUnit::TestCaller<VectorTests>("testEquality", &VectorTests::testEquality));
  suite1->addTest(new CppUnit::TestCaller<VectorTests>("testArithmetic", &VectorTests::testArithmetic));
  suite1->addTest(new CppUnit::TestCaller<VectorTests>("testCopy", &VectorTests::testCopy));
  suite1->addTest(new CppUnit::TestCaller<VectorTests>("testOperations", &VectorTests::testOperations));

  CppUnit::TestSuite* suite2 = new CppUnit::TestSuite("BoundingBoxTests");
  suite2->addTest(new CppUnit::TestCaller<BoundingBoxTests>("testCreator", &BoundingBoxTests::testCreator));
  suite2->addTest(new CppUnit::TestCaller<BoundingBoxTests>("testSwap", &BoundingBoxTests::testSwap));
  suite2->addTest(new CppUnit::TestCaller<BoundingBoxTests>("testEquality", &BoundingBoxTests::testEquality));
  suite2->addTest(new CppUnit::TestCaller<BoundingBoxTests>("testArithmetic", &BoundingBoxTests::testArithmetic));
  suite2->addTest(new CppUnit::TestCaller<BoundingBoxTests>("testCopy", &BoundingBoxTests::testCopy));
  suite2->addTest(new CppUnit::TestCaller<BoundingBoxTests>("testOperations", &BoundingBoxTests::testOperations));

  CppUnit::TestSuite* suite3 = new CppUnit::TestSuite("SimTaDynCellsTests");
  suite2->addTest(new CppUnit::TestCaller<SimTaDynCellsTests>("testCreator", &SimTaDynCellsTests::testCreator));
  suite2->addTest(new CppUnit::TestCaller<SimTaDynCellsTests>("testID", &SimTaDynCellsTests::testID));
  suite2->addTest(new CppUnit::TestCaller<SimTaDynCellsTests>("testNeighbors", &SimTaDynCellsTests::testNeighbors));

  CppUnit::TextUi::TestRunner runner;
  runner.addTest(suite1);
  runner.addTest(suite2);
  runner.addTest(suite3);
  runner.run();

  return 0;
}
