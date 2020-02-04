#version 440

layout (location = 0) in vec2 position;
layout (location = 1) in vec2 tex_coord;

uniform ivec2 screen;

out vec2 frag_tex_coord;

void main()
{
  gl_Position = vec4(position / screen * 2.f - 1.f, 0.f, 1.f);
  frag_tex_coord = tex_coord;
}
