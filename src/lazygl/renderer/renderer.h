#ifndef LAZYGL_WINDOW_RENDERER_H_
#define LAZYGL_WINDOW_RENDERER_H_

#include "lazygl/window/window.h"

#include "lazygl/object/buffer.h"
#include "lazygl/object/vertex_array.h"
#include "lazygl/shader/program.h"
#include "lazygl/geom/texture.h"
#include "lazygl/object/texture_object.h"
#include "lazygl/object/mesh_object.h"

namespace lazygl
{
class Renderer : public Window
{
private:
  using Textures = std::unordered_map<std::string, geom::Texture<unsigned char>>;
  using TextureObjects = std::unordered_map<std::string, TextureObject2D<unsigned char>>;

  using MeshObjects = std::unordered_map<std::string, MeshObject>;

public:
  Renderer();

  Renderer(int width, int height);

  Renderer(int width, int height, const std::string& title);

  ~Renderer();

  void Initialize() override;

  // Callback functions
  void MouseButton(input::MouseButton button, input::MouseAction action, double x, double y) override;
  void MouseMove(double x, double y) override;
  void Keyboard(input::Key key, input::KeyAction action) override;
  void Resize(int width, int height) override;
  void Draw() override;

private:
  Program program_mesh_{ "..\\src\\shader", "mesh" };

  Textures textures_;
  TextureObjects texture_objects_;

  MeshObjects meshes_;

  Logger log_{ "Render2D" };
};
}

#endif // LAZYGL_WINDOW_RENDERER_H_
