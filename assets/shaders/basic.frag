#version 450 core

in vec3 v_color;
in vec2 v_uv1;

uniform sampler2D texture1;

out vec4 fragColor;

void main()
{
	vec4 tex = texture(texture1, v_uv1);
	fragColor = vec4(v_color, 1.0) * tex;
}