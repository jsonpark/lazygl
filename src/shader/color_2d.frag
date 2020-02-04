#version 440

in vec3 frag_color;
in vec2 frag_tex_coord;

uniform sampler2D tex;
uniform bool has_texture;

out vec4 out_color;

void main()
{
  if (has_texture)
    out_color = vec4(texture(tex, frag_tex_coord).rgb * frag_color, 1.f);

  else
    out_color = vec4(frag_color, 1.f);
}
