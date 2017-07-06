#include "Example01.hpp"
#include "Example02.hpp"
#include "Example03.hpp"
#include "Example04.hpp"
#include "Example05.hpp"
//#include "Example06.hpp"
#include <memory>

#define MAX_EXAMPLES 6

static void usage(char *argv[])
{
  std::cout << "Usage: " << std::endl
            << argv[0] << "example       example is an integer meaning the nth example (1 .. " << MAX_EXAMPLES << ")" << std::endl;
  exit(1);
}

int main(int argc, char *argv[])
{
  if (argc <= 1)
    {
      usage(argv);
    }
  long int example = strtol(argv[1], nullptr, 10);
  if ((0L == example) || (example > MAX_EXAMPLES))
    {
      usage(argv);
    }

  IGLWindow *win;

  switch (example)
    {
    case 1:
      win = new GLExample01();
      break;
    case 2:
      win = new GLExample02();
      break;
    case 3:
      win = new GLExample03();
      break;
    case 4:
      win = new GLExample04();
      break;
    case 5:
      win = new GLExample05();
      break;
    default:
      //win = new GLExample06();
      break;
    }

  std::shared_ptr<IGLWindow> window(win);
  window->create();

  return 0;
}
