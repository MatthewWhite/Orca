#ifndef MATERIAL_H
#define MATERIAL_H

#include <string>
#include <unordered_map>

#include <glm/glm.hpp>

class Shader;
class Texture;

class Material
{
public:
	Material();
	~Material();

	void ApplyParams();

	void SetShader(const std::string& vertexShader, const std::string& fragmentShader);
	void SetTexture(const std::string& name, Texture* pTexture);
	void SetMat4(const std::string& name, const glm::mat4& mat4);
	void SetVec4(const std::string& name, const glm::vec4& vec4);
	void SetVec3(const std::string& name, const glm::vec3& vec3);
	void SetFloat(const std::string& name, float f);
	void SetInteger(const std::string& name, int i);

private:
	Shader* m_shader;
	std::unordered_map<std::string, Texture*> m_textures;
	std::unordered_map<std::string, glm::mat4> m_mat4Params;
	std::unordered_map<std::string, glm::vec4> m_vec4Params;
	std::unordered_map<std::string, glm::vec3> m_vec3Params;
	std::unordered_map<std::string, float> m_floatParams;
	std::unordered_map<std::string, int> m_integerParams;
};

#endif