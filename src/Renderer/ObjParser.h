#ifndef OBJ_PARSER_H
#define OBJ_PARSER_H

#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

#include <glm/glm.hpp>

class ObjParser
{
public:
	struct ObjMeshData
	{
		std::vector<glm::vec3> positions;
		std::vector<glm::vec3> normals;
		std::vector<glm::vec2> texCoords;

		std::vector<unsigned int> indices;
	};

	struct IndexedVertex
	{
		uint32_t p;
		uint32_t t;
		uint32_t n;

		uint32_t index;

		IndexedVertex(uint32_t p, uint32_t t, uint32_t n, uint32_t i) : p(p), t(t), n(n), index(i) {}

		bool operator==(const IndexedVertex& v) const
		{
			return p == v.p && t == v.t && n == v.n;
		}
		bool operator!=(const IndexedVertex& v) const
		{
			return !(*this == v);
		}
		bool operator<(const IndexedVertex& v) const
		{
			if (p != v.p) return p < v.p;
			if (n != v.n) return n < v.n;
			return t < v.t;
		}
	};

	ObjParser();
	~ObjParser();

	void Parse(const std::string& filename);

	std::vector<ObjMeshData> NewParse(const std::string& filename);

	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> texCoords;
	std::vector<IndexedVertex> indexedVertices;

private:
	void ParsePosition(char* buffer);
	void ParseNormal(char* buffer);
	void ParseTexCoord(char* buffer);

	void ParseFace(char* buffer);

	void ParseUsemtl(char* buffer);

	std::unordered_map<std::string, std::vector<IndexedVertex>> m_meshVerticesMap;
	std::vector<IndexedVertex>* m_pCurrentMeshVertices;
	std::string m_currentMeshName;
};

#endif