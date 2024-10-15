#ifndef MESH_H
#define MESH_H

#include <string>
#include <vector>

#include <glm/glm.hpp>

#include "Texture.h"

class Mesh
{
	struct Vertex
	{
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 texCoords;

		Vertex(glm::vec3 p, glm::vec3 n, glm::vec2 t) : position(p), normal(n), texCoords(t) {}
	};

public:
	Mesh();
	~Mesh();

	bool Load(const std::string& filename, bool bUseNew = false);
	void Draw();

private:
	bool LoadWavefrontObj(const std::string& filename);

	glm::vec3 ParsePosition(char* buffer);
	glm::vec3 ParseNormal(char* buffer);
	glm::vec2 ParseTexCoord(char* buffer);
	void ParseFace(char* buffer);

	std::vector<Vertex> mVertices;
	std::vector<unsigned int> mIndices;
	Texture mDiffuse;

	unsigned int mVAO;
	unsigned int mVBO;
	unsigned int mEBO;
};

#endif