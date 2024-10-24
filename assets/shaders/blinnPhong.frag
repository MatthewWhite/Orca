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

void main()
{
    // calculate values we'll be using throughout
    vec3 normal = normalize(v_normal);
    vec3 viewDirection = normalize(viewPos - v_fragPos);
    vec3 lightDirection = normalize(light.position - v_fragPos);
    float distance = length(light.position - v_fragPos);
    float attenuation = 1.0 / (distance * distance);

    // sample textures
    vec4 diffuse = texture(material.diffuse, v_uv1);
    vec3 specular = vec3(texture(material.specular, v_uv1));

    if (diffuse.a < 0.1)
    {
        discard;
    }

    // calculate lighting components
    // ambient
    vec3 ambient = diffuse.rgb * light.ambientStrength;

    // diffuse
    float NdotL = max(dot(normal, lightDirection), 0.0);
    diffuse.rgb *= NdotL;

    // specular
    vec3 halfwayDirection = normalize(viewDirection + lightDirection);
    float NdotH = max(dot(normal, halfwayDirection), 0.0);
    float specPow = pow(NdotH, material.shininess);
    float specFalloff = 1 - pow(1 - NdotL, 3.0);
    specular *= specPow * specFalloff;

    // fragColor.rgb = ((diffuse.rgb + specular) * attenuation + ambient) * light.color;
    fragColor.rgb = (diffuse.rgb + specular + ambient) * attenuation * light.color;
    fragColor.rgb = pow(fragColor.rgb, vec3(1.0/2.2));
    fragColor.a = diffuse.a;
}