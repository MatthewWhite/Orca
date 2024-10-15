#include "Mesh.h"

#include <algorithm>
#include <cinttypes>
#include <cstdio>
#include <map>
#include <unordered_map>

#include "ObjParser.h"

Mesh::Mesh()
	: mVertices()
	, mIndices()
	, mDiffuse()
	, mVAO(0)
	, mVBO(0)
	, mEBO(0)
{
}

Mesh::~Mesh()
{
}

bool Mesh::Load(const std::string& filename, bool bUseNew)
{
	// load data
	size_t fileExtensionPos = filename.find_last_of('.') + 1;
	if (fileExtensionPos == filename.npos)
	{
		printf("Failed to determine filetype of \"%s\"\n", filename.c_str());
		return false;
	}
	if (strcmp("obj", filename.substr(fileExtensionPos).c_str()) == 0)
	{
		ObjParser parser;
		parser.Parse(filename);
		std::sort(parser.indexedVertices.begin(), parser.indexedVertices.end());
		size_t indexCount = parser.indexedVertices.size();
		mIndices.resize(indexCount);

		// convert obj data to a format our engine expects
		uint32_t index = 0;
		const ObjParser::IndexedVertex* pCurrentFace = &parser.indexedVertices[0];
		mIndices[pCurrentFace->index] = index;
		mVertices.emplace_back(parser.positions[pCurrentFace->p], parser.normals[pCurrentFace->n], parser.texCoords[pCurrentFace->t]);
		for (int i = 1; i < indexCount; ++i)
		{
			const ObjParser::IndexedVertex& v = parser.indexedVertices[i];
			if (v != *pCurrentFace)
			{
				++index;
				pCurrentFace = &v;
				mVertices.emplace_back(parser.positions[pCurrentFace->p], parser.normals[pCurrentFace->n], parser.texCoords[pCurrentFace->t]);
			}
			mIndices[v.index] = index;
		}
	}
	else
	{
		printf("Failed to load mesh \"%s\". Unsupported file type\n", filename.c_str());
	}

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
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	return true;
}

void Mesh::Draw()
{
	// TODO: come up with a way to determine which texture index to use
	/*glActiveTexture(GL_TEXTURE0);
	mDiffuse.Bind();*/

	glBindVertexArray(mVAO);
	glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, 0);
}