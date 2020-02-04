#ifndef LAZYGL_OBJECT_TEXTURE_OBJECT_H_
#define LAZYGL_OBJECT_TEXTURE_OBJECT_H_

#include <glad/glad.h>

#include "lazygl/geom/texture.h"

namespace lazygl
{
class TextureObject
{
public:
  enum class Wrap
  {
    CLAMP_TO_EDGE = GL_CLAMP_TO_EDGE,
    CLAMP_TO_BORDER = GL_CLAMP_TO_BORDER,
    MIRRORED_REPEAT = GL_MIRRORED_REPEAT,
    REPEAT = GL_REPEAT,
    MIRROR_CLAMP_TO_EDGE = GL_MIRROR_CLAMP_TO_EDGE,
  };

  enum class MinFilter
  {
    NEAREST = GL_NEAREST,
    LINEAR = GL_LINEAR,
    NEAREST_MIPMAP_NEAREST = GL_NEAREST_MIPMAP_NEAREST,
    LINEAR_MIPMAP_NEAREST = GL_LINEAR_MIPMAP_NEAREST,
    NEAREST_MIPMAP_LINEAR = GL_NEAREST_MIPMAP_LINEAR,
    LINEAR_MIPMAP_LINEAR = GL_LINEAR_MIPMAP_LINEAR,
  };

  enum class MagFilter
  {
    NEAREST = GL_NEAREST,
    LINEAR = GL_LINEAR,
  };

protected:
  enum class Type
  {
    UNKNOWN = 0,
    TEXTURE_2D = GL_TEXTURE_2D,
  };

public:
  TextureObject() = delete;

  TextureObject(Type type)
    : type_(type)
  {
  }

  virtual ~TextureObject()
  {
    Delete();
  }

  // Copy constructors
  void Copy(const TextureObject& rhs)
  {
    id_ = rhs.id_;
    need_update_ = rhs.need_update_;
    type_ = rhs.type_;
  }

  TextureObject(const TextureObject& rhs)
  {
    Copy(rhs);
  }

  TextureObject& operator = (const TextureObject& rhs)
  {
    Copy(rhs);
    return *this;
  }

  // Move constructors
  void Move(TextureObject&& rhs) noexcept
  {
    id_ = rhs.id_;
    need_update_ = rhs.need_update_;
    type_ = rhs.type_;

    rhs.id_ = 0;
    rhs.need_update_ = false;
    rhs.type_ = Type::UNKNOWN;
  }


  TextureObject(TextureObject&& rhs) noexcept
  {
    Move(std::move(rhs));
  }

  TextureObject& operator = (TextureObject&& rhs) noexcept
  {
    Move(std::move(rhs));
    return *this;
  }


  void Bind()
  {
    if (!generated_)
      Generate();

    glBindTexture(static_cast<GLenum>(type_), id_);
  }

  void Unbind()
  {
    glBindTexture(static_cast<GLenum>(type_), 0);
  }

  void WrapS(Wrap wrap)
  {
    Bind();
    glTexParameteri(static_cast<GLenum>(type_), GL_TEXTURE_WRAP_S, static_cast<GLint>(wrap));
    Unbind();
  }

  void WrapT(Wrap wrap)
  {
    Bind();
    glTexParameteri(static_cast<GLenum>(type_), GL_TEXTURE_WRAP_T, static_cast<GLint>(wrap));
    Unbind();
  }

  void MinFilter(typename MinFilter filter)
  {
    Bind();
    glTexParameteri(static_cast<GLenum>(type_), GL_TEXTURE_MIN_FILTER, static_cast<GLint>(filter));
    Unbind();
  }

  void MagFilter(typename MagFilter filter)
  {
    Bind();
    glTexParameteri(static_cast<GLenum>(type_), GL_TEXTURE_MAG_FILTER, static_cast<GLint>(filter));
    Unbind();
  }

  void Generate()
  {
    if (!generated_)
    {
      glGenTextures(1, &id_);
      generated_ = true;
    }
  }

  void Delete()
  {
    if (generated_)
    {
      glDeleteTextures(1, &id_);
      generated_ = false;
    }
  }

protected:
  auto GetType() const
  {
    return type_;
  }

private:
  bool generated_ = false;
  GLuint id_ = 0;
  Type type_ = Type::UNKNOWN;

  bool need_update_ = false;

  Logger log_{ "TextureObject" };
};


template <typename T>
class TextureObject2D : public TextureObject
{
public:
  TextureObject2D()
    : TextureObject(Type::TEXTURE_2D)
  {
  }

  ~TextureObject2D() override = default;

  void Update(const geom::Texture<T>& texture)
  {
    GLenum internal_format = GL_RGBA;
    GLenum format = GL_RGBA;
    GLenum type = GL_UNSIGNED_BYTE;
    switch (texture.NumComponents())
    {
    case 1:
      internal_format = GL_RED;
      format = GL_RED;
      break;

    case 2:
      internal_format = GL_RG;
      format = GL_RG;
      break;

    case 3:
      internal_format = GL_RGB;
      format = GL_RGB;
      break;

    case 4:
      internal_format = GL_RGBA;
      format = GL_RGBA;
      break;

    default:
      break;
    }

    Bind();
    glTexImage2D(static_cast<GLenum>(GetType()), 0, internal_format, texture.Width(), texture.Height(), 0, format, type, texture.Data().data());
    Unbind();

    WrapS(Wrap::REPEAT);
    WrapT(Wrap::REPEAT);
    MinFilter(MinFilter::LINEAR_MIPMAP_LINEAR);
    MagFilter(MagFilter::LINEAR);

    Bind();
    glGenerateMipmap(static_cast<GLenum>(GetType()));
    Unbind();
  }

private:
};
}

#endif // LAZYGL_OBJECT_MESH_OBJECT_H_
