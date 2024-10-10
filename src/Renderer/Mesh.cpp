#include "Mesh.h"

#include <cstdio>

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

bool Mesh::Load(const std::string& filename)
{
	// load data...
	size_t fileExtensionPos = filename.find_last_of('.') + 1;
	if (fileExtensionPos == filename.npos)
	{
		printf("Failed to determine filetype of \"%s\"\n", filename);
		return false;
	}
	if (strcmp("obj", filename.substr(fileExtensionPos).c_str()) == 0)
	{
		LoadWavefrontObj(filename);
	}

	glGenVertexArrays(1, &mVAO);
	glGenBuffers(1, &mVBO);
	glGenBuffers(1, &mEBO);

	glBindVertexArray(mVAO);

	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBufferData(GL_ARRAY_BUFFER, mVertices.size(), mVertices.data(), GL_STATIC_DRAW);

	// NOTE: VAO stores any EBO bound, so don't unbind EBO until VAO is unbound
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size(), mIndices.data(), GL_STATIC_DRAW);

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
}

void Mesh::Draw()
{
	// TODO: come up with a way to determine which texture index to use
	glActiveTexture(GL_TEXTURE0);
	mDiffuse.Bind();

	glBindVertexArray(mVAO);
	glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, NULL);
}

bool Mesh::LoadWavefrontObj(const std::string& filename)
{
	FILE* pFile = fopen(filename.c_str(), "rb");
	if (!pFile)
	{
		printf("Failed to load obj file \"%s\". %s\n", filename.c_str(), strerror(errno));
		return nullptr;
	}

	char buff[256];

	while (fgets(buff, sizeof(buff), pFile))
	{
		if (buff[0] == 'v')
		{
			switch (buff[1])
			{
			case ' ':
				// vertex
				break;
			case 'n':
				// normal
				break;
			case 't':
				// tex coords
				break;
			default:
				printf("Unsupported line in Wavefront OBJ file \"%s\"\n\tline: %s", filename, buff);
				break;
			}
		}
		else if (buff[0] == 'f')
		{
			// face
		}
		else if (strncmp(buff, "mtllib", 6) == 0)
		{
			// TODO: support material file
		}
		else if (strncmp(buff, "usemtl", 6) == 0)
		{
			// TODO: support materials
		}
		else if (buff[0] == 'o')
		{
			// TODO: support objects
		}
		else if (buff[0] == 'g')
		{
			// TODO: support polygon groups
		}
		else if (buff[0] == '#' || '\n')
		{
			continue;
		}
	}

	fclose(pFile);
}