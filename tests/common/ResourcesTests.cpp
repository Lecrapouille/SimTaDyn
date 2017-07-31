#include "ResourcesTests.hpp"
#include "ClassCounter.hpp"
#include <iostream>
#include <fstream>

// Register the test suite
CPPUNIT_TEST_SUITE_REGISTRATION(ResourcesTests);

static bool destroyedA = false;
class ResourceA
  : public IResource<uint32_t>,
    private ClassCounter<ResourceA>
{
public:

  ResourceA()
    : IResource(ClassCounter<ResourceA>::count())
  {
  }

  ~ResourceA()
  {
     destroyedA = true;
  }
};

static bool destroyedB = false;
class ResourceB
  : public IResource<std::string>,
    private ClassCounter<ResourceB>
{
public:

  ResourceB()
    : IResource("BB_" + std::to_string(ClassCounter<ResourceB>::count()))
  {
  }

  ~ResourceB()
  {
     destroyedB = true;
  }
};

//--------------------------------------------------------------------------
void ResourcesTests::setUp()
{
}

//--------------------------------------------------------------------------
void ResourcesTests::tearDown()
{
}

//--------------------------------------------------------------------------
void ResourcesTests::testsResources()
{
  // -- Resource A

  CPPUNIT_ASSERT_EQUAL(false, destroyedA);
  ResourceA *rA1 = new ResourceA();
  CPPUNIT_ASSERT_EQUAL(0U, rA1->owners());
  CPPUNIT_ASSERT_EQUAL(0U, rA1->id());
  CPPUNIT_ASSERT_EQUAL(false, destroyedA);

  ResourceA *rA2 = new ResourceA();
  CPPUNIT_ASSERT_EQUAL(0U, rA1->owners());
  CPPUNIT_ASSERT_EQUAL(0U, rA2->owners());
  CPPUNIT_ASSERT_EQUAL(1U, rA2->id());
  CPPUNIT_ASSERT_EQUAL(false, destroyedA);

  rA1->acquire();
  CPPUNIT_ASSERT_EQUAL(1U, rA1->owners());
  CPPUNIT_ASSERT_EQUAL(false, destroyedA);
  rA1->dispose();
  CPPUNIT_ASSERT_EQUAL(0U, rA1->owners());
  rA1->dispose();
  CPPUNIT_ASSERT_EQUAL(true, destroyedA);

  // -- Resource B

  CPPUNIT_ASSERT_EQUAL(false, destroyedB);
  ResourceB *rB1 = new ResourceB();
  CPPUNIT_ASSERT_EQUAL(0U, rB1->owners());
  CPPUNIT_ASSERT_EQUAL(0, rB1->id().compare("BB_0"));
  CPPUNIT_ASSERT_EQUAL(false, destroyedB);

  ResourceB *rB2 = new ResourceB();
  CPPUNIT_ASSERT_EQUAL(0U, rB1->owners());
  CPPUNIT_ASSERT_EQUAL(0U, rB2->owners());
  CPPUNIT_ASSERT_EQUAL(0, rB2->id().compare("BB_1"));
  CPPUNIT_ASSERT_EQUAL(false, destroyedB);

  rB1->acquire();
  CPPUNIT_ASSERT_EQUAL(1U, rB1->owners());
  CPPUNIT_ASSERT_EQUAL(false, destroyedB);
  rB1->dispose();
  CPPUNIT_ASSERT_EQUAL(0U, rB1->owners());
  rB1->dispose();
  CPPUNIT_ASSERT_EQUAL(true, destroyedB);
}

//--------------------------------------------------------------------------
void ResourcesTests::testsResourceManager()
{
  // Check empty resource manager
  ResourceManager<uint32_t> rm;
  CPPUNIT_ASSERT_EQUAL(0U, rm.size());
  CPPUNIT_ASSERT_EQUAL(true, nullptr == rm.look(0U));
  CPPUNIT_ASSERT_EQUAL(true, nullptr == rm.look(1U));

  // Insert resources in the resource manager.
  rm.add(new ResourceA());
  rm.add(new ResourceA());
  CPPUNIT_ASSERT_EQUAL(2U, rm.size());
  CPPUNIT_ASSERT_EQUAL(true, nullptr != rm.look(1U));
  CPPUNIT_ASSERT_EQUAL(true, nullptr != rm.look(2U));
  CPPUNIT_ASSERT_EQUAL(true, nullptr == rm.look(3U));

  // Insert resource #3 and acquire it
  rm.add( new ResourceA());
  CPPUNIT_ASSERT_EQUAL(3U, rm.size());
  CPPUNIT_ASSERT_EQUAL(true, nullptr == rm.acquire(41U));
  CPPUNIT_ASSERT_EQUAL(true, nullptr != rm.acquire(3U));
  CPPUNIT_ASSERT_EQUAL(1U, ((ResourceA*) rm.look(3U))->owners());

  // Check disposing of the resource already acquired.
  rm.dispose(3U);
  CPPUNIT_ASSERT_EQUAL(true, nullptr != rm.acquire(3U));
  CPPUNIT_ASSERT_EQUAL(1U, ((ResourceA*) rm.look(3U))->owners());
  rm.dispose(3U);
  CPPUNIT_ASSERT_EQUAL(0U, ((ResourceA*) rm.look(3U))->owners());

  // Dispose the resource and check it's no longer exists.
  rm.dispose(3U);
  CPPUNIT_ASSERT_EQUAL(true, nullptr == rm.acquire(3U));
  CPPUNIT_ASSERT_EQUAL(2U, rm.size());
}

static bool loadedA = false;
static bool loadedB = false;
static bool savedA = false;
static bool savedB = false;

class LoaderA: public ILoader<ResourceA>
{
public:
  LoaderA() : ILoader<ResourceA>("Loader ResourceA") {};
  ~LoaderA() {};
  virtual void loadFromFile(std::string const& /*filename*/, ResourceA* &object) override
  {
    loadedA = true;
    std::cout << "ResourceA* loadFromFile(std::string const& filename)" << std::endl;
    object = new ResourceA();
  }
  virtual void saveToFile(ResourceA const& /*object*/, std::string const& /*filename*/) override
  {
    savedA = true;
    std::cout << "void saveToFile(ResourceA const& /*object*/, std::string const& filename)" << std::endl;
  }
};

class LoaderB: public ILoader<ResourceB>
{
public:
  LoaderB() : ILoader<ResourceB>("Loader ResourceB") {};
  ~LoaderB() {};
  virtual void loadFromFile(std::string const& /*filename*/, ResourceB* &object) override
  {
    loadedB = true;
    std::cout << "ResourceB* loadFromFile(std::string const& filename)" << std::endl;
    object = new ResourceB();
  }
virtual void saveToFile(ResourceB const& /*object*/, std::string const& /*filename*/) override
  {
    savedB = true;
    std::cout << "void saveToFile(ResourceB const& /*object*/, std::string const& filename)" << std::endl;
  }
};

#  include "Utilities/GenHierarchies.h"

typedef TYPELIST_2(ResourceA, ResourceB) ResourceList;

#  include "LoaderManager.tpp"

//--------------------------------------------------------------------------
void ResourcesTests::testsLoaderManager()
{
  LoaderManager &lm = LoaderManager::instance();
  lm.registerLoader(new LoaderA(), "a:A:aa::AA");
  lm.registerLoader(new LoaderB(), "bb");
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
  ResourceA *rA1 = nullptr;
  CPPUNIT_ASSERT_THROW(lm.loadFromFile("/home/tutu.a", rA1), LoaderException);
  CPPUNIT_ASSERT_EQUAL(true, nullptr == rA1); // the file does not exist
  CPPUNIT_ASSERT_EQUAL(false, loadedA);

  std::cout << "3-----------------------------------------" << std::endl;
  { std::fstream fs; fs.open("/tmp/toto.a", std::ios::out); fs.close(); } // create a file
  lm.loadFromFile("/tmp/toto.a", rA1);
  CPPUNIT_ASSERT_EQUAL(true, nullptr != rA1); // the file exists
  CPPUNIT_ASSERT_EQUAL(true, loadedA);

  std::cout << "4-----------------------------------------" << std::endl;
  ResourceB rB;
  lm.saveToFile(rB, "/home/toto.AA.bb");
  CPPUNIT_ASSERT_EQUAL(true, savedB);
  ResourceB *rB1;
  { std::fstream fs; fs.open("/tmp/tutu.bb", std::ios::out); fs.close(); } // create a file
  lm.loadFromFile("/tmp/tutu.bb", rB1);
  CPPUNIT_ASSERT_EQUAL(true, loadedB);

  CPPUNIT_ASSERT_THROW(lm.saveToFile(rB, "/home/toto.cc"), LoaderException);
  CPPUNIT_ASSERT_THROW(lm.saveToFile(rB, "/home/toto"), LoaderException);

  // Use already existing extension
  lm.registerLoader(new LoaderB(), "a");
  lm.saveToFile(rB, "/home/toto.a");
  lm.saveToFile(rB, "/home/toto.bb");
  lm.saveToFile(rA, "/home/toto.a");
}
