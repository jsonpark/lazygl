#include "lazygl/renderer/renderer.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace lazygl
{
Renderer::Renderer()
  : Renderer(800, 600)
{
}

Renderer::Renderer(int width, int height)
  : Renderer(width, height, "Renderer")
{
}

Renderer::Renderer(int width, int height, const std::string& title)
  : Window(width, height, title)
{
}

Renderer::~Renderer() = default;

void Renderer::MouseButton(input::MouseButton button, input::MouseAction action, double x, double y)
{
}

void Renderer::MouseMove(double x, double y)
{
}

void Renderer::Keyboard(input::Key key, input::KeyAction action)
{
}

void Renderer::Resize(int width, int height)
{
  Window::Resize(width, height);
}

void Renderer::Initialize()
{
  glClearColor(1.f, 1.f, 1.f, 1.f);
  glDisable(GL_DEPTH_TEST);

  geom::Mesh mesh("..\\..\\fetch_ros\\fetch_description\\meshes\\base_link.dae");
  textures_["base"].LoadAsync(mesh.GetTextureFilename());
  meshes_["base"] = std::move(mesh);
}

void Renderer::Draw()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  if (textures_["base"].IsReady())
  {
    textures_["base"].Get();
    texture_objects_["base"].Update(textures_["base"]);
  }

  if (textures_["base"].IsLoaded())
  {
    program_mesh_.Uniform1i("has_texture", 1);
    texture_objects_["base"].Bind();
  }
  else
  {
    program_mesh_.Uniform1i("has_texture", 0);
  }

  program_mesh_.Use();
  meshes_["base"].Draw();
}
}
