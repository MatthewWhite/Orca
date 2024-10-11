#include "Mesh.h"

#include <cinttypes>
#include <cstdio>
#include <map>

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
}

void Mesh::Draw()
{
	// TODO: come up with a way to determine which texture index to use
	/*glActiveTexture(GL_TEXTURE0);
	mDiffuse.Bind();*/

	glBindVertexArray(mVAO);
	glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, 0);
}

bool Mesh::LoadWavefrontObj(const std::string& filename)
{
	FILE* pFile = fopen(filename.c_str(), "rb");
	if (!pFile)
	{
		printf("Failed to load obj file \"%s\". %s\n", filename.c_str(), strerror(errno));
		return nullptr;
	}

	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> texCoords;

	struct Face
	{
		int32_t v, vn, vt;
		bool operator<(const Face& f) const
		{
			if (v != f.v) { return v < f.v; }
			if (vn != f.vn) { return vn < f.vn; }
			return vt < f.vt;
		}
	};
	std::map<Face, unsigned int> faceIndexMap;
	auto GetFaceIndex = [&faceIndexMap](Face f) -> unsigned int {
		const auto it = faceIndexMap.find(f);
		if (it != faceIndexMap.end())
		{
			return it->second;
		}
		else
		{
			unsigned int index = faceIndexMap.size();
			faceIndexMap.insert({ f, index });
			return index;
		}
	};

	char buff[256];
	while (fgets(buff, sizeof(buff), pFile))
	{
		if (buff[0] == 'v')
		{
			switch (buff[1])
			{
			case ' ':
				// vertex
				glm::vec3 v;
				sscanf(buff + 2, "%f %f %f", &v.x, &v.y, &v.z);
				positions.push_back(v);
				break;
			case 'n':
				// normal
				glm::vec3 vn;
				sscanf(buff + 3, "%f %f %f", &vn.x, &vn.y, &vn.z);
				normals.push_back(vn);
				break;
			case 't':
				// tex coords
				glm::vec2 vt;
				sscanf(buff + 3, "%f %f", &vt.s, &vt.t);
				texCoords.push_back(vt);
				break;
			default:
				printf("Unsupported line in Wavefront OBJ file \"%s\"\n\tline: %s", filename, buff);
				break;
			}
		}
		else if (buff[0] == 'f')
		{
			Face faces[4];
			int numFaces = 0;

			size_t offset = 2;
			for (; numFaces < 4; ++numFaces)
			{
				Face& f = faces[numFaces];
				int charsRead = 0;
				if (sscanf(buff + offset, "%" PRIi32 "%n", &f.v, &charsRead) == 1)
				{
					offset += charsRead;
					f.v = (f.v > 0) ? f.v - 1 : positions.size() + f.v;

					if (buff[offset] == '/')
					{
						++offset;
						if (sscanf(buff + offset, "%" PRIi32 "%n", &f.vt, &charsRead) == 0)
						{
							// no texture coordinates
							f.vt = -1;
						}
						f.vt = (f.vt > 0) ? f.vt - 1 : texCoords.size() + f.vt;

						offset += charsRead;
						if (buff[offset] == '/')
						{
							++offset;
							sscanf(buff + offset, "%" PRIi32 "%n", &f.vn, &charsRead);
							offset += charsRead;
							f.vn = (f.vn > 0) ? f.vn - 1 : normals.size() + f.vn;
						}
						else
						{
							f.vn = -1;
						}
					}
					else
					{
						// position-only faces
						f.vn = -1;
						f.vt = -1;
					}
				}
				else
				{
					break;
				}

				/*int read;
				if (sscanf(buff + offset, "%" PRIi32 "/%" PRIi32 "/%" PRIi32 "%n", &f.v, &f.vt, &f.vn, &read) == 0)
				{
					break;
				}
				offset += read;*/

				//if (f.v > 0)
				//{
				//	--f.v;
				//	--f.vn;
				//	--f.vt;
				//}
				//else
				//{
				//	f.v = positions.size() + f.v;
				//	f.vn = normals.size() + f.vn;
				//	f.vt = texCoords.size() + f.vt;
				//}

				unsigned int index = GetFaceIndex(f);
				mIndices.push_back(index);

				if (index >= mVertices.size())
				{
					glm::vec3 normal = f.vn >= 0 ? normals[f.vn] : glm::vec3(0.0f);
					glm::vec2 texCoord = f.vt >= 0 ? texCoords[f.vt] : glm::vec2(0.0f);

					Vertex v = {
						positions[f.v],
						normal,
						texCoord
					};
					mVertices.push_back(v);
				}
			}

			if (numFaces == 4)
			{
				mIndices.push_back(GetFaceIndex(faces[0]));
				mIndices.push_back(GetFaceIndex(faces[2]));
			}
		}
		else if (strncmp(buff, "mtllib", 6) == 0)
		{
			// TODO: support material file
			continue;
		}
		else if (strncmp(buff, "usemtl", 6) == 0)
		{
			// TODO: support materials
			continue;
		}
		else if (buff[0] == 'o')
		{
			// TODO: support objects
			continue;
		}
		else if (buff[0] == 'g')
		{
			// TODO: support polygon groups
			continue;
		}
		else if (buff[0] == '#' || '\n')
		{
			continue;
		}
	}

	fclose(pFile);
}