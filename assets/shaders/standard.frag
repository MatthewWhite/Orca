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
	float flux;
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

vec3 blinnPhongBRDF(vec3 lightDirection, vec3 viewDirection, vec3 normal, vec3 diffuse, vec3 specular, float shininess)
{
	vec3 halfwayDirection = normalize(lightDirection + viewDirection);
	float specDot = max(dot(halfwayDirection, normal), 0.0);
	return diffuse + pow(specDot, shininess) * specular;
}

vec3 rgb2lin(vec3 rgb)
{
	return pow(rgb, vec3(2.2));
}

vec3 lin2rgb(vec3 lin)
{
	return pow(lin, vec3(1.0/2.2));
}

void main()
{
	float distance = length(light.position - v_fragPos);

	vec3 normal = normalize(v_normal);
	vec3 viewDirection = normalize(viewPos - v_fragPos);
	vec3 lightDirection = normalize(light.position - v_fragPos);

	vec4 diffuse  = texture(material.diffuse, v_uv1);
	vec3 specular = vec3(texture(material.specular, v_uv1).r);
	vec3 ambient  = diffuse.rgb * light.ambientStrength;

	if (diffuse.a <= 0.1)
	{
		discard;
	}

	vec3 radiance = ambient;
	float irradiance = max(dot(lightDirection, normal), 0.0) * light.flux / (3.14159 * distance * distance);
	vec3 brdf = blinnPhongBRDF(lightDirection, viewDirection, normal, diffuse.rgb, specular, material.shininess);
	radiance += brdf * irradiance * rgb2lin(light.color);

	fragColor.rgb = lin2rgb(radiance);
	fragColor.a = diffuse.a;
}