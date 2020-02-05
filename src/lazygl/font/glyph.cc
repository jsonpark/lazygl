#include "lazygl/font/glyph.h"

namespace lazygl
{
Glyph::Glyph(FT_GlyphSlot ft_glyph)
  : size_(ft_glyph->bitmap.width, ft_glyph->bitmap.rows),
  bearing_(ft_glyph->bitmap_left, ft_glyph->bitmap_top),
  advance_(ft_glyph->advance.x)
{
  buffer_.resize(static_cast<size_t>(size_(0)) * size_(1));
  std::copy(ft_glyph->bitmap.buffer, ft_glyph->bitmap.buffer + buffer_.size(), buffer_.data());
}

Glyph::~Glyph() = default;
}
