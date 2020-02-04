#ifndef LAZYGL_WINDOW_WINDOW_H_
#define LAZYGL_WINDOW_WINDOW_H_

#include <string>

#include "lazygl/utils/logger.h"
#include "lazygl/window/input/mouse.h"
#include "lazygl/window/input/keyboard.h"

struct GLFWwindow;

namespace lazygl
{
class Window
{
public:
  Window();

  Window(int width, int height);

  Window(int width, int height, const std::string& title);

  ~Window();

  // Create called only once in Engine class
  void Create();
  virtual void Initialize();
  void Destroy();

  // Getters
  auto X() const
  {
    return x_;
  }

  auto Y() const
  {
    return y_;
  }

  auto Width() const
  {
    return width_;
  }

  auto Height() const
  {
    return height_;
  }

  // Callback functions
  virtual void MouseButton(input::MouseButton button, input::MouseAction action, double x, double y);
  virtual void MouseMove(double x, double y);
  virtual void Keyboard(input::Key key, input::KeyAction action);
  virtual void Resize(int width, int height);
  virtual void PrepareDraw();
  virtual void Draw();

  void MakeCurrent();
  bool ShouldClose() const;
  void SwapBuffer();

private:
  int x_ = -1;
  int y_ = -1;
  int width_ = 800;
  int height_ = 600;
  std::string title_;

  GLFWwindow* window_ = 0;
  bool window_created_ = false;

  Logger log_{ "Window" };
};
}

#endif // LAZYGL_WINDOW_WINDOW_H_
