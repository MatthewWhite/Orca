#ifndef MODEL_H
#define MODEL_H

#include <string>
#include <vector>

// TODO: remove after changing glm::mat4 m_transform with Transform component
#include <glm/glm.hpp>

#include "Component.h"

struct aiMesh;
struct aiNode;
struct aiScene;

class Mesh;

class ModelRenderer : public Component
{
public:
	ModelRenderer();
	~ModelRenderer();

	void LoadModel(const std::string& filename);
	void Draw() const;

	// TEMP
	void SetTransform(const glm::mat4& transform);

	DECLARE_COMPONENT_TYPE_ID(ModelRenderer);

private:
	void ProcessAssimpNode(const aiNode* pNode, const aiScene* pScene);
	Mesh* ProcessAssimpMesh(const aiMesh* pMesh, const aiScene* pScene);

	std::vector<Mesh*> m_meshes;
	std::string m_directory;

	// TODO: replace with Transform component
	glm::mat4 m_transform;
};

#endif