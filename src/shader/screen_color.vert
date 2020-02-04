#version 440

layout (location = 0) in vec2 position;
layout (location = 1) in vec3 color;

uniform ivec2 screen;

out vec3 frag_color;

void main()
{
  gl_Position = vec4(position / screen * 2.f - 1.f, 0.f, 1.f);
  //gl_Position = vec4(position, 0.f, 1.f);
  frag_color = color;
}
