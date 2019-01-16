#version 330 core
layout (location = 0) in vec3 aPos;

out vec4 v_Color;

uniform mat4 model;
uniform vec3 color;

void main()
{
  gl_Position = model * vec4(aPos, 1.0);
  v_Color = vec4(color, 1.0);
}

