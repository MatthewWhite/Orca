#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <map>

#include <glm/glm.hpp>

typedef unsigned int shaderId_t;

class Shader
{
public:
	Shader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
	~Shader();

	void Bind();

	void SetUniform(const std::string& name, int value);
	void SetUniform(const std::string& name, float value);
	void SetUniform(const std::string& name, const glm::vec3& value);
	void SetUniform(const std::string& name, const glm::mat4& value);

private:
	shaderId_t m_id;
	std::map<std::string, int> mUniformLocationMap;

	int GetUniformLocation(const std::string& name);

	static shaderId_t sCurrentProgram;
};

#endif