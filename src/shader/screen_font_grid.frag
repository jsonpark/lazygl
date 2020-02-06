#version 440

uniform ivec2 left_bottom;
uniform sampler2D tex;
uniform vec3 color;

out vec4 out_color;

void main()
{
  ivec2 tex_coord = ivec2(gl_FragCoord.xy) - left_bottom;
  float alpha = texelFetch(tex, tex_coord, 0).r;
  out_color = vec4(color, alpha);
}
