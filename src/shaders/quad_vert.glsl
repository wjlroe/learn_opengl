#version 150 core

in vec2 a_Pos;
in float alpha;
out vec4 v_Color;

layout(std140) uniform Locals {
	vec3 u_Color;
	float u_Z;
};

void main() {
	v_Color = vec4(u_Color, alpha);
	vec4 position = vec4(a_Pos, u_Z, 1.0);
	gl_Position = position;
}
