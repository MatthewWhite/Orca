#ifndef MESH_H
#define MESH_H

#include <string>
#include <vector>

#include <glm/glm.hpp>

#include "Material.h"
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

	Material& GetMaterial() { return m_material; }

	void Draw();

private:
	void GenerateBuffers();

	Material m_material;
	std::vector<Vertex> m_vertices;
	std::vector<unsigned int> m_indices;

	unsigned int m_vao;
	unsigned int m_vbo;
	unsigned int m_ebo;
};

#endif