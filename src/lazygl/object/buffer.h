#ifndef LAZYGL_OBJECT_BUFFER_H_
#define LAZYGL_OBJECT_BUFFER_H_

#include <vector>

#include <glad/glad.h>

#include "lazygl/utils/convert_gl.h"

namespace lazygl
{
enum class BufferType
{
  ARRAY_BUFFER = GL_ARRAY_BUFFER,
  ELEMENT_ARRAY_BUFFER = GL_ELEMENT_ARRAY_BUFFER,
};

enum class BufferUsage
{
  STATIC_DRAW = GL_STATIC_DRAW,
  DYNAMIC_DRAW = GL_DYNAMIC_DRAW,
  STREAM_DRAW = GL_STREAM_DRAW
};

class BufferBase
{
public:
  BufferBase() = default;

  ~BufferBase()
  {
    if (generated_)
      glDeleteBuffers(1, &id_);
  }

  // Copy constructors
  void Copy(const BufferBase& rhs)
  {
    usage_ = rhs.usage_;

    if (generated_)
    {
      Generate();
      buffer_size_ = 0;
    }
  }

  BufferBase(const BufferBase& rhs)
  {
    Copy(rhs);
  }

  BufferBase& operator = (const BufferBase& rhs)
  {
    Copy(rhs);
    return *this;
  }

  // Move constructors
  void Move(BufferBase&& rhs) noexcept
  {
    usage_ = rhs.usage_;
    generated_ = rhs.generated_;
    id_ = rhs.id_;
    buffer_size_ = rhs.buffer_size_;

    rhs.usage_ = BufferUsage::STATIC_DRAW;
    rhs.generated_ = false;
    rhs.id_ = 0;
    rhs.buffer_size_ = 0;
  }

  BufferBase(BufferBase&& rhs) noexcept
  {
    Move(std::move(rhs));
  }

  BufferBase& operator = (BufferBase&& rhs) noexcept
  {
    Move(std::move(rhs));
    return *this;
  }


  virtual void Update() = 0;
  virtual int InByte(int num_elements) const noexcept = 0;
  virtual GLenum GlType() const noexcept= 0;


  void Bind(BufferType type)
  {
    if (!generated_)
      Generate();

    glBindBuffer(static_cast<int>(type), id_);
  }

  void Unbind(BufferType type)
  {
    glBindBuffer(static_cast<int>(type), 0);
  }

protected:
  void Generate()
  {
    if (!generated_)
    {
      glGenBuffers(1, &id_);
      usage_ = BufferUsage::STATIC_DRAW;
      generated_ = true;
      buffer_size_ = 0;
    }
  }

  void Data(BufferType type, int size, void* ptr, BufferUsage usage)
  {
    if (!generated_)
      Generate();

    Bind(type);

    if (buffer_size_ < size)
    {
      glBufferData(static_cast<GLenum>(type), size, ptr, static_cast<GLenum>(usage));
      buffer_size_ = size;
      usage_ = usage;
    }
    else
    {
      glBufferSubData(static_cast<GLenum>(type), 0, size, ptr);
    }

    Unbind(type);
  }

private:
  BufferUsage usage_ = BufferUsage::STATIC_DRAW;

  bool generated_ = false;
  GLuint id_ = 0;
  int buffer_size_ = 0;
};

template <typename T, BufferType type>
class Buffer : public BufferBase
{
private:
  // Reference class
  class Ref
  {
  public:
    Ref() = delete;

    Ref(Buffer& buffer, int idx)
      : buffer_(buffer), idx_(idx)
    {
    }

    Ref& operator = (T v)
    {
      buffer_.Modify(idx_, v);
      return *this;
    }

  private:
    Buffer& buffer_;
    int idx_;
  };

public:
  Buffer()
  {
  }

  explicit Buffer(int size)
    : data_(size, static_cast<T>(0))
  {
  }

  Buffer(int size, T value)
    : data_(size, value)
  {
  }

  Buffer(std::initializer_list<T> values)
    : data_(values)
  {
  }

  Buffer(int size, BufferUsage usage)
    : data_(size, static_cast<T>(0)), usage_(usage)
  {
  }

  Buffer(int size, T value, BufferUsage usage)
    : data_(size, value), usage_(usage)
  {
  }

  Buffer(std::initializer_list<T> values, BufferUsage usage)
    : data_(values), usage_(usage)
  {
  }

  ~Buffer()
  {
  }

  // Copy constructors
  void Copy(const Buffer& rhs)
  {
    data_ = rhs.data_;
    usage_ = rhs.usage_;
    need_update_ = false;
  }

  Buffer(const Buffer& rhs)
  {
    BufferBase::Copy(rhs);
    Copy(rhs);
  }

  Buffer& operator = (const Buffer& rhs)
  {
    BufferBase::Copy(rhs);
    Copy(rhs);

    return *this;
  }

  // Move constructors
  void Move(Buffer&& rhs)
  {
    data_ = std::move(rhs.data_);
    usage_ = rhs.usage_;
    need_update_ = rhs.need_update_;

    rhs.usage_ = BufferUsage::STATIC_DRAW;
    rhs.need_update_ = false;
  }

  Buffer(Buffer&& rhs)
  {
    BufferBase::Move(std::move(rhs));
    Move(std::move(rhs));
  }

  Buffer& operator = (Buffer&& rhs)
  {
    BufferBase::Move(std::move(rhs));
    Move(std::move(rhs));

    return *this;
  }


  int Size() const
  {
    return static_cast<int>(data_.size());
  }

  void Resize(int n)
  {
    Resize(n, static_cast<T>(0));
  }

  void Resize(int n, T v)
  {
    if (data_.size() != n)
    {
      data_.resize(n, v);
      need_update_ = true;
    }
  }

  Buffer& operator = (const std::vector<T>& data)
  {
    data_ = data;
    need_update_ = true;

    return *this;
  }

  Buffer& operator = (std::vector<T>&& data)
  {
    data_ = std::move(data);
    need_update_ = true;

    return *this;
  }


  // Index operator
  Ref operator [] (int idx)
  {
    return Ref(*this, idx);
  }


  void Update() override
  {
    if (need_update_)
    {
      Data(type, static_cast<int>(data_.size()) * sizeof(T), data_.data(), usage_);
      need_update_ = false;
    }
  }

  int InByte(int num_elements) const noexcept override
  {
    return num_elements * sizeof(T);
  }

  GLenum GlType() const noexcept override
  {
    return gl::ConvertTypeToGlEnum<T>();
  }

  void Modify(int idx, T v)
  {
    data_[idx] = v;
    need_update_ = true;
  }

private:
  std::vector<T> data_;

  BufferUsage usage_ = BufferUsage::STATIC_DRAW;

  bool need_update_ = true;
};

template <typename T>
using ArrayBuffer = Buffer<T, BufferType::ARRAY_BUFFER>;

using ElementBuffer = Buffer<unsigned int, BufferType::ELEMENT_ARRAY_BUFFER>;
}

#endif // LAZYGL_OBJECT_BUFFER_H_
