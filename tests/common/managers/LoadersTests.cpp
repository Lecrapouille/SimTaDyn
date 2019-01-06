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

#include "LoadersTests.hpp"
#include "ClassCounter.tpp"
#include <iostream>
#include <fstream>

static bool loadedA = false;
static bool loadedB = false;
static bool savedA = false;
static bool savedB = false;

class ResourceA
{
public:

  ResourceA() {}
  ~ResourceA() {}
};

class ResourceB
{
public:

  ResourceB() {}
  ~ResourceB() {}
};

using RAPtr = std::shared_ptr<ResourceA>;
using RBPtr = std::shared_ptr<ResourceB>;

class LoaderA: public ILoader<ResourceA>
{
public:

  LoaderA()
    : ILoader<ResourceA>("Loader ResourceA")
  {}

  ~LoaderA() {}

  virtual void loadFromFile(std::string const& /*filename*/, ResourceA& /*object*/) override
  {
    loadedA = true;
    std::cout << "loadFromFile ResourceA" << std::endl;
  }

  virtual void saveToFile(ResourceA const& /*object*/, std::string const& /*filename*/) const override
  {
    savedA = true;
    std::cout << "saveToFile ResourceA" << std::endl;
  }
};

class LoaderB: public ILoader<ResourceB>
{
public:
  LoaderB()
    : ILoader<ResourceB>("Loader ResourceB")
  {}

  ~LoaderB() {}

  virtual void loadFromFile(std::string const& /*filename*/, ResourceB& /*object*/) override
  {
    loadedB = true;
    std::cout << "loadFromFile ResourceB" << std::endl;
  }

  virtual void saveToFile(ResourceB const& /*object*/, std::string const& /*filename*/) const override
  {
    savedB = true;
    std::cout << "saveToFile ResourceB" << std::endl;
  }
};

#include "YesEngine/GenHierarchies.h"
using namespace Yes;

typedef TYPELIST_2(ResourceA, ResourceB) ResourceList;

#include "LoaderManager.tpp"

void LoaderManager::registerAllLoaders()
{
}

// Register the test suite
CPPUNIT_TEST_SUITE_REGISTRATION(LoadersTests);

//--------------------------------------------------------------------------
void LoadersTests::testsLoaderManager()
{
  LoaderManager &lm = LoaderManager::instance();
  lm.registerLoader<LoaderA, ResourceA>("a:A:aa::AA");
  lm.registerLoader<LoaderB, ResourceB>("bb");
  CPPUNIT_ASSERT_EQUAL(true, lm.hasLoader<ResourceA>("a"));
  CPPUNIT_ASSERT_EQUAL(true, lm.hasLoader<ResourceA>("A"));
  CPPUNIT_ASSERT_EQUAL(true, lm.hasLoader<ResourceA>("aa"));
  CPPUNIT_ASSERT_EQUAL(true, lm.hasLoader<ResourceA>("AA"));
  CPPUNIT_ASSERT_EQUAL(true, lm.hasLoader<ResourceB>("bb"));
  CPPUNIT_ASSERT_EQUAL(false, lm.hasLoader<ResourceA>("cc"));

  CPPUNIT_ASSERT_EQUAL(false, loadedA);
  CPPUNIT_ASSERT_EQUAL(false, loadedB);
  CPPUNIT_ASSERT_EQUAL(false, savedA);
  CPPUNIT_ASSERT_EQUAL(false, savedB);

  std::cout << "1-----------------------------------------" << std::endl;
  ResourceA rA;
  lm.saveToFile(rA, "/home/toto.AA~");
  CPPUNIT_ASSERT_EQUAL(true, savedA);

  std::cout << "2-----------------------------------------" << std::endl;
  ResourceA rA1;
  CPPUNIT_ASSERT_THROW(lm.loadFromFile("/home/tutu.a", rA1), LoaderException);
  CPPUNIT_ASSERT_EQUAL(false, loadedA);

  std::cout << "3-----------------------------------------" << std::endl;
  { std::fstream fs; fs.open("/tmp/toto.a", std::ios::out); fs.close(); } // create a file
  lm.loadFromFile("/tmp/toto.a", rA1);
  CPPUNIT_ASSERT_EQUAL(true, loadedA);

  std::cout << "4-----------------------------------------" << std::endl;
  ResourceB rB;
  lm.saveToFile(rB, "/home/toto.AA.bb");
  CPPUNIT_ASSERT_EQUAL(true, savedB);
  ResourceB rB1;
  { std::fstream fs; fs.open("/tmp/tutu.bb", std::ios::out); fs.close(); } // create a file
  lm.loadFromFile("/tmp/tutu.bb", rB1);
  CPPUNIT_ASSERT_EQUAL(true, loadedB);

  CPPUNIT_ASSERT_THROW(lm.saveToFile(rB, "/home/toto.cc"), LoaderException);
  CPPUNIT_ASSERT_THROW(lm.saveToFile(rB, "/home/toto"), LoaderException);

  // Use already existing extension
  lm.registerLoader<LoaderB, ResourceB>("a"); //egisterLoader(new LoaderB(), "a");
  lm.saveToFile(rB, "/home/toto.a");
  lm.saveToFile(rB, "/home/toto.bb");
  lm.saveToFile(rA, "/home/toto.a");
}
