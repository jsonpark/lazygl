#ifndef LAZYGL_GEOM_TEXTURE_H_
#define LAZYGL_GEOM_TEXTURE_H_

#include <string>
#include <future>

#include "lazygl/types.h"
#include "lazygl/utils/logger.h"
#include "lazygl/utils/async_loader.h"

namespace lazygl
{
namespace geom
{
class TextureBase : public AsyncLoader
{
public:
  TextureBase() = default;
  ~TextureBase() = default;

  // Copy constructors
  void Copy(const TextureBase& rhs)
  {
  }

  TextureBase(const TextureBase& rhs)
    : AsyncLoader(rhs)
  {
    Copy(rhs);
  }

  TextureBase& operator = (const TextureBase& rhs)
  {
    AsyncLoader::Copy(rhs);
    Copy(rhs);
    return *this;
  }

  // Move constructors
  void Move(TextureBase&& rhs) noexcept
  {
  }

  TextureBase(TextureBase&& rhs) noexcept
    : AsyncLoader(std::move(rhs))
  {
    Move(std::move(rhs));
  }

  TextureBase& operator = (TextureBase&& rhs) noexcept
  {
    AsyncLoader::Move(std::move(rhs));
    Move(std::move(rhs));
    return *this;
  }

private:
};

template <typename T>
class Texture : public TextureBase
{
private:
  class Ref
  {
  public:
    Ref() = delete;

    Ref(Texture& texture, int idx)
      : texture_(texture), idx_(idx)
    {
    }

    ~Ref()
    {
    }

    T& R()
    {
      return texture_.data_[idx_ + 0];
    }

    T& G()
    {
      return texture_.data_[idx_ + 1];
    }

    T& B()
    {
      return texture_.data_[idx_ + 2];
    }

    T& A()
    {
      return texture_.data_[idx_ + 3];
    }

    Ref& operator = (const VectorX<T>& v)
    {
      for (int i = 0; i < v.rows(); i++)
        texture_.data_[idx_ + i] = v(i);
      return *this;
    }

    template<int rows>
    Ref& operator = (const Vector<T, rows>& v)
    {
      for (int i = 0; i < v.rows(); i++)
        texture_.data_[idx_ + i] = v(i);
      return *this;
    }

  private:
    Texture& texture_;
    int idx_;
  };

public:
  Texture() = default;

  explicit Texture(const std::string& filename)
  {
    Load(filename);
  }

  Texture(int width, int height, int num_components)
    : width_(width), height_(height), num_components_(num_components),
    data_(width * height * num_components, static_cast<T>(0))
  {
  }

  ~Texture() = default;

  // Copy constructors do not copy the asynchronously loading texture
  void Copy(const Texture& rhs)
  {
    width_ = rhs.width_;
    height_ = rhs.height_;
    num_components_ = rhs.num_components_;
    data_ = rhs.data_;
  }

  Texture(const Texture& rhs)
    : TextureBase(rhs)
  {
    Copy(rhs);
  }

  Texture& operator = (const Texture& rhs)
  {
    TextureBase::Copy(rhs);
    Copy(rhs);
    return *this;
  }

  // Move constructors
  void Move(Texture&& rhs) noexcept
  {
    width_ = rhs.width_;
    height_ = rhs.height_;
    num_components_ = rhs.num_components_;
    data_ = std::move(rhs.data_);

    rhs.width_ = 0;
    rhs.height_ = 0;
    rhs.num_components_ = 0;
  }

  Texture(Texture&& rhs) noexcept
    : TextureBase(std::move(rhs))
  {
    Move(std::move(rhs));
  }

  Texture& operator = (Texture&& rhs) noexcept
  {
    AsyncLoader::Move(std::move(rhs));
    Move(std::move(rhs));
    return *this;
  }

  bool LoadFunc(const std::string& filename) override;

  auto Width() const
  {
    return width_;
  }

  auto Height() const
  {
    return height_;
  }

  auto NumComponents() const
  {
    return num_components_;
  }

  const auto& Data() const
  {
    return data_;
  }

  Ref operator () (int s, int t)
  {
    return Ref(*this, (s + t * width_) * num_components_);
  }

private:
  // left-to-right from (0, 0), bottom-to-top to (width-1, height-1)
  int width_ = 0;
  int height_ = 0;
  int num_components_ = 0;
  std::vector<T> data_;

  Logger log_{ "Texture" };
};

template <>
bool Texture<unsigned char>::LoadFunc(const std::string& filename);

template <>
bool Texture<float>::LoadFunc(const std::string& filename);
}
}

#endif // LAZYGL_GEOM_TEXTURE_H_
