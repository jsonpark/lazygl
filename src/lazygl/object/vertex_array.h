#ifndef LAZYGL_OBJECT_VERTEX_ARRAY_H_
#define LAZYGL_OBJECT_VERTEX_ARRAY_H_

#include <glad/glad.h>

#include "lazygl/object/buffer.h"
#include "lazygl/utils/convert_gl.h"
#include "lazygl/utils/logger.h"

namespace lazygl
{
class VertexArray
{
public:
  enum class DrawMode
  {
    POINTS = GL_POINTS,
    LINES = GL_LINES,
    TRIANGLES = GL_TRIANGLES,
    TRIANGLE_STRIP = GL_TRIANGLE_STRIP,
    TRIANGLE_FAN = GL_TRIANGLE_FAN,
  };

public:
  VertexArray();
  ~VertexArray();

  // Copy constructors
  VertexArray(const VertexArray& rhs) = delete;
  VertexArray& operator = (const VertexArray& rhs) = delete;

  // Move constructors
  void Move(VertexArray&& rhs) noexcept;
  VertexArray(VertexArray&& rhs) noexcept;
  VertexArray& operator = (VertexArray&& rhs) noexcept;


  void Generate();
  void Delete();

  void Bind();
  void Unbind();

  template <typename T>
  void AttribPointer(int index, int size, ArrayBuffer<T>& buffer, int stride = 0, int offset = 0)
  {
    attribs_.emplace_back(index, size, buffer, stride, offset);
    attribs_commanded_ = false;
  }

  void Indices(DrawMode mode, int count);
  void Indices(DrawMode mode, ElementBuffer& buffer, int count = -1);

  void Draw();

private:
  void AttribPointerLazy();
  void BindElementBufferLazy();
  void UpdateBuffers();

  bool generated_ = false;
  GLuint id_ = 0;

  bool has_element_buffer_ = false;
  bool element_buffer_bound_ = false;
  int num_elements_ = 0;
  ElementBuffer* element_buffer_ = nullptr;

  struct Attrib
  {
    std::reference_wrapper<BufferBase> buffer;
    int index;
    int size;
    int stride;
    int offset;

    Attrib() = delete;

    Attrib(int index, int size, BufferBase& buffer, int stride, int offset)
      : index(index), size(size), buffer(buffer), stride(stride), offset(offset)
    {
    }
  };

  std::vector<Attrib> attribs_;
  bool attribs_commanded_ = false;

  DrawMode mode_ = DrawMode::POINTS;

  Logger log_{ "VertexArray" };
};
}

#endif // LAZYGL_OBJECT_VERTEX_ARRAY_H_
