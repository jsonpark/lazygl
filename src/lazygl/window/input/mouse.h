#ifndef LAZYGL_INPUT_MOUSE_H_
#define LAZYGL_INPUT_MOUSE_H_

namespace lazygl
{
namespace input
{
enum class MouseButton : unsigned char
{
  LEFT,
  MIDDLE,
  RIGHT,
  UNKNOWN
};

enum class MouseAction : unsigned char
{
  PRESS,
  RELEASE,
  UNKNOWN
};

MouseButton ConvertGlfwMouseButton(int glfw_button);

MouseAction ConvertGlfwMouseAction(int glfw_action);
}
}

#endif // LAZYGL_INPUT_MOUSE_H_
