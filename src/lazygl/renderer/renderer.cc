#include "lazygl/renderer/renderer.h"

#include "lazygl/renderer/renderer_split_widget-inl.h"
#include "lazygl/renderer/renderer_text_widget-inl.h"
#include "lazygl/renderer/renderer_pushbutton_widget-inl.h"

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
  // (0, 0) at the left-top corner

  if (mouse_active_widget_ == nullptr)
  {
    WidgetMouseButton(widget_, button, action, x, y);
  }

  else
  {
    auto current_widget = mouse_active_widget_;
    mouse_active_widget_ = nullptr;

    const auto& root_pos = current_widget->GetRootPos();

    WidgetMouseButton(current_widget, button, action, x - root_pos(0), y - root_pos(1));
  }
}

void Renderer::MouseMove(double x, double y)
{
  if (mouse_active_widget_ == nullptr)
  {
    WidgetMouseMove(widget_, x, y);
  }

  else
  {
    const auto& root_pos = mouse_active_widget_->GetRootPos();
    WidgetMouseMove(mouse_active_widget_, x - root_pos(0), y - root_pos(1));
  }
}

void Renderer::Keyboard(input::Key key, input::KeyAction action)
{
  // TODO
  //WidgetKeyboard(widget_, key, action);
}

template <>
void Renderer::WidgetMouseButton(std::shared_ptr<Widget> widget, input::MouseButton button, input::MouseAction action, double x, double y)
{
  if (auto hs = std::dynamic_pointer_cast<HorizontalSplit>(widget))
    WidgetMouseButton(hs, button, action, x, y);

  if (auto vs = std::dynamic_pointer_cast<VerticalSplit>(widget))
    WidgetMouseButton(vs, button, action, x, y);

  if (auto pw = std::dynamic_pointer_cast<PushbuttonWidget>(widget))
    WidgetMouseButton(pw, button, action, x, y);
}

template <>
void Renderer::WidgetMouseMove(std::shared_ptr<Widget> widget, double x, double y)
{
  if (auto hs = std::dynamic_pointer_cast<HorizontalSplit>(widget))
    WidgetMouseMove(hs, x, y);

  if (auto vs = std::dynamic_pointer_cast<VerticalSplit>(widget))
    WidgetMouseMove(vs, x, y);
}

template <>
void Renderer::WidgetDraw(std::shared_ptr<Widget> widget)
{
  const auto& size = widget->GetSize();

  if (auto hs = std::dynamic_pointer_cast<HorizontalSplit>(widget))
    WidgetDraw(hs);

  if (auto vs = std::dynamic_pointer_cast<VerticalSplit>(widget))
    WidgetDraw(vs);

  if (auto tw = std::dynamic_pointer_cast<TextWidget>(widget))
    WidgetDraw(tw);

  if (auto pw = std::dynamic_pointer_cast<PushbuttonWidget>(widget))
    WidgetDraw(pw);

  for (auto child : widget->GetChildren())
  {
    const auto& pos = child->GetPos();
    WidgetDraw(child);
  }
}

void Renderer::Resize(int width, int height)
{
  Window::Resize(width, height);

  widget_->SetSize(width, height);
  widget_->UpdatePosSize();
}

void Renderer::Initialize()
{
  glClearColor(1.f, 1.f, 1.f, 1.f);
  glEnable(GL_MULTISAMPLE);
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

  rect_font_grid_.AttribPointer(0, 2, rect_buffer_);
  rect_font_grid_.Indices(VertexArray::DrawMode::TRIANGLE_STRIP, rect_elements_);

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

  glyph_object_.WrapS(TextureObject::Wrap::MIRRORED_REPEAT);
  glyph_object_.WrapT(TextureObject::Wrap::MIRRORED_REPEAT);
  glyph_object_.MinFilter(TextureObject::MinFilter::NEAREST);
  glyph_object_.MagFilter(TextureObject::MagFilter::NEAREST);
  glyph_object_.UpdateFilters();

  geom::Mesh mesh("..\\..\\fetch_ros\\fetch_description\\meshes\\base_link.dae");
  textures_["base"].LoadAsync(mesh.GetTextureFilename());
  meshes_["base"] = std::move(mesh);

  auto split0 = std::make_shared<VerticalSplit>();
  auto split1 = std::make_shared<VerticalSplit>();
  auto split2 = std::make_shared<HorizontalSplit>();
  auto split3 = std::make_shared<HorizontalSplit>();
  auto split4 = std::make_shared<VerticalSplit>();
  auto split5 = std::make_shared<HorizontalSplit>();
  auto split_main = std::make_shared<HorizontalSplit>();

  auto push20 = std::make_shared<PushbuttonWidget>("Push Button 2_0");
  auto push21 = std::make_shared<PushbuttonWidget>("Push Button 2_1");
  auto push30 = std::make_shared<PushbuttonWidget>("Push Button 3_0");
  auto push31 = std::make_shared<PushbuttonWidget>("Push Button 3_1");
  auto push40 = std::make_shared<PushbuttonWidget>("Push Button 4_0");
  auto push41 = std::make_shared<PushbuttonWidget>("Push Button 4_1");
  auto push50 = std::make_shared<PushbuttonWidget>("Push Button 5_0");
  auto push51 = std::make_shared<PushbuttonWidget>("Push Button 5_1");

  split_main->AttachWidget(split0, split1);
  split0->AttachWidget(split2, split4);
  split1->AttachWidget(split3, split5);
  split2->AttachWidget(push20, push21);
  split3->AttachWidget(push30, push31);
  split4->AttachWidget(push40, push41);
  split5->AttachWidget(push50, push51);

  widget_ = split_main;
}

void Renderer::PrepareDraw()
{
  program_screen_color_.Uniform2i("screen", Width(), Height());
  program_screen_texture_.Uniform2i("screen", Width(), Height());
  program_screen_texture_1d_horizontal_.Uniform2i("screen", Width(), Height());
  program_screen_font_.Uniform2i("screen", Width(), Height());
  program_screen_font_grid_.Uniform2i("screen", Width(), Height());
}

void Renderer::Draw()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glViewport(0, 0, Width(), Height());

  widget_->SetPos(0., 0.);
  widget_->SetSize(Width(), Height());
  widget_->UpdatePosSize();

  WidgetDraw(widget_);

  /*
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
  */
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
  // Convert font_size from point to pixel
  int glyph_height = arial_.HeightInPixels(font_size);
  if (glyph_height <= 64)
  {
    RenderTextGrid(x, y, font_size, s, color);
    return;
  }

  program_screen_font_.Uniform3f("color", color);

  double scale = static_cast<double>(glyph_height) / 64.;
  double line_height = 64.;

  double start_x = x;

  y -= line_height * scale;

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

void Renderer::RenderTextGrid(double x, double y, double font_size, const std::wstring& s, const Vector3f& color)
{
  // Convert font_size from point to pixel
  int glyph_height = arial_.HeightInPixels(font_size);
  if (glyph_height > 64)
  {
    RenderText(x, y, font_size, s, color);
    return;
  }

  program_screen_font_grid_.Uniform3f("color", color);

  // TODO: line width
  double line_height = static_cast<double>(glyph_height);

  // Round (x, y) to integer
  x = std::round(x);
  y = std::round(y);

  y -= line_height;

  double start_x = x;

  for (auto c : s)
  {
    if (c == L'\n')
    {
      x = start_x;
      y -= line_height;
      y = std::round(y);
    }

    else
    {
      const auto& glyph = arial_(c, glyph_height);

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

      double width = size(0);
      double height = size(1);

      DrawGlyphTextureGrid(x + bearing(0), y + bearing(1) - height, width, height, size, glyph_object_);

      x += (advance >> 6);
      x = std::round(x);
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

void Renderer::DrawGlyphTextureGrid(double x, double y, double width, double height, const Vector2i& glyph_size, TextureObject2D<unsigned char>& glyph)
{
  glDisable(GL_DEPTH_TEST);

  x = std::round(x);
  y = std::round(y);

  constexpr int stride = 2;

  rect_buffer_[0 * stride + 0] = static_cast<float>(x);
  rect_buffer_[0 * stride + 1] = static_cast<float>(y);

  rect_buffer_[1 * stride + 0] = static_cast<float>(x + width);
  rect_buffer_[1 * stride + 1] = static_cast<float>(y);

  rect_buffer_[2 * stride + 0] = static_cast<float>(x);
  rect_buffer_[2 * stride + 1] = static_cast<float>(y + height);

  rect_buffer_[3 * stride + 0] = static_cast<float>(x + width);
  rect_buffer_[3 * stride + 1] = static_cast<float>(y + height);

  program_screen_font_grid_.Uniform2i("left_bottom", static_cast<int>(x), static_cast<int>(y));
  program_screen_font_grid_.Use();
  glyph.Bind();
  rect_font_grid_.Draw();
  glyph.Unbind();
}
}
