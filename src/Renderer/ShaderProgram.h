#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <map>

#include <glm/vec3.hpp>

typedef unsigned int shaderId_t;

class ShaderProgram
{
public:
    ShaderProgram(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
    ~ShaderProgram();

    void Bind();

    void SetUniform(const std::string& name, int value);
    void SetUniform(const std::string& name, float value);
    void SetUniform(const std::string& name, const glm::vec3& value);

private:
    shaderId_t mId;
    std::map<std::string, int> mUniformLocationMap;

    int GetUniformLocation(const std::string& name);

    static shaderId_t sCurrentProgram;
};

#endif