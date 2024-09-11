#include "ShaderProgram.h"

#include <cerrno>
#include <cstdio>

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include "Core/Utils.h"


shaderId_t ShaderProgram::sCurrentProgram = 0;

ShaderProgram::ShaderProgram(const std::string& vertexShaderPath, const std::string& fragmentShaderPath)
	: mId(0)
	, mUniformLocationMap()
{
	auto CheckShaderCompileStatus = [](GLuint shader) -> void
	{
		GLint status;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
		if (!status)
		{
			char infoLog[512];
			glGetShaderInfoLog(shader, 512, NULL, infoLog);
			printf("ShaderProgram compilation failed. %s\n", infoLog);
		}
	};
	auto CheckProgramLinkStatus = [](GLuint program) -> void
	{
		GLint status;
		glGetProgramiv(program, GL_LINK_STATUS, &status);
		if (!status)
		{
			char infoLog[512];
			glGetProgramInfoLog(program, 512, NULL, infoLog);
			printf("Program linking failed. %s\n", infoLog);
		}
	};

	// vertex shader
	char* pVertexShaderSource = (char*)LoadFileContents(vertexShaderPath);
	if (!pVertexShaderSource)
	{
		printf("Failed to generate shader program. Invalid vertex shader \"%s\"\n", vertexShaderPath.c_str());
		return;
	}
	//const char* const pVertexShaderSource = vertexShaderSource.c_str();
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &pVertexShaderSource, NULL);
	glCompileShader(vertexShader);
	CheckShaderCompileStatus(vertexShader);

	// fragment shader
	char* pFragmentShaderSource = (char*)LoadFileContents(fragmentShaderPath);
	if (!pFragmentShaderSource)
	{
		printf("Failed to generate shader program. Invalid fragment shader \"%s\"\n", fragmentShaderPath.c_str());
		glDeleteShader(vertexShader);
		return;
	}
	//const char* const pFragmentShaderSource = fragmentShaderSource.c_str();
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &pFragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	CheckShaderCompileStatus(fragmentShader);

	// link shaders together
	mId = glCreateProgram();
	glAttachShader(mId, vertexShader);
	glAttachShader(mId, fragmentShader);
	glLinkProgram(mId);
	CheckProgramLinkStatus(mId);

	// clean up source code pointers and delete shaders
	delete[] pVertexShaderSource;
	delete[] pFragmentShaderSource;
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// TODO: pre-populate uniform map with locations of uniforms
}

ShaderProgram::~ShaderProgram()
{
	glDeleteProgram(mId);
}

void ShaderProgram::Bind()
{
	if (sCurrentProgram == mId)
	{
		return;
	}

	glUseProgram(mId);
	sCurrentProgram = mId;
}

void ShaderProgram::SetUniform(const std::string& name, int value)
{
	glUniform1i(GetUniformLocation(name), value);
}

void ShaderProgram::SetUniform(const std::string& name, float value)
{
	glUniform1f(GetUniformLocation(name), value);
}

void ShaderProgram::SetUniform(const std::string& name, const glm::vec3& value)
{
	glUniform3fv(GetUniformLocation(name), 1, glm::value_ptr(value));
}

void ShaderProgram::SetUniform(const std::string& name, const glm::mat4& value)
{
	glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value));
}

int ShaderProgram::GetUniformLocation(const std::string& name)
{
	auto it = mUniformLocationMap.find(name);
	if (it != mUniformLocationMap.end())
	{
		return it->second;
	}

	GLint loc = glGetUniformLocation(mId, name.c_str());
	if (loc != GL_INVALID_INDEX)
	{
		mUniformLocationMap[name] = loc;
	}
	return loc;
}