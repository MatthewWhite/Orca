#version 450 core

in vec3 v_fragPos;
in vec3 v_normal;
in vec2 v_uv1;

uniform vec3 lightPosition;
uniform vec3 lightColor;
uniform vec3 ambientColor;

uniform sampler2D texture1;
uniform sampler2D texture2;

out vec4 fragColor;

void main()
{
	vec3 normal = normalize(v_normal);
	vec3 lightDir = normalize(lightPosition - v_fragPos);
	float lightDirFactor = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = lightDirFactor * lightColor;
	vec3 light = diffuse + ambientColor;

	vec4 tex1 = texture(texture1, v_uv1);
	vec4 tex2 = texture(texture2, v_uv1);
	fragColor = mix(tex1, tex2, 0.2) * vec4(light, 1.0);
}