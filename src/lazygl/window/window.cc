#include "lazygl/window/window.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace lazygl
{
namespace
{
void FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
  auto window_object = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));

  window_object->Resize(width, height);
}

void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
  double x, y;
  glfwGetCursorPos(window, &x, &y);

  auto window_object = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));

  window_object->MouseButton(input::ConvertGlfwMouseButton(button), input::ConvertGlfwMouseAction(action), x, y);
}

void CursorPosCallback(GLFWwindow* window, double x, double y)
{
  auto window_object = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));

  window_object->MouseMove(x, y);
}

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
  auto window_object = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));

  window_object->Keyboard(input::ConvertGlfwKey(key), input::ConvertGlfwKeyAction(action));
}
}

Window::Window()
  : Window(800, 600)
{
}

Window::Window(int width, int height)
  : Window(width, height, "Window")
{
}

Window::Window(int width, int height, const std::string& title)
  : title_(title)
{
}

Window::~Window()
{
  Destroy();
}

void Window::Create()
{
  window_ = glfwCreateWindow(width_, height_, title_.c_str(), NULL, NULL);
  if (window_ == NULL)
  {
    log_ << "Failed to create GLFW window" << Logger::endl;
    return;
  }
  window_created_ = true;

  MakeCurrent();
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    log_ << "Failed to initialize GLAD" << Logger::endl;
    return;
  }

  if (x_ != -1 && y_ != -1)
    glfwSetWindowPos(window_, x_, y_);

  glfwGetWindowPos(window_, &x_, &y_);

  // Enroll callback functions
  glfwSetWindowUserPointer(window_, this);
  glfwSetMouseButtonCallback(window_, MouseButtonCallback);
  glfwSetCursorPosCallback(window_, CursorPosCallback);
  glfwSetKeyCallback(window_, KeyCallback);
  glfwSetFramebufferSizeCallback(window_, FramebufferSizeCallback);

  Initialize();
}

void Window::Initialize()
{
  log_ << "Initialize" << Logger::endl;
}

void Window::Destroy()
{
  if (window_created_)
  {
    glfwDestroyWindow(window_);
    window_created_ = false;
  }
}

void Window::MouseButton(input::MouseButton button, input::MouseAction action, double x, double y)
{
  log_ << "MouseButton" << Logger::endl;
}

void Window::MouseMove(double x, double y)
{
  log_ << "MouseMove" << Logger::endl;
}

void Window::Keyboard(input::Key key, input::KeyAction action)
{
  log_ << "Keyboard" << Logger::endl;
}

void Window::Resize(int width, int height)
{
  width_ = width;
  height_ = height;
}

void Window::PrepareDraw()
{
}

void Window::Draw()
{
}

bool Window::ShouldClose() const
{
  return window_created_ && glfwWindowShouldClose(window_);
}

void Window::MakeCurrent()
{
  glfwMakeContextCurrent(window_);
}

void Window::SwapBuffer()
{
  glfwSwapBuffers(window_);
}
}
