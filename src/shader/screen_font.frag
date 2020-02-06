#version 440

in vec2 frag_tex_coord;

uniform sampler2D tex;
uniform vec3 color;

out vec4 out_color;

void main()
{
  float alpha = texture(tex, frag_tex_coord).r;
  out_color = vec4(color, 1.f - (1.f - alpha) * (1.f - alpha));
}
