#version 430 core

out vec4 out_color;

uniform sampler2D tex0;

in vec3 f_pos;
in vec2 f_uv;

void main()
{
  out_color = texture(tex0, f_uv);
}
