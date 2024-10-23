#version 450 core

struct Material
{
	sampler2D diffuse;
	sampler2D specular;
	float smoothness;
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

uniform vec3 viewPos;

uniform Material material;
uniform PointLight light;

out vec4 fragColor;

vec3 shadeBlinnPhong(vec3 diffuse, vec3 specular, vec3 ambient, float smoothness, vec3 lightColor, vec3 lightDirection, vec3 eyeDirection, vec3 normal)
{
	vec3 halfwayDirection = normalize(lightDirection + eyeDirection);

	float diffuseIntensity  = max(dot(normal, lightDirection), 0.0);
	float specularIntensity = pow(max(dot(normal, halfwayDirection), 0.0), material.smoothness);

	vec3 result = vec3(0.0f);
	result += lightColor * diffuse  * diffuseIntensity;	 // diffuse
	result += lightColor * specular * specularIntensity; // specular
	result += lightColor * ambient;						 // ambient

	return result;
}

void main()
{
	float gamma = 2.2;

	vec3 normal = normalize(v_normal);
	vec3 eyeDirection = normalize(viewPos - v_fragPos);
	vec3 lightDirection = normalize(light.position - v_fragPos);

	vec3 diffuse  = vec3(texture(material.diffuse, v_uv1));
	vec3 specular = vec3(texture(material.specular, v_uv1));
	vec3 ambient  = diffuse * light.ambientStrength;

	vec3 shadedColor = shadeBlinnPhong(diffuse, specular, ambient, material.smoothness, light.color, lightDirection, eyeDirection, normal);

	fragColor = vec4(shadedColor, 1.0);
	fragColor.rgb = pow(fragColor.rgb, vec3(1.0/gamma));
}