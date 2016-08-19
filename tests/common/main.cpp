#include <cppunit/ui/text/TestRunner.h>
#include "VectorTests.hpp"

int main(void)
{
  CppUnit::TestResult result;
  CppUnit::TestSuite* suite = new CppUnit::TestSuite("VectorTests");
  suite->addTest(new CppUnit::TestCaller<VectorTests>("testCreator", &VectorTests::testCreator));
  suite->addTest(new CppUnit::TestCaller<VectorTests>("testSwap", &VectorTests::testSwap));
  suite->addTest(new CppUnit::TestCaller<VectorTests>("testEquality", &VectorTests::testEquality));
  suite->addTest(new CppUnit::TestCaller<VectorTests>("testArithmetic", &VectorTests::testArithmetic));
  suite->addTest(new CppUnit::TestCaller<VectorTests>("testCopy", &VectorTests::testCopy));
  suite->addTest(new CppUnit::TestCaller<VectorTests>("testOperations", &VectorTests::testOperations));

  CppUnit::TextUi::TestRunner runner;
  runner.addTest(suite);
  runner.run();

  return 0;
}
