#version 450 core

in vec3 v_fragPos;
in vec3 v_normal;

uniform vec3 viewPos;

uniform vec3 lightPosition;
uniform vec3 lightColor;
uniform float ambientStrength;
uniform float smoothness;
uniform float specularIntensity;

uniform vec3 color;

out vec4 fragColor;

void main()
{
	// apply gamma to color
	float gamma = 2.2;
	vec3 c = pow(color, vec3(gamma));

	vec3 normal     = normalize(v_normal);
	vec3 viewDir    = normalize(viewPos - v_fragPos);
	vec3 lightDir   = normalize(lightPosition - v_fragPos);
	vec3 halfwayDir = normalize(viewDir + lightDir);

	float diffuseStrength  = max(dot(normal, lightDir), 0.0);

	vec3 ambient  = lightColor * ambientStrength * c;
	vec3 diffuse  = lightColor * diffuseStrength * c;
	vec3 specular = lightColor * specularIntensity * pow(max(dot(normal, halfwayDir), 0.0), smoothness);

	fragColor = vec4(diffuse + specular + ambient, 1.0);
	fragColor.rgb = pow(fragColor.rgb, vec3(1.0/gamma));
}