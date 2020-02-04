#ifndef LAZYGL_OBJECT_MESH_OBJECT_H_
#define LAZYGL_OBJECT_MESH_OBJECT_H_

#include <glad/glad.h>

#include "lazygl/geom/mesh.h"
#include "lazygl/object/buffer.h"
#include "lazygl/object/vertex_array.h"
#include "lazygl/utils/logger.h"

namespace lazygl
{
class MeshObject
{
public:
  MeshObject();
  ~MeshObject();

  // Copy constructors
  MeshObject(const MeshObject& rhs) = delete;
  MeshObject& operator = (const MeshObject& rhs) = delete;

  // Move constructors
  void Move(MeshObject&& rhs) noexcept;
  MeshObject(MeshObject&& rhs) noexcept;
  MeshObject& operator = (MeshObject&& rhs) noexcept;

  void SetMesh(const geom::Mesh& mesh);
  void SetMesh(geom::Mesh&& mesh);

  MeshObject& operator = (const geom::Mesh& mesh)
  {
    SetMesh(mesh);
    return *this;
  }

  MeshObject& operator = (geom::Mesh&& mesh)
  {
    SetMesh(std::move(mesh));
    return *this;
  }

  const auto& GetTextureFilename() const
  {
    return texture_filename_;
  }

  void Draw();

private:
  ArrayBuffer<float> positions_;
  ArrayBuffer<float> normals_;
  ArrayBuffer<float> tex_coords_;
  ElementBuffer indices_;
  std::string texture_filename_;

  VertexArray vao_;

  Logger log_{ "MeshObject" };
};
}

#endif // LAZYGL_OBJECT_MESH_OBJECT_H_
