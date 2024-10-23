#ifndef MODEL_H
#define MODEL_H

#include <string>
#include <vector>

// TEMP
#include <glm/glm.hpp>

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

	// TEMP
	void SetTransform(const glm::mat4& transform);

private:
	void ProcessAssimpNode(const aiNode* pNode, const aiScene* pScene);
	Mesh* ProcessAssimpMesh(const aiMesh* pMesh, const aiScene* pScene);

	std::vector<Mesh*> m_meshes;
	std::string m_directory;

	// TEMP
	glm::mat4 m_transform;
};

#endif