#version 450 core

in vec3 v_fragPos;
in vec3 v_normal;
in vec2 v_uv1;

uniform vec3 viewPos;

uniform vec3 lightPosition;
uniform vec3 lightColor;
uniform float ambientStrength;
uniform float smoothness;

uniform sampler2D diffuseMap;
uniform sampler2D specularMap;

out vec4 fragColor;

void main()
{
	float gamma = 2.2;

	vec3 normal     = normalize(v_normal);
	vec3 viewDir    = normalize(viewPos - v_fragPos);
	vec3 lightDir   = normalize(lightPosition - v_fragPos);
	vec3 halfwayDir = normalize(viewDir + lightDir);

	float diffuseStrength  = max(dot(normal, lightDir), 0.0);
	float specularStrength = texture(specularMap, v_uv1).x;

	vec3 diffuseSample = vec3(texture(diffuseMap, v_uv1));

	vec3 ambient  = lightColor * ambientStrength * diffuseSample;
	vec3 diffuse  = lightColor * diffuseStrength * diffuseSample;
	vec3 specular = lightColor * specularStrength * pow(max(dot(normal, halfwayDir), 0.0), smoothness);

	fragColor = vec4(diffuse + specular + ambient, 1.0);
	fragColor.rgb = pow(fragColor.rgb, vec3(1.0/gamma));
}