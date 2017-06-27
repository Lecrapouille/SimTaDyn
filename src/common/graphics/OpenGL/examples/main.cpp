#include "Example01.hpp"
#include "Example02.hpp"
#include "Example03.hpp"
#include <memory>

int main(void)
{
  std::shared_ptr<IGLWindow> window(new GLExample03());

  window->create();
  return 0;
}
