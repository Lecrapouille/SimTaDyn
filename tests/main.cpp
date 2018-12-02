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

// --- Utils ----------------------------------------------------------
#include "StrongTypeTests.hpp"
#include "LoggerTests.hpp"
#include "PathTests.hpp"
#include "FileTests.hpp"
#include "TerminalColorTests.hpp"

// --- Maths ----------------------------------------------------------
#include "VectorTests.hpp"
#include "MatrixTests.hpp"
#include "TransformationTests.hpp"
#include "FilteringTests.hpp"
#include "BoundingBoxTests.hpp"

// --- Containers -----------------------------------------------------
#include "SetTests.hpp"
#include "CollectionTests.hpp"

// --- Graph ----------------------------------------------------------
/*#include "BasicNodeTests.hpp"
#include "BasicArcTests.hpp"
#include "BasicGraphTests.hpp"
#include "GraphAlgoTests.hpp"*/

// --- OpenGL ---------------------------------------------------------
#include "ColorTests.hpp"
#include "GLVAOTests.hpp"
#include "GLObjectTests.hpp"
#include "GLVAOTests.hpp"
#include "GLVBOTests.hpp"
#include "GLShadersTests.hpp"
#include "GLProgramTests.hpp"

// --- Loader ---------------------------------------------------------
//#include "ResourcesTests.hpp"
#include "SimTaDynFileLoaderTests.hpp"
// --- Core ---------------------------------------------------------
#include "ClassicSpreadSheetTests.hpp"

// --- CPPUnit --------------------------------------------------------
#include <cppunit/ui/text/TestRunner.h>


//--------------------------------------------------------------------------
static void testUtils(CppUnit::TextUi::TestRunner& runner)
{
  CppUnit::TestSuite* suite;

  suite = new CppUnit::TestSuite("StrongTypeTests");
  suite->addTest(new CppUnit::TestCaller<StrongTypeTests>("StrongType tests", &StrongTypeTests::testEquality));
  runner.addTest(suite);

  suite = new CppUnit::TestSuite("LoggerTests");
  suite->addTest(new CppUnit::TestCaller<LoggerTests>("Logger tests", &LoggerTests::testEmptyLog));
  suite->addTest(new CppUnit::TestCaller<LoggerTests>("Logger tests", &LoggerTests::testBasic));
  suite->addTest(new CppUnit::TestCaller<LoggerTests>("Logger tests", &LoggerTests::testWithConcurrency));
  runner.addTest(suite);

  suite = new CppUnit::TestSuite("PathTests");
  suite->addTest(new CppUnit::TestCaller<PathTests>("Path tests", &PathTests::testEmptyConstructor));
  suite->addTest(new CppUnit::TestCaller<PathTests>("Path tests", &PathTests::testSplitConstructor));
  suite->addTest(new CppUnit::TestCaller<PathTests>("Path tests", &PathTests::testFindAndExpand));
  suite->addTest(new CppUnit::TestCaller<PathTests>("Path tests", &PathTests::testSplitDir));
  runner.addTest(suite);

  suite = new CppUnit::TestSuite("FileTests");
  suite->addTest(new CppUnit::TestCaller<FileTests>("File tests", &FileTests::testfiles));
  runner.addTest(suite);

  suite = new CppUnit::TestSuite("TerminalColorTests");
  suite->addTest(new CppUnit::TestCaller<TerminalColorTests>("TerminalColor tests", &TerminalColorTests::testcolors));
  runner.addTest(suite);
}

//--------------------------------------------------------------------------
static void testMath(CppUnit::TextUi::TestRunner& runner)
{
  CppUnit::TestSuite* suite;

  suite = new CppUnit::TestSuite("VectorTests");
  suite->addTest(new CppUnit::TestCaller<VectorTests>("testCreator", &VectorTests::testCreator));
  suite->addTest(new CppUnit::TestCaller<VectorTests>("testSwap", &VectorTests::testSwap));
  suite->addTest(new CppUnit::TestCaller<VectorTests>("testEquality", &VectorTests::testEquality));
  suite->addTest(new CppUnit::TestCaller<VectorTests>("testArithmetic", &VectorTests::testArithmetic));
  suite->addTest(new CppUnit::TestCaller<VectorTests>("testCopy", &VectorTests::testCopy));
  suite->addTest(new CppUnit::TestCaller<VectorTests>("testOperations", &VectorTests::testOperations));
  runner.addTest(suite);

  suite = new CppUnit::TestSuite("MatrixTests");
  suite->addTest(new CppUnit::TestCaller<MatrixTests>("testCreator", &MatrixTests::testCreator));
  suite->addTest(new CppUnit::TestCaller<MatrixTests>("testSwap", &MatrixTests::testSwap));
  suite->addTest(new CppUnit::TestCaller<MatrixTests>("testEquality", &MatrixTests::testEquality));
  suite->addTest(new CppUnit::TestCaller<MatrixTests>("testArithmetic", &MatrixTests::testArithmetic));
  suite->addTest(new CppUnit::TestCaller<MatrixTests>("testCopy", &MatrixTests::testCopy));
  suite->addTest(new CppUnit::TestCaller<MatrixTests>("testOperations", &MatrixTests::testOperations));
  runner.addTest(suite);

  suite = new CppUnit::TestSuite("TransformationsTests");
  suite->addTest(new CppUnit::TestCaller<TransformationTests>("testMatrixTransform", &TransformationTests::tests));
  suite->addTest(new CppUnit::TestCaller<TransformationTests>("testMovableClass", &TransformationTests::movable));
  runner.addTest(suite);

  suite = new CppUnit::TestSuite("Filtering");
  //suite->addTest(new CppUnit::TestCaller<FilteringTests>("RollingAverage", &FilteringTests::rolling));
  //suite->addTest(new CppUnit::TestCaller<FilteringTests>("LowPassFilter", &FilteringTests::lpf));
  suite->addTest(new CppUnit::TestCaller<FilteringTests>("PolynomialFit", &FilteringTests::polyfit));
  runner.addTest(suite);

  suite = new CppUnit::TestSuite("BoundingBoxTests");
  suite->addTest(new CppUnit::TestCaller<BoundingBoxTests>("testCreator", &BoundingBoxTests::testCreator));
  suite->addTest(new CppUnit::TestCaller<BoundingBoxTests>("testSwap", &BoundingBoxTests::testSwap));
  suite->addTest(new CppUnit::TestCaller<BoundingBoxTests>("testEquality", &BoundingBoxTests::testEquality));
  suite->addTest(new CppUnit::TestCaller<BoundingBoxTests>("testArithmetic", &BoundingBoxTests::testArithmetic));
  suite->addTest(new CppUnit::TestCaller<BoundingBoxTests>("testCopy", &BoundingBoxTests::testCopy));
  suite->addTest(new CppUnit::TestCaller<BoundingBoxTests>("testOperations", &BoundingBoxTests::testOperations));
  runner.addTest(suite);
}

//--------------------------------------------------------------------------
static void testContainer(CppUnit::TextUi::TestRunner& runner)
{
  CppUnit::TestSuite* suite;

  suite = new CppUnit::TestSuite("SetTests");
  suite->addTest(new CppUnit::TestCaller<SetTests>("SetDummy", &SetTests::testDummy));
  suite->addTest(new CppUnit::TestCaller<SetTests>("SetInsert", &SetTests::testInsert));
  suite->addTest(new CppUnit::TestCaller<SetTests>("SetRemove", &SetTests::testRemove));
  suite->addTest(new CppUnit::TestCaller<SetTests>("SetInsertArr", &SetTests::testInsertArr));
  runner.addTest(suite);

  suite = new CppUnit::TestSuite("CollectionTests");
  suite->addTest(new CppUnit::TestCaller<CollectionTests>("CollectionDummy", &CollectionTests::testDummy));
  suite->addTest(new CppUnit::TestCaller<CollectionTests>("CollectionOccupy", &CollectionTests::testOccupy));
  suite->addTest(new CppUnit::TestCaller<CollectionTests>("CollectionAppend", &CollectionTests::testAppend));
  suite->addTest(new CppUnit::TestCaller<CollectionTests>("CollectionRemove", &CollectionTests::testRemove));
  suite->addTest(new CppUnit::TestCaller<CollectionTests>("CollectionInsert", &CollectionTests::testInsert));
  runner.addTest(suite);
}

//--------------------------------------------------------------------------
static void testGraph(CppUnit::TextUi::TestRunner& /*runner*/)
{
  /*CppUnit::TestSuite* suite;

  suite = new CppUnit::TestSuite("BasicNodeTests");
  suite->addTest(new CppUnit::TestCaller<BasicNodeTests>("test", &BasicNodeTests::dummy));
  suite->addTest(new CppUnit::TestCaller<BasicNodeTests>("test", &BasicNodeTests::neighbor));
  runner.addTest(suite);

  suite = new CppUnit::TestSuite("BasicArcTests");
  suite->addTest(new CppUnit::TestCaller<BasicArcTests>("test", &BasicArcTests::test));
  runner.addTest(suite);

  suite = new CppUnit::TestSuite("BasicGraphTests");
  suite->addTest(new CppUnit::TestCaller<BasicGraphTests>("test", &BasicGraphTests::test));
  runner.addTest(suite);

  suite = new CppUnit::TestSuite("GraphAlgoTests");
  suite->addTest(new CppUnit::TestCaller<GraphAlgoTests>("test", &GraphAlgoTests::test));
  runner.addTest(suite);*/
}

//--------------------------------------------------------------------------
static void testOpenGL(CppUnit::TextUi::TestRunner& runner)
{
  CppUnit::TestSuite* suite;

  suite = new CppUnit::TestSuite("ColorTests");
  suite->addTest(new CppUnit::TestCaller<ColorTests>("testCreator", &ColorTests::testCreator));
  suite->addTest(new CppUnit::TestCaller<ColorTests>("testSwap", &ColorTests::testSwap));
  suite->addTest(new CppUnit::TestCaller<ColorTests>("testEquality", &ColorTests::testEquality));
  suite->addTest(new CppUnit::TestCaller<ColorTests>("testArithmetic", &ColorTests::testArithmetic));
  suite->addTest(new CppUnit::TestCaller<ColorTests>("testCopy", &ColorTests::testCopy));
  suite->addTest(new CppUnit::TestCaller<ColorTests>("testOperations", &ColorTests::testOperations));
  runner.addTest(suite);

  suite = new CppUnit::TestSuite("GLObjectTests");
  suite->addTest(new CppUnit::TestCaller<GLObjectTests>("tests", &GLObjectTests::tests));
  runner.addTest(suite);

  suite = new CppUnit::TestSuite("GLVAOTests");
  suite->addTest(new CppUnit::TestCaller<GLVAOTests>("tests", &GLVAOTests::tests));
  runner.addTest(suite);

  suite = new CppUnit::TestSuite("GLVBOTests");
  suite->addTest(new CppUnit::TestCaller<GLVBOTests>("tests", &GLVBOTests::tests));
  runner.addTest(suite);

  suite = new CppUnit::TestSuite("GLShadersTests");
  suite->addTest(new CppUnit::TestCaller<GLShadersTests>("tests", &GLShadersTests::tests));
  runner.addTest(suite);

  suite = new CppUnit::TestSuite("GLProgramTests");
  suite->addTest(new CppUnit::TestCaller<GLProgramTests>("tests", &GLProgramTests::tests));
  runner.addTest(suite);
}

//--------------------------------------------------------------------------
static void testLoader(CppUnit::TextUi::TestRunner& runner)
{
  /*CppUnit::TestSuite* suite;

  suite = new CppUnit::TestSuite("resourcesManagerTests");
  suite->addTest(new CppUnit::TestCaller<ResourcesTests>("Resources", &ResourcesTests::testsResources));
  suite->addTest(new CppUnit::TestCaller<ResourcesTests>("RessourceManager", &ResourcesTests::testsResourceManager));
  suite->addTest(new CppUnit::TestCaller<ResourcesTests>("LoaderManager", &ResourcesTests::testsLoaderManager));
  runner.addTest(suite);

  suite = new CppUnit::TestSuite("LoaderTests");
  suite->addTest(new CppUnit::TestCaller<LoaderTests>("Loader", &LoaderTests::testSimTaDyn));
  runner.addTest(suite);*/
}

//--------------------------------------------------------------------------
static void testCore(CppUnit::TextUi::TestRunner& runner)
{
  CppUnit::TestSuite* suite;

  suite = new CppUnit::TestSuite("ClassicSpreadSheetTests");
  suite->addTest(new CppUnit::TestCaller<ClassicSpreadSheetTests>("Spreadsheet 1", &ClassicSpreadSheetTests::testInput1));
  /*suite->addTest(new CppUnit::TestCaller<ClassicSpreadSheetTests>("Spreadsheet 2", &ClassicSpreadSheetTests::testInput2)); // FIXME bug libc++abi.dylib: Pure virtual function called!
  suite->addTest(new CppUnit::TestCaller<ClassicSpreadSheetTests>("Spreadsheet 3", &ClassicSpreadSheetTests::testInput3));
  suite->addTest(new CppUnit::TestCaller<ClassicSpreadSheetTests>("Spreadsheet 4", &ClassicSpreadSheetTests::testInput4));
  suite->addTest(new CppUnit::TestCaller<ClassicSpreadSheetTests>("Spreadsheet 5", &ClassicSpreadSheetTests::testInput5));*/
  runner.addTest(suite);
}

//--------------------------------------------------------------------------
static bool run_tests(bool const has_xdisplay)
{
  CppUnit::TextUi::TestRunner runner;

  // Call it before Logger constructor
  if (!File::mkdir(config::tmp_path))
    {
      std::cerr << "Failed creating the temporary directory '"
                << config::tmp_path << "'" << std::endl;
    }

  testUtils(runner);
  testMath(runner);
  testContainer(runner);
  testGraph(runner);
  // Travis-CI does not support export display
  if (has_xdisplay) testOpenGL(runner);
  testLoader(runner);
  testCore(runner);

  return runner.run();
}

#include <getopt.h>

int main(int argc, char *argv[])
{
  const char *c_short_options = "a";
  bool has_xdisplay = true;
  int c;
  opterr = 0;

  // TODO: make this code better. Only used for disabling Travis
  while ((c = getopt(argc, argv, c_short_options)) != -1)
    {
      switch (c)
        {
        case 'a':
          has_xdisplay = false;
          break;
        case '?':
          if (optopt == 'c')
            {
              std::cerr << "Option -" << optopt << " requires an argument" << std::endl;
            }
          else if (isprint(optopt))
            {
              std::cerr << "Unknown option -" << (char) optopt << std::endl;
            }
          else
            {
              std::cerr << "Unknown option character " << optopt << std::endl;
            }
          return 1;
        default:
          abort();
        }
    }

  for (int index = optind; index < argc; ++index)
    {
      std::cerr << "Non-option argument " << argv[index] << std::endl;
    }

  bool wasSucessful = run_tests(has_xdisplay);
  if (wasSucessful)
    {
      std::cout << "*** Congratulation: all tests passed ***" << std::endl;
    }
  else
    {
      std::cout << "*** Sorry but not all tests passed ***" << std::endl;
    }
  return wasSucessful ? 0 : 1;
}
