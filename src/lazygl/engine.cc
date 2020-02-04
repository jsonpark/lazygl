#include "lazygl/engine.h"

#include <thread>

#include <GLFW/glfw3.h>

#include "lazygl/window/window.h"

namespace lazygl
{
Engine::Engine()
{
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

Engine::~Engine()
{
  glfwTerminate();
}

void Engine::Run()
{
  // Create windows
  for (auto window : windows_)
    window->Create();

  constexpr int target_fps = 60;
  constexpr double delay = 1. / target_fps;

  bool first = true;
  double swap_buffer_time = 0.;

  while (!ShouldClose())
  {
    glfwPollEvents();

    for (auto window : windows_)
    {
      window->MakeCurrent();
      window->PrepareDraw();
      window->Draw();
    }

    if (!first)
    {
      double elapsed_time = glfwGetTime() - swap_buffer_time;
      if (elapsed_time < delay)
        std::this_thread::sleep_for(std::chrono::duration<double, std::ratio<1>>(delay - elapsed_time));
    }

    swap_buffer_time = glfwGetTime();

    for (auto window : windows_)
      window->SwapBuffer();

    first = false;
  }
}

bool Engine::ShouldClose()
{
  for (int i = 0; i < windows_.size(); i++)
  {
    if (windows_[i]->ShouldClose())
    {
      windows_[i]->Destroy();
      for (int j = 0; j < windows_.size() - 1; j++)
        windows_[j] = windows_[static_cast<size_t>(j) + 1];
      windows_.pop_back();
      i--;
    }
  }

  for (auto window : windows_)
  {
    if (!window->ShouldClose())
      return false;
  }

  return true;
}
}
