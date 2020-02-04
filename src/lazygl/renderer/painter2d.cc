#include "lazygl/renderer/painter2d.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace lazygl
{
Painter2D::Painter2D()
  : Painter2D(800, 600)
{
}

Painter2D::Painter2D(int width, int height)
  : Painter2D(width, height, "Render2D Window")
{
}

Painter2D::Painter2D(int width, int height, const std::string& title)
  : Window(width, height, title)
{
}

Painter2D::~Painter2D() = default;

void Painter2D::MouseButton(input::MouseButton button, input::MouseAction action, double x, double y)
{
}

void Painter2D::MouseMove(double x, double y)
{
}

void Painter2D::Keyboard(input::Key key, input::KeyAction action)
{
}

void Painter2D::Resize(int width, int height)
{
  Window::Resize(width, height);
}

void Painter2D::Initialize()
{
  glClearColor(1.f, 1.f, 1.f, 1.f);
  glDisable(GL_DEPTH_TEST);

  rect_.AttribPointer(0, 2, rect_buffer_, 5, 0);
  rect_.AttribPointer(1, 3, rect_buffer_, 5, 2);
  rect_.Indices(VertexArray::DrawMode::TRIANGLE_STRIP, rect_elements_);

  rect_texture_.AttribPointer(0, 2, rect_buffer_, 4, 0);
  rect_texture_.AttribPointer(1, 2, rect_buffer_, 4, 2);
  rect_texture_.Indices(VertexArray::DrawMode::TRIANGLE_STRIP, rect_elements_);

  texture_(0, 0) = Vector<unsigned char, 4>(255, 255, 255, 255);
  texture_(1, 0) = Vector<unsigned char, 4>(0, 0, 0, 255);
  texture_(0, 1) = Vector<unsigned char, 4>(0, 0, 0, 255);
  texture_(1, 1) = Vector<unsigned char, 4>(255, 255, 255, 255);

  tex_object_.Update(texture_);
  tex_object_.WrapS(TextureObject::Wrap::REPEAT);
  tex_object_.WrapT(TextureObject::Wrap::REPEAT);
  tex_object_.MinFilter(TextureObject::MinFilter::NEAREST);
  tex_object_.MagFilter(TextureObject::MagFilter::NEAREST);
}

void Painter2D::PrepareDraw()
{
  program_screen_color_.Uniform2i("screen", Width(), Height());
  program_screen_texture_.Uniform2i("screen", Width(), Height());
}

void Painter2D::Draw()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glViewport(0, 0, Width(), Height());

  DrawRect(0, 0, 200, 100, Vector3f(0.5, 0.5, 1.0));
  DrawRect(200, 0, 200, 100, Vector3f(0.0, 0.0, 1.0));

  DrawTexture(200, 200, 400, 300, tex_object_);
}

void Painter2D::DrawRect(double x, double y, double width, double height, const Vector3f& color)
{
  constexpr int stride = 5;

  rect_buffer_[0 * stride + 0] = static_cast<float>(x);
  rect_buffer_[0 * stride + 1] = static_cast<float>(y);

  rect_buffer_[1 * stride + 0] = static_cast<float>(x + width);
  rect_buffer_[1 * stride + 1] = static_cast<float>(y);

  rect_buffer_[2 * stride + 0] = static_cast<float>(x);
  rect_buffer_[2 * stride + 1] = static_cast<float>(y + height);

  rect_buffer_[3 * stride + 0] = static_cast<float>(x + width);
  rect_buffer_[3 * stride + 1] = static_cast<float>(y + height);

  for (int i = 0; i < 4; i++)
  {
    rect_buffer_[i * stride + 2] = color(0);
    rect_buffer_[i * stride + 3] = color(1);
    rect_buffer_[i * stride + 4] = color(2);
  }

  program_screen_color_.Use();
  rect_.Draw();
}
}
