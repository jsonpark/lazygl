#ifndef LAZYGL_WINDOW_RENDERER_H_
#define LAZYGL_WINDOW_RENDERER_H_

#include "lazygl/window/window.h"

#include "lazygl/object/buffer.h"
#include "lazygl/object/vertex_array.h"
#include "lazygl/shader/program.h"
#include "lazygl/geom/texture.h"
#include "lazygl/object/texture_object.h"
#include "lazygl/object/mesh_object.h"
#include "lazygl/font/font.h"

namespace lazygl
{
class Renderer : public Window
{
private:
  using Textures = std::unordered_map<std::string, geom::Texture<unsigned char>>;
  using TextureObjects = std::unordered_map<std::string, TextureObject2D<unsigned char>>;

  using MeshObjects = std::unordered_map<std::string, MeshObject>;

public:
  Renderer();

  Renderer(int width, int height);

  Renderer(int width, int height, const std::string& title);

  ~Renderer();

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
    rect_buffer_[1 * stride + 2] = 1.f;
    rect_buffer_[1 * stride + 3] = 0.f;

    rect_buffer_[2 * stride + 0] = static_cast<float>(x);
    rect_buffer_[2 * stride + 1] = static_cast<float>(y + height);
    rect_buffer_[2 * stride + 2] = 0.f;
    rect_buffer_[2 * stride + 3] = 1.f;

    rect_buffer_[3 * stride + 0] = static_cast<float>(x + width);
    rect_buffer_[3 * stride + 1] = static_cast<float>(y + height);
    rect_buffer_[3 * stride + 2] = 1.f;
    rect_buffer_[3 * stride + 3] = 1.f;

    program_screen_texture_.Use();
    texture_object.Bind();
    rect_texture_.Draw();
    texture_object.Unbind();
  }

  template <typename T>
  void DrawTexture1DHorizontal(double x, double y, double width, double height, TextureObject1D<T>& texture_object)
  {
    constexpr int stride = 3;

    rect_buffer_[0 * stride + 0] = static_cast<float>(x);
    rect_buffer_[0 * stride + 1] = static_cast<float>(y);
    rect_buffer_[0 * stride + 2] = 0.f;

    rect_buffer_[1 * stride + 0] = static_cast<float>(x + width);
    rect_buffer_[1 * stride + 1] = static_cast<float>(y);
    rect_buffer_[1 * stride + 2] = 1.f;

    rect_buffer_[2 * stride + 0] = static_cast<float>(x);
    rect_buffer_[2 * stride + 1] = static_cast<float>(y + height);
    rect_buffer_[2 * stride + 2] = 0.f;

    rect_buffer_[3 * stride + 0] = static_cast<float>(x + width);
    rect_buffer_[3 * stride + 1] = static_cast<float>(y + height);
    rect_buffer_[3 * stride + 2] = 1.f;

    program_screen_texture_1d_horizontal_.Use();
    line_tex_object_.Bind();
    rect_texture_1d_.Draw();
    line_tex_object_.Unbind();
  }

  void RenderText(double x, double y, double font_size, const std::string& s, const Vector3f& color = Vector3f(0.f, 0.f, 0.f));
  void RenderText(double x, double y, double font_size, const std::wstring& s, const Vector3f& color = Vector3f(0.f, 0.f, 0.f));
  // When font height in pixel <= 64
  void RenderTextGrid(double x, double y, double font_size, const std::wstring& s, const Vector3f& color = Vector3f(0.f, 0.f, 0.f));

  // Texture size is always 64x64, while glyph size is smaller.
  void DrawGlyphTexture(double x, double y, double width, double height, const Vector2i& glyph_size, TextureObject2D<unsigned char>& glyph);
  void DrawGlyphTextureGrid(double x, double y, double width, double height, const Vector2i& glyph_size, TextureObject2D<unsigned char>& glyph);


private:
  ArrayBuffer<float> rect_buffer_{ {
    -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
     0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
    -0.5f,  0.5f,  1.0f, 1.0f, 1.0f,
     0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
     }, BufferUsage::DYNAMIC_DRAW
  };

  ElementBuffer rect_elements_{ {0, 1, 2, 3}, BufferUsage::STATIC_DRAW };
  VertexArray rect_;
  VertexArray rect_texture_;
  VertexArray rect_font_grid_;
  VertexArray rect_texture_1d_;

  Program program_screen_color_{ "..\\src\\shader", "screen_color" };
  Program program_screen_texture_{ "..\\src\\shader", "screen_texture" };
  Program program_screen_texture_1d_horizontal_{ "..\\src\\shader", "screen_texture_1d_horizontal" };
  Program program_screen_font_{ "..\\src\\shader", "screen_font" };
  Program program_screen_font_grid_{ "..\\src\\shader", "screen_font_grid" };
  Program program_mesh_{ "..\\src\\shader", "mesh" };

  geom::Texture<unsigned char> texture_{ 2, 2, 4 };
  TextureObject2D<unsigned char> tex_object_;

  geom::Texture1D<unsigned char> line_texture_{ 10, 4 };
  TextureObject1D<unsigned char> line_tex_object_;

  geom::Texture<unsigned char> glyph_texture_{ 64, 64, 1 };
  TextureObject2D<unsigned char> glyph_object_;

  //Font arial_{ "C:\\Windows\\Fonts", "malgun" };
  Font arial_{ "C:\\Windows\\Fonts", "consola" };

  Textures textures_;
  TextureObjects texture_objects_;

  MeshObjects meshes_;

  Logger log_{ "Render2D" };
};
}

#endif // LAZYGL_WINDOW_RENDERER_H_
