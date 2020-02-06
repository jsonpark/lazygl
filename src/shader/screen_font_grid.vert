#version 440

layout (location = 0) in vec2 position;

uniform ivec2 screen;

void main()
{
  gl_Position = vec4(position / screen * 2.f - 1.f, 0.f, 1.f);
}
