#version 450 core
layout (location = 0) in vec3 a_position;
layout (location = 2) in vec2 a_uv1;

out vec2 v_uv1;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(a_position, 1.0);
	v_uv1 = a_uv1;
}