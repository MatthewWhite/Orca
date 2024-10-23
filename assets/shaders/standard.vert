#version 450 core
layout (location = 0) in vec3 a_position;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec2 a_uv1;

out vec3 v_fragPos;
out vec3 v_normal;
out vec2 v_uv1;

layout (std140, binding=0) uniform Matrices
{
	mat4 projection;
	mat4 view;
};
uniform mat4 model;

void main()
{
	v_fragPos = vec3(model * vec4(a_position, 1.0));
	// TODO: calculate normal matrix on CPU
	v_normal = mat3(transpose(inverse(model))) * a_normal;
	v_uv1 = a_uv1;
	
	gl_Position = projection * view * model * vec4(a_position, 1.0);
}