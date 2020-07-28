#shader vertex
#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

out vec3 FragPos;
out vec3 Normal;

void main()
{
	gl_Position = proj * view * model * vec4(aPos, 1.0);
	FragPos = vec3(model * vec4(aPos, 1.0));
	Normal = mat3(transpose(inverse(model))) * aNormal;
}
#shader fragment
#version 460 core

in vec3 Normal;
in vec3 FragPos;

uniform struct {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
} material;

uniform struct {
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
} light;

uniform vec3 lightPos;
uniform vec3 viewPos;

out vec4 outputColor;

void main()
{
	vec3 ambient = light.ambient * material.ambient;

	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.diffuse * (diff * material.diffuse);

	float specularStrength = 0.5;
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * (spec * material.specular);

	vec3 result = ambient + diffuse + specular;	
	outputColor = vec4(result, 1.0);
}