#include "FileTests.hpp"
#include "VectorTests.hpp"
//#include "BoundingBoxTests.hpp"
#include "ColorTests.hpp"
#include "SetTests.hpp"
#include "CollectionTests.hpp"
#include "BasicNodeTests.hpp"
#include "BasicArcTests.hpp"
#include "BasicGraphTests.hpp"
#include "GraphAlgoTests.hpp"
#include "GLBufferTests.hpp"
#include "OpenGLTests.hpp"
#include <cppunit/ui/text/TestRunner.h>

int main(void)
{
  CppUnit::TestSuite* suite1 = new CppUnit::TestSuite("FileTests");
  suite1->addTest(new CppUnit::TestCaller<FileTests>("File tests", &FileTests::tests));

  CppUnit::TestSuite* suite2 = new CppUnit::TestSuite("SetTests");
  suite2->addTest(new CppUnit::TestCaller<SetTests>("SetDummy", &SetTests::testDummy));
  suite2->addTest(new CppUnit::TestCaller<SetTests>("SetInsert", &SetTests::testInsert));
  suite2->addTest(new CppUnit::TestCaller<SetTests>("SetRemove", &SetTests::testRemove));
  suite2->addTest(new CppUnit::TestCaller<SetTests>("SetInsertArr", &SetTests::testInsertArr));

  CppUnit::TestSuite* suite6 = new CppUnit::TestSuite("CollectionTests");
  suite6->addTest(new CppUnit::TestCaller<CollectionTests>("CollectionDummy", &CollectionTests::testDummy));
  suite6->addTest(new CppUnit::TestCaller<CollectionTests>("CollectionOccupy", &CollectionTests::testOccupy));
  suite6->addTest(new CppUnit::TestCaller<CollectionTests>("CollectionAppend", &CollectionTests::testAppend));
  suite6->addTest(new CppUnit::TestCaller<CollectionTests>("CollectionRemove", &CollectionTests::testRemove));
  suite6->addTest(new CppUnit::TestCaller<CollectionTests>("CollectionInsert", &CollectionTests::testInsert));

  CppUnit::TestSuite* suite7 = new CppUnit::TestSuite("GLBufferTests");
  suite7->addTest(new CppUnit::TestCaller<GLBufferTests>("createOpenGLContext", &GLBufferTests::createOpenGLContext));
  suite7->addTest(new CppUnit::TestCaller<GLBufferTests>("GLBufferDummy", &GLBufferTests::testDummy));
  suite7->addTest(new CppUnit::TestCaller<GLBufferTests>("GLBufferPendingData", &GLBufferTests::testPendingData));
  suite7->addTest(new CppUnit::TestCaller<GLBufferTests>("GLBufferInsert", &GLBufferTests::testInsert));
  suite7->addTest(new CppUnit::TestCaller<GLBufferTests>("GLBufferSuppress", &GLBufferTests::testSuppress));
  suite7->addTest(new CppUnit::TestCaller<GLBufferTests>("GLBufferSwap", &GLBufferTests::testSwap));
  suite7->addTest(new CppUnit::TestCaller<GLBufferTests>("deleteOpenGLContext", &GLBufferTests::deleteOpenGLContext));

  CppUnit::TestSuite* suite12 = new CppUnit::TestSuite("OpenGLTests");
  suite12->addTest(new CppUnit::TestCaller<OpenGLTests>("GLObject", &OpenGLTests::testOpenGL));

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

  CppUnit::TestSuite* suite8 = new CppUnit::TestSuite("BasicArcTests");
  suite8->addTest(new CppUnit::TestCaller<BasicArcTests>("test", &BasicArcTests::test));
  CppUnit::TestSuite* suite9 = new CppUnit::TestSuite("BasicNodeTests");
  suite9->addTest(new CppUnit::TestCaller<BasicNodeTests>("test", &BasicNodeTests::dummy));
  suite9->addTest(new CppUnit::TestCaller<BasicNodeTests>("test", &BasicNodeTests::neighbor));
  CppUnit::TestSuite* suite10 = new CppUnit::TestSuite("BasicGraphTests");
  suite10->addTest(new CppUnit::TestCaller<BasicGraphTests>("test", &BasicGraphTests::test));
  CppUnit::TestSuite* suite11 = new CppUnit::TestSuite("GraphAlgoTests");
  suite11->addTest(new CppUnit::TestCaller<GraphAlgoTests>("test", &GraphAlgoTests::test));

  CppUnit::TextUi::TestRunner runner;
  runner.addTest(suite1);
  runner.addTest(suite2);
  runner.addTest(suite3);
  runner.addTest(suite5);
  runner.addTest(suite6);
  runner.addTest(suite7);
  runner.addTest(suite12);
  //runner.addTest(suite4);
  runner.addTest(suite8);
  runner.addTest(suite9);
  runner.addTest(suite10);
  runner.addTest(suite11);
  runner.run();

  return 0;
}
