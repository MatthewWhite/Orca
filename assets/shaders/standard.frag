#version 450 core

struct Material
{
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

struct PointLight
{
	vec3 position;
	vec3 color;
	float ambientStrength;

	// attenuation
	float constant;
	float linear;
	float quadratic;
};

in vec3 v_fragPos;
in vec3 v_normal;
in vec2 v_uv1;

uniform Material material;

layout (std140, binding=1) uniform Lighting
{
	PointLight light;
};

// TEMP
layout (std140, binding=2) uniform Camera
{
	vec3 viewPos;
};

out vec4 fragColor;

vec3 shadeBlinnPhong(vec3 diffuse, vec3 specular, vec3 ambient, float shininess, vec3 lightColor, vec3 lightDirection, vec3 eyeDirection, vec3 normal)
{
	vec3 halfwayDirection = normalize(lightDirection + eyeDirection);

	float diffuseIntensity  = max(dot(normal, lightDirection), 0.0);
	float specularIntensity = pow(max(dot(normal, halfwayDirection), 0.0), material.shininess) * (1 - pow(1-diffuseIntensity, 3));

	vec3 result = vec3(0.0f);
	result += lightColor * diffuse  * diffuseIntensity;
	result += lightColor * specular * specularIntensity;
	result += lightColor * ambient  * light.ambientStrength;

	return result;
}

void main()
{
	float gamma = 2.2;
	float distance = length(light.position - v_fragPos);
	float attenuation = 1.0 / (light.constant + (light.linear * distance) + (light.quadratic * distance * distance));
	attenuation = 1.0 / (distance * distance);

	vec3 normal = normalize(v_normal);
	vec3 eyeDirection = normalize(viewPos - v_fragPos);
	vec3 lightDirection = normalize(light.position - v_fragPos);

	vec4 diffuse  = texture(material.diffuse, v_uv1);
	vec3 specular = vec3(texture(material.specular, v_uv1));
	vec3 ambient  = diffuse.rgb;

	if (diffuse.a <= 0.1)
	{
		discard;
	}

	vec3 shadedColor = shadeBlinnPhong(diffuse.rgb, specular, ambient, material.shininess, light.color, lightDirection, eyeDirection, normal);

	fragColor = vec4(shadedColor, 1.0);
	fragColor.rgb *= attenuation;
	fragColor.rgb = pow(fragColor.rgb, vec3(1.0/gamma));
}