#version 440

layout (location = 0) in vec2 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 tex_coord;

out vec3 frag_color;
out vec2 frag_tex_coord;

void main()
{
  gl_Position = vec4(position, 0.f, 1.f);
  frag_color = color;
  frag_tex_coord = tex_coord;
}
