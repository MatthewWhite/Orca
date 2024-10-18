#include "Model.h"

#include "ObjParser.h"

Model::Model()
{
}

Model::~Model()
{
}

void Model::Load(const std::string& filename)
{
	ObjParser parser;
	std::vector<ObjParser::ObjMeshData> meshData = parser.NewParse(filename);
	for (const auto& it : meshData)
	{
		printf("Mesh contains %zi tris\n", it.indices.size() / 3);
	}
}