#ifndef LAZYGL_FONT_FONT_H_
#define LAZYGL_FONT_FONT_H_

#include <string>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "lazygl/utils/logger.h"
#include "lazygl/font/glyph.h"

namespace lazygl
{
class Font
{
public:
  Font();

  explicit Font(const std::string& font_filename);

  Font(const std::string& font_directory, const std::string& font_filename);

  ~Font();

  Glyph operator () (char x, int height_pixels = 64);
  Glyph operator () (wchar_t x, int height_pixels = 64);
  std::vector<Glyph> operator () (const std::string& s, int height_pixels = 64);
  std::vector<Glyph> operator () (const std::wstring& s, int height_pixels = 64);

  int HeightInPixels(double font_size);

private:
  std::string font_directory_;
  std::string font_name_;

  FT_Library ft_ = nullptr;
  FT_Face face_ = nullptr;

  Logger log_{ "Font" };
};
}

#endif // LAZYGL_FONT_FONT_H_
