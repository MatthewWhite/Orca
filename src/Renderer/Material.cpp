#include "Material.h"

#include <glad/glad.h>

#include "Shader.h"
#include "TextureManager.h"

Material::Material()
	: m_shader(nullptr)
	, m_textures()
	, m_vec4Params()
	, m_vec3Params()
	, m_floatParams()
	, m_integerParams()
{
}

Material::~Material()
{
	delete m_shader;
}

void Material::ApplyParams()
{
	m_shader->Bind();

	for (const auto& textureIt : m_textures)
	{
		int textureUnit = m_integerParams[textureIt.first];
		glActiveTexture(GL_TEXTURE0 + textureUnit);
		textureIt.second->Bind();
	}

	for (const auto& mat4It : m_mat4Params)
	{
		m_shader->SetUniform(mat4It.first, mat4It.second);
	}

	for (const auto& vec4It : m_vec4Params)
	{
		m_shader->SetUniform(vec4It.first, vec4It.second);
	}

	for (const auto& vec3It : m_vec3Params)
	{
		m_shader->SetUniform(vec3It.first, vec3It.second);
	}

	for (const auto& floatIt : m_floatParams)
	{
		m_shader->SetUniform(floatIt.first, floatIt.second);
	}

	for (const auto& integerIt : m_integerParams)
	{
		m_shader->SetUniform(integerIt.first, integerIt.second);
	}
}

void Material::SetShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	m_shader = new Shader(vertexShader, fragmentShader);
}

void Material::SetMat4(const std::string& name, const glm::mat4& mat4)
{
	m_mat4Params[name] = mat4;
}

void Material::SetTexture(const std::string& name, Texture* pTexture)
{
	auto it = m_textures.find(name);
	if (it != m_textures.end())
	{
		TextureManager::GetInstance()->DeleteTexture(it->second);
	}

	m_textures[name] = pTexture;
}

void Material::SetVec4(const std::string& name, const glm::vec4& vec4)
{
	m_vec4Params[name] = vec4;
}

void Material::SetVec3(const std::string& name, const glm::vec3& vec3)
{
	m_vec3Params[name] = vec3;
}

void Material::SetFloat(const std::string& name, float f)
{
	m_floatParams[name] = f;
}

void Material::SetInteger(const std::string& name, int i)
{
	m_integerParams[name] = i;
}