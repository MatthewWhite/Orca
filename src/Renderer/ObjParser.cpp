#include "ObjParser.h"

#include <algorithm>
#include <cinttypes>
#include <cstdio>

ObjParser::ObjParser()
	: positions()
	, normals()
	, texCoords()
	, indexedVertices()
	, m_meshVerticesMap()
	, m_pCurrentMeshVertices(nullptr)
	, m_currentMeshName("")
{
}

ObjParser::~ObjParser()
{
}

void ObjParser::Parse(const std::string& filename)
{
	FILE* pFile = fopen(filename.c_str(), "rb");
	if (!pFile)
	{
		printf("Failed to load obj file \"%s\". %s\n", filename.c_str(), strerror(errno));
		return;
	}

	positions.emplace_back(0.0f);
	normals.emplace_back(0.0f, 0.0f, 1.0f);
	texCoords.emplace_back(0.0f);

	m_pCurrentMeshVertices = &m_meshVerticesMap[m_currentMeshName];

	char buff[256];
	while (fgets(buff, sizeof(buff), pFile))
	{
		if (buff[0] == 'v')
		{
			switch (buff[1])
			{
			case ' ':
			case '\t':
				// vertex
				ParsePosition(buff);
				break;
			case 'n':
				// normal
				ParseNormal(buff);
				break;
			case 't':
				// tex coords
				ParseTexCoord(buff);
				break;
			default:
				printf("Unsupported line in Wavefront OBJ file \"%s\"\n\tline: %s", filename.c_str(), buff);
				break;
			}
		}
		else if (buff[0] == 'f')
		{
			ParseFace(buff);
		}
		else if (buff[0] == 'u' &&
				 buff[1] == 's' &&
				 buff[2] == 'e' &&
				 buff[3] == 'm' &&
				 buff[4] == 't' &&
				 buff[5] == 'l')
		{
			// TODO: swap which mesh we're parsing faces for
		}
	}

	fclose(pFile);

	indexedVertices = *m_pCurrentMeshVertices;
}

std::vector<ObjParser::ObjMeshData> ObjParser::NewParse(const std::string& filename)
{
	FILE* pFile = fopen(filename.c_str(), "rb");
	if (!pFile)
	{
		printf("Failed to load obj file \"%s\". %s\n", filename.c_str(), strerror(errno));
		return std::vector<ObjMeshData>();
	}

	positions.emplace_back(0.0f);
	normals.emplace_back(0.0f, 0.0f, 1.0f);
	texCoords.emplace_back(0.0f);

	m_pCurrentMeshVertices = &m_meshVerticesMap[m_currentMeshName];

	char buff[256];
	while (fgets(buff, sizeof(buff), pFile))
	{
		if (buff[0] == 'v')
		{
			switch (buff[1])
			{
			case ' ':
			case '\t':
				// vertex
				ParsePosition(buff);
				break;
			case 'n':
				// normal
				ParseNormal(buff);
				break;
			case 't':
				// tex coords
				ParseTexCoord(buff);
				break;
			default:
				printf("Unsupported line in Wavefront OBJ file \"%s\"\n\tline: %s", filename.c_str(), buff);
				break;
			}
		}
		else if (buff[0] == 'f')
		{
			ParseFace(buff);
		}
		else if (strncmp(buff, "usemtl", 6) == 0)
		{
			ParseUsemtl(buff);
		}
	}

	fclose(pFile);

	std::vector<ObjMeshData> meshes;
	meshes.reserve(m_meshVerticesMap.size());
	for (auto& it : m_meshVerticesMap)
	{
		std::vector<IndexedVertex>& indexedVertices = it.second;
		if (indexedVertices.size() == 0)
		{
			continue;
		}
		std::sort(indexedVertices.begin(), indexedVertices.end());

		meshes.emplace_back();
		ObjMeshData& mesh = meshes[meshes.size() - 1];
		size_t totalIndexCount = indexedVertices.size();
		mesh.indices.resize(totalIndexCount);

		unsigned int uniqueIndexCount = 0;
		const IndexedVertex* pCurrVert = indexedVertices.data();
		const bool bMeshContainsTextureCoordinates = pCurrVert->t;
		const auto AddVertexToMesh = [&](const IndexedVertex* pVert, ObjMeshData& mesh) ->void
		{
			mesh.positions.push_back(positions[pCurrVert->p]);
			mesh.normals.push_back(normals[pCurrVert->n]);
			if (bMeshContainsTextureCoordinates)
			{
				mesh.texCoords.push_back(texCoords[pCurrVert->t]);
			}
		};

		AddVertexToMesh(pCurrVert, mesh);
		mesh.indices[pCurrVert->index] = uniqueIndexCount;
		for (int i = 1; i < totalIndexCount; ++i)
		{
			const IndexedVertex& vert = indexedVertices[i];
			if (vert != *pCurrVert)
			{
				pCurrVert = &vert;
				AddVertexToMesh(pCurrVert, mesh);
				++uniqueIndexCount;
			}
			mesh.indices[vert.index] = uniqueIndexCount;
		}
	}

	return meshes;
}

void ObjParser::ParsePosition(char* buffer)
{
	float x = strtof(buffer + 2, &buffer);
	float y = strtof(buffer, &buffer);
	float z = strtof(buffer, &buffer);
	positions.emplace_back(x, y, z);
}

void ObjParser::ParseNormal(char* buffer)
{
	float x = strtof(buffer + 3, &buffer);
	float y = strtof(buffer + 1, &buffer);
	float z = strtof(buffer + 1, &buffer);
	normals.emplace_back(x, y, z);
}

void ObjParser::ParseTexCoord(char* buffer)
{
	float s = strtof(buffer + 3, &buffer);
	float t = strtof(buffer + 1, &buffer);
	texCoords.emplace_back(s, t);
}

void ObjParser::ParseFace(char* buffer)
{
	int32_t p = 0;
	int32_t t = 0;
	int32_t n = 0;
	uint8_t vertCount = 0;

	uint32_t numTotalVerts = m_pCurrentMeshVertices->size();

	bool bFaceDefinesNormals = false;

	buffer += 2;
	while (p = (int32_t)strtol(buffer, &buffer, 10))
	{
		if (*buffer == '/')
		{
			++buffer;
			t = (int32_t)strtol(buffer, &buffer, 10);
			if (*buffer == '/')
			{
				++buffer;
				n = (int32_t)strtol(buffer, &buffer, 10);
				bFaceDefinesNormals = n;
			}
		}

		// fix up negative indices
		if (p < 0)
			p = positions.size() + p;
		if (t < 0)
			t = texCoords.size() + t;
		if (n < 0)
			n = normals.size() + n;

		m_pCurrentMeshVertices->emplace_back(p, t, n, numTotalVerts + vertCount++);
	}

	if (vertCount > 3)
	{
		const IndexedVertex& f0 = (*m_pCurrentMeshVertices)[numTotalVerts];
		m_pCurrentMeshVertices->emplace_back(f0.p, f0.t, f0.n, numTotalVerts + vertCount++);
		const IndexedVertex& f2 = (*m_pCurrentMeshVertices)[numTotalVerts + 2];
		m_pCurrentMeshVertices->emplace_back(f2.p, f2.t, f2.n, numTotalVerts + vertCount++);
	}

	if (!bFaceDefinesNormals)
	{
		// no normals defined. have to generate them ourself
		IndexedVertex* pV0 = &(*m_pCurrentMeshVertices)[numTotalVerts];
		const IndexedVertex* pEnd = &(*m_pCurrentMeshVertices)[numTotalVerts + vertCount - 1];
		int32_t normCount = normals.size();
		while (pV0 < pEnd)
		{
			IndexedVertex* pV1 = pV0 + 1;
			IndexedVertex* pV2 = pV1 + 1;

			// positions of each vertex
			const glm::vec3& pos0 = positions[pV0->p];
			const glm::vec3& pos1 = positions[pV1->p];
			const glm::vec3& pos2 = positions[pV2->p];

			// vectors between each vertex
			glm::vec3 vec01 = pos1 - pos0;
			glm::vec3 vec02 = pos2 - pos0;
			glm::vec3 vec12 = pos2 - pos1;

			normals.push_back(glm::normalize(glm::cross(vec01, vec02)));
			normals.push_back(glm::normalize(glm::cross(vec12, -vec02)));
			normals.push_back(glm::normalize(glm::cross(-vec02, -vec12)));

			pV0->n = normCount++;
			pV1->n = normCount++;
			pV2->n = normCount++;

			pV0 += 3;
		}
	}
}

void ObjParser::ParseUsemtl(char* buffer)
{
	char* c = &buffer[8];
	while (!isspace(*c))
	{
		++c;
	}
	char* start = &buffer[7];
	m_currentMeshName = std::string(start, c - start);

	m_pCurrentMeshVertices = &m_meshVerticesMap[m_currentMeshName];
}