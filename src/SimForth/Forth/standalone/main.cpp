#include "Forth.hpp"
#include "SharedLibrary.hpp"
#include <unistd.h>

static void usage(const std::string& fun)
{
  std::cout << "Usage:   " << fun << " [-option] [argument]" << std::endl;
  std::cout << "option:  " << "-h              Show help information" << std::endl;
  std::cout << "option:  " << "-u              Show help information" << std::endl;
  std::cout << "         " << "-l dico         Load a SimForth dictionary binary file instead of booting the by default core" << std::endl;
  std::cout << "         " << "-d dico         Dump the dictionary as a binary file" << std::endl;
  std::cout << "         " << "-f file         Run a SimForth text file" << std::endl;
  std::cout << "         " << "-e string       Execute a string as SimForth script" << std::endl;
  std::cout << "         " << "-p              Pretty print the dictionary" << std::endl;
  std::cout << "         " << "-i              Interactive mode" << std::endl;
}

/*
typedef void (*HelloFunc)(); // function pointer type
static void test()
{
  //std::string path("/usr/lib/i386-linux-gnu/libGL.so");
  //path.append(SharedLibrary::suffix()); // adds ".dll" or ".so"
  //std::cout << path << std::endl;

  SharedLibrary library("/usr/lib/i386-linux-gnu/libGL.so");//path); // will also load the library

  //std::cout << "passed\n";

  HelloFunc func = (HelloFunc) library.getSymbol("glClear");
  func();
  library.unload();
}
*/
int main(int argc,char *argv[])
{
  Forth forth;
  std::pair<bool, std::string> res;
  bool r;
  int tmp;

  //test();

  // No option
  if (1 == argc)
  {
    usage(argv[0]);
    return 1;
  }

  // Boot the default core. Even if the user will load
  // a dictionary instead
  forth.boot();

  while ((tmp = getopt(argc, argv, "hul:d:f:e:pi")) != -1)
  {
    switch (tmp)
      {
        // Help infomation
      case 'h':
      case 'u':
        usage(argv[0]);
        return 1;
        break;

        // Load a dictionary
      case 'l':
        r = forth.dictionary().load(optarg);
        if (r)
          {
            std::cout << "Dictionary successfully loaded from file '"
                      << optarg << "'" << std::endl;
          }
        break;

        // Dump a dictionary
      case 'd':
        r = forth.dictionary().dump(optarg);
        if (r)
          {
            std::cout << "Dictionary successfully dumped in file '"
                      << optarg << "'" << std::endl;
          }
        break;

        // Execute a script file
      case 'f':
        res = forth.eatFile(optarg);
        forth.ok(res);
        break;

        // Execute a script given as option
      case 'e':
        res = forth.eatString(optarg);
        forth.ok(res);
        break;

        // Pretty print the dictionary
      case 'p':
        forth.dictionary().display();
        break;

        // Interactive mode
      case 'i':
        std::cerr << "Not yet implemented" << std::endl;
        return 1;
        break;

      default:
        break;
      }
  }

  return 0;
}
