#ifndef LAZYGL_GEOMETRY_MESH_H_
#define LAZYGL_GEOMETRY_MESH_H_

#include <string>
#include <vector>

#include "lazygl/utils/async_loader.h"

namespace lazygl
{
namespace geom
{
class Mesh : public AsyncLoader
{
public:
  Mesh();

  explicit Mesh(const std::string& filename);

  ~Mesh();

  // Copy constructors do not copy the asynchronously loading texture
  void Copy(const Mesh& rhs);
  Mesh(const Mesh& rhs);
  Mesh& operator = (const Mesh& rhs);

  // Move constructors
  void Move(Mesh&& rhs) noexcept;
  Mesh(Mesh&& rhs) noexcept;
  Mesh& operator = (Mesh&& rhs) noexcept;

  bool LoadFunc(const std::string& filename) override;

  auto& GetPositions()
  {
    return positions_;
  }

  const auto& GetPositions() const
  {
    return positions_;
  }

  auto& GetNormals()
  {
    return normals_;
  }

  const auto& GetNormals() const
  {
    return normals_;
  }

  auto& GetTexCoords()
  {
    return tex_coords_;
  }

  const auto& GetTexCoords() const
  {
    return tex_coords_;
  }

  auto& GetIndices()
  {
    return indices_;
  }

  const auto& GetIndices() const
  {
    return indices_;
  }

  auto& GetTextureFilename()
  {
    return texture_filename_;
  }

  const auto& GetTextureFilename() const
  {
    return texture_filename_;
  }

private:
  std::vector<float> positions_;
  std::vector<float> normals_;
  std::vector<float> tex_coords_;
  std::vector<unsigned int> indices_;
  std::string texture_filename_;

  Logger log_{ "Mesh" };
};
}
}

#endif // LAZYGL_GEOMETRY_MESH_H_
