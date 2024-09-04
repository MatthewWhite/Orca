#version 450 core
layout (location = 0) in vec3 a_position;
layout (location = 1) in vec3 a_color;
layout (location = 2) in vec2 a_uv1;

out vec3 v_color;
out vec2 v_uv1;

uniform mat4 transform;

void main()
{
	gl_Position = transform * vec4(a_position, 1.0);
	v_color = a_color;
	v_uv1 = a_uv1;
}