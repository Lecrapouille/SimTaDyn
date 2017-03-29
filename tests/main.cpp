#include "FileTests.hpp"
#include "VectorTests.hpp"
//#include "BoundingBoxTests.hpp"
#include "ColorTests.hpp"
#include "SetTests.hpp"
/*#include "GraphMemoryTests.hpp"
  #include "BasicGraphTests.hpp"*/

#include <cppunit/ui/text/TestRunner.h>

int main(void)
{
  CppUnit::TestSuite* suite1 = new CppUnit::TestSuite("FileTests");
  suite1->addTest(new CppUnit::TestCaller<FileTests>("File tests", &FileTests::tests));

  CppUnit::TestSuite* suite2 = new CppUnit::TestSuite("SetTests");
  suite2->addTest(new CppUnit::TestCaller<SetTests>("SetDummy", &SetTests::testDummy));
  suite2->addTest(new CppUnit::TestCaller<SetTests>("SetInsert", &SetTests::testInsert));
  suite2->addTest(new CppUnit::TestCaller<SetTests>("SetRemove", &SetTests::testRemove));

  CppUnit::TestSuite* suite3 = new CppUnit::TestSuite("VectorTests");
  suite3->addTest(new CppUnit::TestCaller<VectorTests>("testCreator", &VectorTests::testCreator));
  suite3->addTest(new CppUnit::TestCaller<VectorTests>("testSwap", &VectorTests::testSwap));
  suite3->addTest(new CppUnit::TestCaller<VectorTests>("testEquality", &VectorTests::testEquality));
  suite3->addTest(new CppUnit::TestCaller<VectorTests>("testArithmetic", &VectorTests::testArithmetic));
  suite3->addTest(new CppUnit::TestCaller<VectorTests>("testCopy", &VectorTests::testCopy));
  suite3->addTest(new CppUnit::TestCaller<VectorTests>("testOperations", &VectorTests::testOperations));

  /*CppUnit::TestSuite* suite4 = new CppUnit::TestSuite("BoundingBoxTests");
  suite4->addTest(new CppUnit::TestCaller<BoundingBoxTests>("testCreator", &BoundingBoxTests::testCreator));
  suite4->addTest(new CppUnit::TestCaller<BoundingBoxTests>("testSwap", &BoundingBoxTests::testSwap));
  suite4->addTest(new CppUnit::TestCaller<BoundingBoxTests>("testEquality", &BoundingBoxTests::testEquality));
  suite4->addTest(new CppUnit::TestCaller<BoundingBoxTests>("testArithmetic", &BoundingBoxTests::testArithmetic));
  suite4->addTest(new CppUnit::TestCaller<BoundingBoxTests>("testCopy", &BoundingBoxTests::testCopy));
  suite4->addTest(new CppUnit::TestCaller<BoundingBoxTests>("testOperations", &BoundingBoxTests::testOperations));
  */
  CppUnit::TestSuite* suite5 = new CppUnit::TestSuite("ColorTests");
  suite5->addTest(new CppUnit::TestCaller<ColorTests>("testCreator", &ColorTests::testCreator));
  suite5->addTest(new CppUnit::TestCaller<ColorTests>("testSwap", &ColorTests::testSwap));
  suite5->addTest(new CppUnit::TestCaller<ColorTests>("testEquality", &ColorTests::testEquality));
  suite5->addTest(new CppUnit::TestCaller<ColorTests>("testArithmetic", &ColorTests::testArithmetic));
  suite5->addTest(new CppUnit::TestCaller<ColorTests>("testCopy", &ColorTests::testCopy));
  suite5->addTest(new CppUnit::TestCaller<ColorTests>("testOperations", &ColorTests::testOperations));

  /*
  CppUnit::TestSuite* suite4 = new CppUnit::TestSuite("GraphMemoryTests");
  suite4->addTest(new CppUnit::TestCaller<GraphMemoryTests>("test", &GraphMemoryTests::test));
  CppUnit::TestSuite* suite5 = new CppUnit::TestSuite("BasicGraphTests");
  suite5->addTest(new CppUnit::TestCaller<BasicGraphTests>("test", &BasicGraphTests::test));*/

  CppUnit::TextUi::TestRunner runner;
  runner.addTest(suite1);
  runner.addTest(suite2);
  runner.addTest(suite3);
  //runner.addTest(suite4);
  runner.addTest(suite5);
  runner.run();

  return 0;
}
