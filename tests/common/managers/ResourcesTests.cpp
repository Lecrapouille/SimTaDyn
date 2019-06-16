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

#include "ResourcesTests.hpp"
#include "ClassCounter.tpp"
#include <iostream>
#include <fstream>

static bool destroyedA = false;
class ResourceA
{
public:

  ResourceA(uint32_t id, std::string const& str)
    : m_id(id), m_str(str)
  {
    std::cout << "Created ResourceA " << m_id
              << " " << m_str << std::endl;
  }

  ~ResourceA()
  {
     destroyedA = true;
  }

  uint32_t m_id;
  std::string m_str;
};

static bool destroyedB = false;
class ResourceB
  : private UniqueID<ResourceB>
{
public:

  ResourceB()
    : m_id("BB_" + std::to_string(UniqueID<ResourceB>::getID()))
  {
    std::cout << "Created ResourceB " << m_id << std::endl;
  }

  ~ResourceB()
  {
     destroyedB = true;
  }

  std::string m_id;
};

using RAPtr = std::shared_ptr<ResourceA>;
using RBPtr = std::shared_ptr<ResourceB>;
static ResourceManager<uint32_t, ResourceA>* RMA;
static ResourceManager<uint32_t, ResourceB>* RMB;

// Register the test suite
CPPUNIT_TEST_SUITE_REGISTRATION(ResourcesTests);

//--------------------------------------------------------------------------
void ResourcesTests::setUp()
{
  std::cout << "*** ResourcesTests ***************************************" << std::endl;
  RMA = new ResourceManager<uint32_t, ResourceA>;
  RMB = new ResourceManager<uint32_t, ResourceB>;
}

//--------------------------------------------------------------------------
void ResourcesTests::tearDown()
{
  if (nullptr != RMA)
    delete RMA;
  RMA = nullptr;

  if (nullptr != RMB)
    delete RMB;
  RMB = nullptr;
}

//--------------------------------------------------------------------------
void ResourcesTests::testsResourceManager()
{
  // -- Resources A

  CPPUNIT_ASSERT_EQUAL(false, destroyedA);
  {
    RAPtr rA1 = RMA->create(0U, resource::Strategy::ThrowException, 0U, "toto");
    CPPUNIT_ASSERT_EQUAL(true, nullptr != rA1);
    CPPUNIT_ASSERT_EQUAL(2l, rA1.use_count());
    CPPUNIT_ASSERT_EQUAL(0U, rA1->m_id);
    CPPUNIT_ASSERT_EQUAL(true, rA1->m_str == "toto");
    CPPUNIT_ASSERT_EQUAL(false, destroyedA);
    CPPUNIT_ASSERT_EQUAL(1_z, RMA->size());
  }

  RAPtr rA2 = RMA->create(1U, resource::Strategy::ThrowException, 1U, "foo");
  CPPUNIT_ASSERT_EQUAL(true, nullptr != rA2);
  CPPUNIT_ASSERT_EQUAL(2l, rA2.use_count());
  CPPUNIT_ASSERT_EQUAL(1U, rA2->m_id);
  CPPUNIT_ASSERT_EQUAL(true, rA2->m_str == "foo");
  CPPUNIT_ASSERT_EQUAL(false, destroyedA);
  CPPUNIT_ASSERT_EQUAL(2_z, RMA->size());

  RAPtr rA100 = RMA->create(100U, resource::Strategy::ThrowException, 100U, "100");
  CPPUNIT_ASSERT_EQUAL(true, nullptr != rA100);
  CPPUNIT_ASSERT_EQUAL(2l, rA100.use_count());
  CPPUNIT_ASSERT_EQUAL(100U, rA100->m_id);
  CPPUNIT_ASSERT_EQUAL(true, rA100->m_str == "100");
  CPPUNIT_ASSERT_EQUAL(false, destroyedA);
  CPPUNIT_ASSERT_EQUAL(3_z, RMA->size());

  // -- Resources B

  CPPUNIT_ASSERT_EQUAL(false, destroyedB);
  RBPtr rB1 = RMB->create(0U, resource::Strategy::ThrowException);
  CPPUNIT_ASSERT_EQUAL(true, nullptr != rB1);
  CPPUNIT_ASSERT_EQUAL(2l, rB1.use_count());
  CPPUNIT_ASSERT_EQUAL(true, rB1->m_id == "BB_1");
  CPPUNIT_ASSERT_EQUAL(false, destroyedB);
  CPPUNIT_ASSERT_EQUAL(1_z, RMB->size());

  RBPtr rB2 = RMB->create(1U, resource::Strategy::ThrowException);
  CPPUNIT_ASSERT_EQUAL(true, nullptr != rB2);
  CPPUNIT_ASSERT_EQUAL(2l, rB2.use_count());
  CPPUNIT_ASSERT_EQUAL(true, rB2->m_id == "BB_2");
  CPPUNIT_ASSERT_EQUAL(false, destroyedB);
  CPPUNIT_ASSERT_EQUAL(2_z, RMB->size());

  // Search resources and check ptr user counter
  {
    RAPtr rA = RMA->acquire(0U);
    CPPUNIT_ASSERT_EQUAL(true, nullptr != rA);
    CPPUNIT_ASSERT_EQUAL(0U, rA->m_id);
    CPPUNIT_ASSERT_EQUAL(2l, rA.use_count());
  }

  RAPtr rAA = RMA->acquire(1U);
  CPPUNIT_ASSERT_EQUAL(true, nullptr != rAA);
  CPPUNIT_ASSERT_EQUAL(1U, rAA->m_id);
  CPPUNIT_ASSERT_EQUAL(3l, rAA.use_count());

  // Try adding double: check exception
  try
    {
      RBPtr rB3 = RMB->create(0U, resource::Strategy::ThrowException);
      CPPUNIT_FAIL("ResourceManagerException should have occured");
    }
  catch (ResourceManagerException const& e)
    {
    }

  // Try adding double: check return nullptr
  RBPtr rB4 = RMB->create(0U, resource::Strategy::ReturnNull);
  CPPUNIT_ASSERT_EQUAL(true, nullptr == rB4);
  CPPUNIT_ASSERT_EQUAL(true, nullptr != RMB->acquire(0U));
  CPPUNIT_ASSERT_EQUAL(true, RMB->acquire(0U)->m_id == "BB_1");

  // Try adding double: check it has been replaced
  RAPtr rA3 = RMA->create(0U, resource::Strategy::Replace, 42U, "bar");
  CPPUNIT_ASSERT_EQUAL(true, nullptr != rA3);
  CPPUNIT_ASSERT_EQUAL(2l, rA3.use_count());
  CPPUNIT_ASSERT_EQUAL(42U, rA3->m_id);
  CPPUNIT_ASSERT_EQUAL(true, rA3->m_str == "bar");
  CPPUNIT_ASSERT_EQUAL(true, destroyedA);
  CPPUNIT_ASSERT_EQUAL(3_z, RMA->size());
  destroyedA = false;

  // Try adding double: check it the older is still used
  RAPtr rA4 = RMA->create(1U, resource::Strategy::UseOlder, 42U, "bar");
  CPPUNIT_ASSERT_EQUAL(true, nullptr != rA4);
  CPPUNIT_ASSERT_EQUAL(4l, rA4.use_count());
  CPPUNIT_ASSERT_EQUAL(1U, rA4->m_id);
  CPPUNIT_ASSERT_EQUAL(true, rA4->m_str == "foo");
  CPPUNIT_ASSERT_EQUAL(false, destroyedA);
  CPPUNIT_ASSERT_EQUAL(3_z, RMA->size());

  // Try acquiring a non existant resource
  RAPtr rA5 = RMA->acquire(55U);
  CPPUNIT_ASSERT_EQUAL(true, nullptr == rA5);

  // Get a resource
  ResourceA a = (*RMA)[1];
  CPPUNIT_ASSERT_EQUAL(1U, a.m_id);
  CPPUNIT_ASSERT_EQUAL(true, a.m_str == "foo");
  CPPUNIT_ASSERT_EQUAL(3_z, RMA->size());

  // Remove a non existing resource
  RMA->remove(55U);
  CPPUNIT_ASSERT_EQUAL(true, nullptr == rA5);

  // Remove-weak an existing resource
  RMA->remove(1U, false);
  RAPtr rA6 = RMA->acquire(1U);
  CPPUNIT_ASSERT_EQUAL(true, nullptr != rA6);

  // Remove-force an existing resource
  RMA->remove(1U);
  RAPtr rA7 = RMA->acquire(1U);
  CPPUNIT_ASSERT_EQUAL(true, nullptr == rA7);

  // Rename a resource that does not exists
  try
    {
      RAPtr nA1 = RMA->rename(55U, 0U, resource::Strategy::ThrowException);
      CPPUNIT_FAIL("ResourceManagerException should have occured");
    }
  catch (ResourceManagerException const& e)
    {
    }

  // Rename a resource that does not exists
  RAPtr nA2 = RMA->rename(55U, 0U, resource::Strategy::ReturnNull);
  CPPUNIT_ASSERT_EQUAL(true, nullptr == nA2);

  // Rename a resource with its own id
  RAPtr nA3 = RMA->rename(0U, 0U, resource::Strategy::ReturnNull);
  CPPUNIT_ASSERT_EQUAL(true, nullptr != nA3);
  CPPUNIT_ASSERT_EQUAL(42U, nA3->m_id);
  CPPUNIT_ASSERT_EQUAL(true, nA3->m_str == "bar");

  // Rename an existing resource
  RAPtr nA4 = RMA->rename(0U, 66U, resource::Strategy::ReturnNull);
  CPPUNIT_ASSERT_EQUAL(true, nullptr != nA4);
  CPPUNIT_ASSERT_EQUAL(42U, nA4->m_id);
  CPPUNIT_ASSERT_EQUAL(true, nA4->m_str == "bar");

  // Rename an acquired resource
  RAPtr nA5 = RMA->acquire(66U);
  CPPUNIT_ASSERT_EQUAL(42U, nA5->m_id);
  CPPUNIT_ASSERT_EQUAL(true, nA5->m_str == "bar");
  RMA->rename(66U, 0U, resource::Strategy::ReturnNull);
  RAPtr nA6 = RMA->acquire(0U);
  CPPUNIT_ASSERT_EQUAL(42U, nA6->m_id);
  CPPUNIT_ASSERT_EQUAL(true, nA6->m_str == "bar");

  // Rename an resource but the new key already exists
  RAPtr nA7 = RMA->rename(0U, 100U, resource::Strategy::ReturnNull);
  CPPUNIT_ASSERT_EQUAL(true, nullptr == nA7);

  // Rename an resource but the new key already exists
  try
    {
      RAPtr nA8 = RMA->rename(0U, 100U, resource::Strategy::ThrowException);
      CPPUNIT_FAIL("ResourceManagerException should have occured");
    }
  catch (ResourceManagerException const& e)
    {
    }

  // Rename an resource but the new key already exists
  RMA->create(101U, resource::Strategy::ThrowException, 101U, "101");
  RMA->create(102U, resource::Strategy::ThrowException, 102U, "102");
  RMA->create(103U, resource::Strategy::ThrowException, 103U, "103");
  RMA->create(104U, resource::Strategy::ThrowException, 104U, "104");

  RAPtr nA8 = RMA->rename(101U, 102U, resource::Strategy::UseOlder);
  CPPUNIT_ASSERT_EQUAL(true, nullptr != nA8);
  CPPUNIT_ASSERT_EQUAL(101U, nA8->m_id);
  CPPUNIT_ASSERT_EQUAL(true, nA8->m_str == "101");
  CPPUNIT_ASSERT_EQUAL(true, nullptr == RMA->acquire(101U));
  CPPUNIT_ASSERT_EQUAL(true, nullptr != RMA->acquire(102U));

  // Rename an resource but the new key already exists
  RAPtr nA9 = RMA->rename(103U, 104U, resource::Strategy::Replace);
  CPPUNIT_ASSERT_EQUAL(true, nullptr != nA9);
  CPPUNIT_ASSERT_EQUAL(104U, nA9->m_id);
  CPPUNIT_ASSERT_EQUAL(true, nA9->m_str == "104");
  CPPUNIT_ASSERT_EQUAL(true, nullptr == RMA->acquire(103U));
  CPPUNIT_ASSERT_EQUAL(true, nullptr != RMA->acquire(104U));

  // Destroy the manager while resource is used.
  // Check the manager detects it.
  RAPtr rA9 = RMA->acquire(0U);
  {
    delete RMA;
  }
  RMA = new ResourceManager<uint32_t, ResourceA>;
}

static bool loadedA = false;
static bool loadedB = false;
static bool savedA = false;
static bool savedB = false;

class Resource1
{
public:

  Resource1() {}
  ~Resource1() {}
};

class Resource2
{
public:

  Resource2() {}
  ~Resource2() {}
};

class LoaderA: public ILoader<Resource1>
{
public:

  LoaderA()
    : ILoader<Resource1>("Loader Resource1")
  {}

  ~LoaderA() {}

  virtual void loadFromFile(std::string const& /*filename*/, Resource1& /*object*/) override
  {
    loadedA = true;
    std::cout << "loadFromFile Resource1" << std::endl;
  }

  virtual void saveToFile(Resource1 const& /*object*/, std::string const& /*filename*/) const override
  {
    savedA = true;
    std::cout << "saveToFile Resource1" << std::endl;
  }
};

class LoaderB: public ILoader<Resource2>
{
public:
  LoaderB()
    : ILoader<Resource2>("Loader Resource2")
  {}

  ~LoaderB() {}

  virtual void loadFromFile(std::string const& /*filename*/, Resource2& /*object*/) override
  {
    loadedB = true;
    std::cout << "loadFromFile Resource2" << std::endl;
  }

  virtual void saveToFile(Resource2 const& /*object*/, std::string const& /*filename*/) const override
  {
    savedB = true;
    std::cout << "saveToFile Resource2" << std::endl;
  }
};

#include "YesEngine/GenHierarchies.h"
using namespace Yes;

typedef TYPELIST_2(Resource1, Resource2) ResourceList;

#include "LoaderManager.tpp"

void LoaderManager::registerAllLoaders()
{
}

//--------------------------------------------------------------------------
void ResourcesTests::testsLoaderManager()
{
  LoaderManager &lm = LoaderManager::instance();
  lm.registerLoader<LoaderA, Resource1>("a:A:aa::AA");
  lm.registerLoader<LoaderB, Resource2>("bb");
  CPPUNIT_ASSERT_EQUAL(true, lm.hasLoader<Resource1>("a"));
  CPPUNIT_ASSERT_EQUAL(true, lm.hasLoader<Resource1>("A"));
  CPPUNIT_ASSERT_EQUAL(true, lm.hasLoader<Resource1>("aa"));
  CPPUNIT_ASSERT_EQUAL(true, lm.hasLoader<Resource1>("AA"));
  CPPUNIT_ASSERT_EQUAL(true, lm.hasLoader<Resource2>("bb"));
  CPPUNIT_ASSERT_EQUAL(false, lm.hasLoader<Resource1>("cc"));

  CPPUNIT_ASSERT_EQUAL(false, loadedA);
  CPPUNIT_ASSERT_EQUAL(false, loadedB);
  CPPUNIT_ASSERT_EQUAL(false, savedA);
  CPPUNIT_ASSERT_EQUAL(false, savedB);

  std::cout << "1-----------------------------------------" << std::endl;
  Resource1 rA;
  lm.saveToFile(rA, "/home/toto.AA~");
  CPPUNIT_ASSERT_EQUAL(true, savedA);

  std::cout << "2-----------------------------------------" << std::endl;
  Resource1 rA1;
  CPPUNIT_ASSERT_THROW(lm.loadFromFile("/home/tutu.a", rA1), LoaderManagerException);
  CPPUNIT_ASSERT_EQUAL(false, loadedA);

  std::cout << "3-----------------------------------------" << std::endl;
  { std::fstream fs; fs.open("/tmp/toto.a", std::ios::out); fs.close(); } // create a file
  lm.loadFromFile("/tmp/toto.a", rA1);
  CPPUNIT_ASSERT_EQUAL(true, loadedA);

  std::cout << "4-----------------------------------------" << std::endl;
  Resource2 rB;
  lm.saveToFile(rB, "/home/toto.AA.bb");
  CPPUNIT_ASSERT_EQUAL(true, savedB);
  Resource2 rB1;
  { std::fstream fs; fs.open("/tmp/tutu.bb", std::ios::out); fs.close(); } // create a file
  lm.loadFromFile("/tmp/tutu.bb", rB1);
  CPPUNIT_ASSERT_EQUAL(true, loadedB);

  CPPUNIT_ASSERT_THROW(lm.saveToFile(rB, "/home/toto.cc"), LoaderException);
  CPPUNIT_ASSERT_THROW(lm.saveToFile(rB, "/home/toto"), LoaderException);

  // Use already existing extension
  lm.registerLoader<LoaderB, Resource2>("a"); //egisterLoader(new LoaderB(), "a");
  lm.saveToFile(rB, "/home/toto.a");
  lm.saveToFile(rB, "/home/toto.bb");
  lm.saveToFile(rA, "/home/toto.a");
}
