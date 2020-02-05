#version 440

in float frag_tex_coord;

uniform sampler1D tex;

out vec4 out_color;

void main()
{
  out_color = vec4(texture(tex, frag_tex_coord).rgb, 1.f);
}
