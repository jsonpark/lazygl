#version 440

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 tex_coord;

out vec3 frag_color;
out vec3 frag_normal;
out vec2 frag_tex_coord;

void main()
{
  gl_Position = vec4(position, 1.f);
  frag_normal = normal;
  frag_tex_coord = tex_coord;
}
