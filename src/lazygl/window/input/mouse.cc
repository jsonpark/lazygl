#include "lazygl/window/input/mouse.h"

#include <GLFW/glfw3.h>

namespace lazygl
{
namespace input
{
MouseButton ConvertGlfwMouseButton(int glfw_button)
{
  switch (glfw_button)
  {
  case GLFW_MOUSE_BUTTON_LEFT:
    return MouseButton::LEFT;

  case GLFW_MOUSE_BUTTON_RIGHT:
    return MouseButton::RIGHT;

  case GLFW_MOUSE_BUTTON_MIDDLE:
    return MouseButton::MIDDLE;

  default:
    return MouseButton::UNKNOWN;
  }
}

MouseAction ConvertGlfwMouseAction(int glfw_action)
{
  switch (glfw_action)
  {
  case GLFW_PRESS:
    return MouseAction::PRESS;

  case GLFW_RELEASE:
    return MouseAction::RELEASE;

  default:
    return MouseAction::UNKNOWN;
  }
}
}
}
