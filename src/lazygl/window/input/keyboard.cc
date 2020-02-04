#include "lazygl/window/input/keyboard.h"

#include <GLFW/glfw3.h>

namespace lazygl
{
namespace input
{
Key ConvertGlfwKey(int glfw_key)
{
  return static_cast<Key>(glfw_key);
}

KeyAction ConvertGlfwKeyAction(int glfw_action)
{
  switch (glfw_action)
  {
  case GLFW_PRESS:
    return KeyAction::PRESS;

  case GLFW_RELEASE:
    return KeyAction::RELEASE;

  case GLFW_REPEAT:
    return KeyAction::REPEAT;

  default:
    return KeyAction::UNKNOWN;
  }
}
}
}
