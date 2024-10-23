#include "Mesh.h"

#include <algorithm>
#include <cinttypes>
#include <cstdio>
#include <map>
#include <unordered_map>

Mesh::Mesh()
	: m_material()
	, m_vertices()
	, m_indices()
	, m_vao(0)
	, m_vbo(0)
	, m_ebo(0)
{
	m_material.SetShader("assets/shaders/standard.vert", "assets/shaders/standard.frag");
}

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int> indices)
	: m_vertices(vertices)
	, m_indices(indices)
	, m_vao(0)
	, m_vbo(0)
	, m_ebo(0)
{
	GenerateBuffers();
	m_material.SetShader("assets/shaders/standard.vert", "assets/shaders/standard.frag");
}

Mesh::~Mesh()
{
}

void Mesh::GenerateBuffers()
{
	// generate buffers
	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vbo);
	glGenBuffers(1, &m_ebo);

	glBindVertexArray(m_vao);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), m_vertices.data(), GL_STATIC_DRAW);

	// NOTE: VAO stores any EBO bound, so don't unbind EBO until VAO is unbound
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), m_indices.data(), GL_STATIC_DRAW);

	// position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	glEnableVertexAttribArray(0);
	// normals
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	glEnableVertexAttribArray(1);
	// uvs
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));
	glEnableVertexAttribArray(2);

	// unbind buffers
	glBindVertexArray(0);
}

void Mesh::Draw()
{
	m_material.ApplyParams();

	glBindVertexArray(m_vao);
	glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
}