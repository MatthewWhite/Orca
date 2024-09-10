#version 450 core

in vec2 v_uv1;

uniform sampler2D texture1;

out vec4 fragColor;

void main()
{
	fragColor = texture(texture1, v_uv1);
}