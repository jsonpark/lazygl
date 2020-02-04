#ifndef LAZYGL_UTILS_CONVERT_GL_H_
#define LAZYGL_UTILS_CONVERT_GL_H_

#include <glad/glad.h>

namespace lazygl
{
namespace gl
{
template <typename T>
GLenum ConvertTypeToGlEnum();

template <>
GLenum ConvertTypeToGlEnum<float>();

template <>
GLenum ConvertTypeToGlEnum<unsigned int>();
}
}

#endif // LAZYGL_UTILS_CONVERT_GL_H_
