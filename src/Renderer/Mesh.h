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
	};

public:
	Mesh();
	~Mesh();

	bool Load(const std::string& filename);
	void Draw();

private:
	bool LoadWavefrontObj(const std::string& filename);

	std::vector<Vertex> mVertices;
	std::vector<unsigned int> mIndices;
	Texture mDiffuse;

	unsigned int mVAO;
	unsigned int mVBO;
	unsigned int mEBO;
};

#endif