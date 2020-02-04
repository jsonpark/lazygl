#include "lazygl/utils/convert_gl.h"

#include "lazygl/utils/logger.h"

namespace lazygl
{
namespace gl
{
template <typename T>
GLenum ConvertTypeToGlEnum()
{
  Logger("ConvertTypeToGlEnum") << "Undefined type" << std::endl;
  return 0;
}

template <>
GLenum ConvertTypeToGlEnum<float>()
{
  return GL_FLOAT;
}

template <>
GLenum ConvertTypeToGlEnum<unsigned int>()
{
  return GL_UNSIGNED_INT;
}
}
}
