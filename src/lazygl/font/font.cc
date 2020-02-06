#include "lazygl/font/font.h"

#include <locale>
#include <codecvt>

namespace lazygl
{
Font::Font()
  : Font("ariblk")
{
}

Font::Font(const std::string& font_name)
  : Font("C:\\Windows\\Fonts", font_name)
{
}

Font::Font(const std::string& font_directory, const std::string& font_name)
  : font_directory_(font_directory), font_name_(font_name)
{
  if (FT_Init_FreeType(&ft_))
  {
    log_ << "Could not init FreeType Library" << Logger::endl;
    return;
  }
  
  if (FT_New_Face(ft_, (font_directory_ + "\\" + font_name_ + ".ttf").c_str(), 0, &face_))
    log_ << "Failed to load font" << Logger::endl;

  FT_Set_Pixel_Sizes(face_, 0, 64);
}

Font::~Font()
{
  FT_Done_Face(face_);
  FT_Done_FreeType(ft_);
}

Glyph Font::operator () (char x, int height_pixels)
{
  return (*this)(static_cast<wchar_t>(x));
}

Glyph Font::operator () (wchar_t x, int height_pixels)
{
  FT_Set_Pixel_Sizes(face_, 0, height_pixels);
  FT_Load_Char(face_, x, FT_LOAD_RENDER);
  return Glyph(face_->glyph);
}

std::vector<Glyph> Font::operator () (const std::string& s, int height_pixels)
{
  std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
  return (*this)(converter.from_bytes(s));
}

std::vector<Glyph> Font::operator () (const std::wstring& s, int height_pixels)
{
  FT_Set_Pixel_Sizes(face_, 0, height_pixels);
  std::vector<Glyph> result;

  for (auto c : s)
    result.emplace_back((*this)(c));

  return result;
}
}
