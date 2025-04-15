#version 330 core

uniform sampler2D tex0;
// uniform vec4 color; 

out vec4 out_color;

in vec3 f_pos;
in vec2 f_uv;
in vec4 f_color;

void main()
{
  out_color = texture(tex0, f_uv) * f_color;
}
