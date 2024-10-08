#version 450 core
layout (location = 0) in vec3 a_position;
layout (location = 1) in vec3 a_normal;

out vec3 v_fragPos;
out vec3 v_normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	v_fragPos = vec3(model * vec4(a_position, 1.0));
	// TODO: calculate normal matrix on CPU
	v_normal = mat3(transpose(inverse(model))) * a_normal;

	gl_Position = projection * view * model * vec4(a_position, 1.0);
}