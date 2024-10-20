#ifndef MODEL_H
#define MODEL_H

#include <string>
#include <vector>

#include "Mesh.h"

struct aiMesh;
struct aiNode;
struct aiScene;

class Model
{
public:
	Model();
	~Model();

	void LoadModel(const std::string& filename);
	void Draw() const;

private:
	void ProcessAssimpNode(const aiNode* pNode, const aiScene* pScene);
	Mesh ProcessAssimpMesh(const aiMesh* pMesh, const aiScene* pScene);

	std::vector<Mesh> m_meshes;
	std::string m_directory;
};

#endif