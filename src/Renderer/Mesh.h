#ifndef MESH_H
#define MESH_H

#include <string>
#include <vector>

#include <glm/glm.hpp>

#include "Texture.h"

class Mesh
{
public:
	struct Vertex
	{
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 texCoords;

		Vertex(glm::vec3 p, glm::vec3 n, glm::vec2 t) : position(p), normal(n), texCoords(t) {}
	};

	Mesh();
	Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int> indices);
	~Mesh();

	void Draw() const;

private:
	void GenerateBuffers();

	std::vector<Vertex> mVertices;
	std::vector<unsigned int> mIndices;

	unsigned int mVAO;
	unsigned int mVBO;
	unsigned int mEBO;
};

#endif