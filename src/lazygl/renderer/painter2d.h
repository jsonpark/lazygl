#ifndef LAZYGL_WINDOW_PAINTER2D_H_
#define LAZYGL_WINDOW_PAINTER2D_H_

#include "lazygl/window/window.h"

#include "lazygl/object/buffer.h"
#include "lazygl/object/vertex_array.h"
#include "lazygl/geom/texture.h"
#include "lazygl/object/texture_object.h"
#include "lazygl/shader/program.h"

namespace lazygl
{
class Painter2D : public Window
{
public:
  Painter2D();

  Painter2D(int width, int height);

  Painter2D(int width, int height, const std::string& title);

  ~Painter2D();

  void Initialize() override;

  // Callback functions
  void MouseButton(input::MouseButton button, input::MouseAction action, double x, double y) override;
  void MouseMove(double x, double y) override;
  void Keyboard(input::Key key, input::KeyAction action) override;
  void Resize(int width, int height) override;
  void PrepareDraw() override;
  void Draw() override;

  // Utility functions
  void DrawRect(double x, double y, double width, double height, const Vector3f& color);

  template <typename T>
  void DrawTexture(double x, double y, double width, double height, TextureObject2D<T>& texture_object)
  {
    constexpr int stride = 4;

    rect_buffer_[0 * stride + 0] = static_cast<float>(x);
    rect_buffer_[0 * stride + 1] = static_cast<float>(y);
    rect_buffer_[0 * stride + 2] = 0.f;
    rect_buffer_[0 * stride + 3] = 0.f;

    rect_buffer_[1 * stride + 0] = static_cast<float>(x + width);
    rect_buffer_[1 * stride + 1] = static_cast<float>(y);
    rect_buffer_[1 * stride + 2] = 0.f;
    rect_buffer_[1 * stride + 3] = 1.f;

    rect_buffer_[2 * stride + 0] = static_cast<float>(x);
    rect_buffer_[2 * stride + 1] = static_cast<float>(y + height);
    rect_buffer_[2 * stride + 2] = 1.f;
    rect_buffer_[2 * stride + 3] = 0.f;

    rect_buffer_[3 * stride + 0] = static_cast<float>(x + width);
    rect_buffer_[3 * stride + 1] = static_cast<float>(y + height);
    rect_buffer_[3 * stride + 2] = 1.f;
    rect_buffer_[3 * stride + 3] = 1.f;

    program_screen_texture_.Use();
    texture_object.Bind();
    rect_texture_.Draw();
    texture_object.Unbind();
  }

private:
  ArrayBuffer<float> rect_buffer_{
    -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
     0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
    -0.5f,  0.5f,  1.0f, 1.0f, 1.0f,
     0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
  };

  ElementBuffer rect_elements_{ {0, 1, 2, 3}, BufferUsage::STATIC_DRAW };
  VertexArray rect_;
  VertexArray rect_texture_;

  Program program_screen_color_{ "..\\src\\shader", "screen_color" };
  Program program_screen_texture_{ "..\\src\\shader", "screen_texture" };

  geom::Texture<unsigned char> texture_{ 2, 2, 4 };
  TextureObject2D<unsigned char> tex_object_;

  Logger log_{ "Render2D" };
};
}

#endif // LAZYGL_WINDOW_PAINTER2D_H_
