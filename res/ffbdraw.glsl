#version 330 core

uniform sampler2D tex0;
// uniform vec4 color; 

out vec4 out_color;

in vec2 f_pos;
in vec2 f_uv;

void main()
{
  out_color = texture(tex0, f_uv);
}
