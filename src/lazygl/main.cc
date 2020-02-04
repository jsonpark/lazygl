#include "lazygl/engine.h"
#include "lazygl/renderer/painter2d.h"
#include "lazygl/renderer/renderer.h"

#include <iostream>
#include <exception>

int main()
{
  lazygl::Engine engine;
  auto test_window = engine.CreateWindowPtr<lazygl::Painter2D>(800, 600, "Painter2d");
  auto renderer = engine.CreateWindowPtr<lazygl::Renderer>(800, 600, "Renderer");

  try
  {
    engine.Run();
  }
  catch (std::exception e)
  {
    std::cout << e.what() << std::endl;
    return -1;
  }

  return 0;
}
