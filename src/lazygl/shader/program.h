#ifndef LAZYGL_SHADER_PROGRAM_H_
#define LAZYGL_SHADER_PROGRAM_H_

#include <initializer_list>
#include <vector>

#include "lazygl/shader/shader.h"
#include "lazygl/types.h"

namespace lazygl
{
class Program
{
public:
  // Empty shader program
  Program();

  // Load shaders with extensions .vert, .frag, .geom
  Program(const std::string& path, const std::string& name);

  ~Program();

  // Copy constructors are deleted
  Program(const Program& rhs) = delete;

  Program& operator = (const Program& rhs) = delete;

  // Move constructors
  Program(Program&& rhs) noexcept;

  Program& operator = (Program&& rhs) noexcept;


  void Create();

  auto Id() const noexcept
  {
    return id_;
  }

  void Use();

  void Uniform1f(const std::string& name, float v);
  void Uniform1i(const std::string& name, int i);
  void Uniform2f(const std::string& name, const Vector2f& v);
  void Uniform2i(const std::string& name, int i0, int i1);
  void Uniform3f(const std::string& name, const Vector3f& v);
  void Uniform4f(const std::string& name, const Vector4f& v);
  void UniformMatrix3f(const std::string& name, const Matrix3f& m);
  void UniformMatrix4f(const std::string& name, const Matrix4f& m);

private:
  void Load(const std::string& path, const std::string& name);

  bool created_ = false;

  std::vector<Shader> shaders_;

  GLuint id_;
};
}

#endif // LAZYGL_SHADER_PROGRAM_H_
