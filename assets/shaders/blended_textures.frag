#version 450 core

in vec2 v_uv1;

uniform sampler2D texture1;
uniform sampler2D texture2;

out vec4 fragColor;

void main()
{
	vec4 tex1 = texture(texture1, v_uv1);
	vec4 tex2 = texture(texture2, v_uv1);
	fragColor = mix(tex1, tex2, 0.2);
}