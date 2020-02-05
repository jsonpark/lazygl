#version 440

in vec2 frag_tex_coord;

uniform sampler2D tex;
uniform vec3 color;

out vec4 out_color;

void main()
{
  out_color = vec4(color, texture(tex, frag_tex_coord).r);
}
