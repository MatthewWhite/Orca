#version 450 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;

out vec3 vertexColor;

uniform float time;

void main()
{
	vec3 scaledPos = position * (sin(time) / 4 + 0.5);
	gl_Position = vec4(scaledPos, 1.0);
	vertexColor = color;
}