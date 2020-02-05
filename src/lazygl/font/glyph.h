#ifndef LAZYGL_FONT_GLYPH_H_
#define LAZYGL_FONT_GLYPH_H_

#include <string>
#include <vector>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "lazygl/types.h"

namespace lazygl
{
class Glyph
{
public:
  Glyph() = delete;

  explicit Glyph(FT_GlyphSlot ft_glyph);

  ~Glyph();

  const auto& Size() const
  {
    return size_;
  }

  const auto& Bearing() const
  {
    return bearing_;
  }

  auto Advance() const
  {
    return advance_;
  }

  const auto& Buffer() const
  {
    return buffer_;
  }

private:
  Vector2i size_{ 0, 0 };
  Vector2i bearing_{ 0, 0 };
  int advance_ = 0;
  std::vector<unsigned char> buffer_;
};
}

#endif // LAZYGL_FONT_GLYPH_H_
