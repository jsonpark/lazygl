#include "lazygl/renderer/renderer.h"

#include <locale>
#include <codecvt>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace lazygl
{
Renderer::Renderer()
  : Renderer(800, 600)
{
}

Renderer::Renderer(int width, int height)
  : Renderer(width, height, "Renderer")
{
}

Renderer::Renderer(int width, int height, const std::string& title)
  : Window(width, height, title)
{
}

Renderer::~Renderer() = default;

void Renderer::MouseButton(input::MouseButton button, input::MouseAction action, double x, double y)
{
}

void Renderer::MouseMove(double x, double y)
{
}

void Renderer::Keyboard(input::Key key, input::KeyAction action)
{
}

void Renderer::Resize(int width, int height)
{
  Window::Resize(width, height);
}

void Renderer::Initialize()
{
  glClearColor(1.f, 1.f, 1.f, 1.f);
  //glEnable(GL_MULTISAMPLE);
  glDisable(GL_MULTISAMPLE);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  rect_.AttribPointer(0, 2, rect_buffer_, 5, 0);
  rect_.AttribPointer(1, 3, rect_buffer_, 5, 2);
  rect_.Indices(VertexArray::DrawMode::TRIANGLE_STRIP, rect_elements_);

  rect_texture_.AttribPointer(0, 2, rect_buffer_, 4, 0);
  rect_texture_.AttribPointer(1, 2, rect_buffer_, 4, 2);
  rect_texture_.Indices(VertexArray::DrawMode::TRIANGLE_STRIP, rect_elements_);

  rect_texture_1d_.AttribPointer(0, 2, rect_buffer_, 3, 0);
  rect_texture_1d_.AttribPointer(1, 1, rect_buffer_, 3, 2);
  rect_texture_1d_.Indices(VertexArray::DrawMode::TRIANGLE_STRIP, rect_elements_);

  texture_(0, 0) = Vector<unsigned char, 4>(255, 255, 255, 255);
  texture_(1, 0) = Vector<unsigned char, 4>(0, 0, 0, 255);
  texture_(0, 1) = Vector<unsigned char, 4>(0, 0, 0, 255);
  texture_(1, 1) = Vector<unsigned char, 4>(255, 255, 255, 255);

  tex_object_.Update(texture_);
  tex_object_.WrapS(TextureObject::Wrap::REPEAT);
  tex_object_.WrapT(TextureObject::Wrap::REPEAT);
  tex_object_.MinFilter(TextureObject::MinFilter::NEAREST);
  tex_object_.MagFilter(TextureObject::MagFilter::NEAREST);
  tex_object_.UpdateFilters();

  for (int i = 0; i < line_texture_.Length(); i++)
  {
    auto t = static_cast<float>(i) / (line_texture_.Length() - 1);
    float r = (1.f - t) * 0.5f + t * 1.f;
    float g = (1.f - t) * 0.5f + t * 1.f;
    float b = (1.f - t) * 0.5f + t * 1.f;

    auto rb = static_cast<unsigned char>(r * 255.f);
    auto gb = static_cast<unsigned char>(g * 255.f);
    auto bb = static_cast<unsigned char>(b * 255.f);

    line_texture_(i) = Vector<unsigned char, 4>(rb, gb, bb, 255);
  }
  line_tex_object_.WrapS(TextureObject::Wrap::CLAMP_TO_EDGE);
  line_tex_object_.MinFilter(TextureObject::MinFilter::NEAREST);
  line_tex_object_.Update(line_texture_);

  glyph_object_.WrapS(TextureObject::Wrap::CLAMP_TO_EDGE);
  glyph_object_.WrapT(TextureObject::Wrap::CLAMP_TO_EDGE);
  glyph_object_.MinFilter(TextureObject::MinFilter::LINEAR);
  glyph_object_.MagFilter(TextureObject::MagFilter::LINEAR);
  glyph_object_.UpdateFilters();

  geom::Mesh mesh("..\\..\\fetch_ros\\fetch_description\\meshes\\base_link.dae");
  textures_["base"].LoadAsync(mesh.GetTextureFilename());
  meshes_["base"] = std::move(mesh);
}

void Renderer::PrepareDraw()
{
  program_screen_color_.Uniform2i("screen", Width(), Height());
  program_screen_texture_.Uniform2i("screen", Width(), Height());
  program_screen_texture_1d_horizontal_.Uniform2i("screen", Width(), Height());
  program_screen_font_.Uniform2i("screen", Width(), Height());
}

void Renderer::Draw()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glViewport(0, 0, Width(), Height());

  glEnable(GL_DEPTH_TEST);
  if (textures_["base"].IsReady())
  {
    textures_["base"].Get();
    texture_objects_["base"].Update(textures_["base"]);
  }

  if (textures_["base"].IsLoaded())
  {
    program_mesh_.Uniform1i("has_texture", 1);
    texture_objects_["base"].Bind();
  }
  else
  {
    program_mesh_.Uniform1i("has_texture", 0);
  }

  program_mesh_.Use();
  meshes_["base"].Draw();

  RenderText(20, 500, 36., "Sample", Vector3f(0.f, 0.f, 0.f));
  RenderText(20, 550, 36., L"æ»≥Á«œººø‰", Vector3f(0.f, 0.f, 0.f));
  RenderText(20, 400, 8., 
    "#include <iostream>\n"
    "\n"
    "int main()\n"
    "{\n"
    "  std::cout << \"Hello World!\" << std::endl;\n"
    "}\n"
    , Vector3f(0.f, 0.f, 0.f));
}

void Renderer::DrawRect(double x, double y, double width, double height, const Vector3f& color)
{
  glDisable(GL_DEPTH_TEST);

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

void Renderer::RenderText(double x, double y, double font_size, const std::string& s, const Vector3f& color)
{
  glDisable(GL_DEPTH_TEST);

  std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
  RenderText(x, y, font_size, converter.from_bytes(s), color);
}

void Renderer::RenderText(double x, double y, double font_size, const std::wstring& s, const Vector3f& color)
{
  glDisable(GL_DEPTH_TEST);

  program_screen_font_.Uniform3f("color", color);

  // TODO: convert font_size to scale
  double scale = font_size / 64. * 4. / 3.;

  // TODO: line width
  constexpr double line_height = 70.;

  double start_x = x;

  for (auto c : s)
  {
    if (c == L'\n')
    {
      x = start_x;
      y -= line_height * scale;
    }

    else
    {
      const auto& glyph = arial_(c);

      const auto& size = glyph.Size();
      const auto& bearing = glyph.Bearing();
      const auto& buffer = glyph.Buffer();
      auto advance = glyph.Advance();

      for (int i = 0; i < 64; i++)
        for (int j = 0; j < 64; j++)
          glyph_texture_(i, j) = 0;

      for (int i = 0; i < size(0); i++)
      {
        for (int j = 0; j < size(1); j++)
        {
          int idx = i + (size(1) - j - 1) * size(0);
          glyph_texture_(i, j) = buffer[idx];
        }
      }
      glyph_object_.Update(glyph_texture_);

      double width = size(0) * scale;
      double height = size(1) * scale;

      DrawGlyphTexture(x + bearing(0) * scale, y + bearing(1) * scale - height, width, height, size, glyph_object_);

      x += (advance >> 6) * scale;
    }
  }
}

void Renderer::DrawGlyphTexture(double x, double y, double width, double height, const Vector2i& glyph_size, TextureObject2D<unsigned char>& glyph)
{
  glDisable(GL_DEPTH_TEST);

  constexpr int stride = 4;

  rect_buffer_[0 * stride + 0] = static_cast<float>(x);
  rect_buffer_[0 * stride + 1] = static_cast<float>(y);
  rect_buffer_[0 * stride + 2] = 0.f;
  rect_buffer_[0 * stride + 3] = 0.f;

  rect_buffer_[1 * stride + 0] = static_cast<float>(x + width);
  rect_buffer_[1 * stride + 1] = static_cast<float>(y);
  rect_buffer_[1 * stride + 2] = static_cast<float>(glyph_size(0)) / 64.f;
  rect_buffer_[1 * stride + 3] = 0.f;

  rect_buffer_[2 * stride + 0] = static_cast<float>(x);
  rect_buffer_[2 * stride + 1] = static_cast<float>(y + height);
  rect_buffer_[2 * stride + 2] = 0.f;
  rect_buffer_[2 * stride + 3] = static_cast<float>(glyph_size(1)) / 64.f;

  rect_buffer_[3 * stride + 0] = static_cast<float>(x + width);
  rect_buffer_[3 * stride + 1] = static_cast<float>(y + height);
  rect_buffer_[3 * stride + 2] = static_cast<float>(glyph_size(0)) / 64.f;
  rect_buffer_[3 * stride + 3] = static_cast<float>(glyph_size(1)) / 64.f;

  program_screen_font_.Use();
  glyph.Bind();
  rect_texture_.Draw();
  glyph.Unbind();
}
}
