#include "Mesh.h"

#include <algorithm>
#include <cinttypes>
#include <cstdio>
#include <map>
#include <unordered_map>

Mesh::Mesh()
	: mVertices()
	, mIndices()
	, mVAO(0)
	, mVBO(0)
	, mEBO(0)
{
}

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int> indices)
	: mVertices(vertices)
	, mIndices(indices)
	, mVAO(0)
	, mVBO(0)
	, mEBO(0)
{
	GenerateBuffers();
}

Mesh::~Mesh()
{
}

void Mesh::GenerateBuffers()
{
	// generate buffers
	glGenVertexArrays(1, &mVAO);
	glGenBuffers(1, &mVBO);
	glGenBuffers(1, &mEBO);

	glBindVertexArray(mVAO);

	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBufferData(GL_ARRAY_BUFFER, mVertices.size() * sizeof(Vertex), mVertices.data(), GL_STATIC_DRAW);

	// NOTE: VAO stores any EBO bound, so don't unbind EBO until VAO is unbound
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size() * sizeof(unsigned int), mIndices.data(), GL_STATIC_DRAW);

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

void Mesh::Draw() const
{
	// TODO: come up with a way to determine which texture index to use
	/*glActiveTexture(GL_TEXTURE0);
	mDiffuse.Bind();*/

	glBindVertexArray(mVAO);
	glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, 0);
}