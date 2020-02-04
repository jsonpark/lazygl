#include "lazygl/object/mesh_object.h"

namespace lazygl
{
MeshObject::MeshObject()
{
}

MeshObject::~MeshObject()
{
}

void MeshObject::Move(MeshObject&& rhs) noexcept
{
}

MeshObject::MeshObject(MeshObject&& rhs) noexcept
{
  Move(std::move(rhs));
}

MeshObject& MeshObject::operator = (MeshObject&& rhs) noexcept
{
  Move(std::move(rhs));
  return *this;
}

void MeshObject::SetMesh(const geom::Mesh& mesh)
{
  positions_ = mesh.GetPositions();
  normals_ = mesh.GetNormals();
  tex_coords_ = mesh.GetTexCoords();
  indices_ = mesh.GetIndices();
  texture_filename_ = mesh.GetTextureFilename();

  vao_.AttribPointer(0, 3, positions_);
  vao_.AttribPointer(1, 3, normals_);
  vao_.AttribPointer(2, 2, tex_coords_);
  vao_.Indices(VertexArray::DrawMode::TRIANGLES, indices_);
}

void MeshObject::SetMesh(geom::Mesh&& mesh)
{
  positions_ = std::move(mesh.GetPositions());
  normals_ = std::move(mesh.GetNormals());
  tex_coords_ = std::move(mesh.GetTexCoords());
  indices_ = std::move(mesh.GetIndices());
  texture_filename_ = std::move(mesh.GetTextureFilename());

  vao_.AttribPointer(0, 3, positions_);
  vao_.AttribPointer(1, 3, normals_);
  vao_.AttribPointer(2, 2, tex_coords_);
  vao_.Indices(VertexArray::DrawMode::TRIANGLES, indices_);
}

void MeshObject::Draw()
{
  // TODO: Texture

  vao_.Draw();
}
}
