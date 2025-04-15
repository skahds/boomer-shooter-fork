#version 330 core

layout (location = 0) in vec3 v_pos;
layout (location = 1) in vec2 v_uv;
layout (location = 2) in vec4 v_color;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out vec3 f_pos;
out vec2 f_uv;
out vec4 f_color;

void main()
{
  gl_Position = projection * view * model * vec4(v_pos, 1.0);

  f_pos = v_pos;
  f_uv = v_uv;
  f_color = v_color;
}
