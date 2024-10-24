#include "Model.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <glm/glm.hpp>

#include "TextureManager.h"

// TEMP
#include "Core/InputManager.h"

Model::Model()
	: m_meshes()
	, m_directory("")
{
}

Model::~Model()
{
}

void Model::LoadModel(const std::string& filename)
{
	m_meshes.clear();

	size_t directorySeperatorPos = filename.find_last_of('/');
	if (directorySeperatorPos == filename.npos)
	{
		directorySeperatorPos = filename.find_last_of('\\');
		if (directorySeperatorPos = filename.npos)
		{
			printf("Failed to determine directory of \"%s\"\n", filename.c_str());
		}
	}
	m_directory = filename.substr(0, directorySeperatorPos + 1);

	Assimp::Importer importer;
	unsigned int postProcessFlags = aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_JoinIdenticalVertices | aiProcess_OptimizeGraph | aiProcess_OptimizeMeshes;
	const aiScene* pScene = importer.ReadFile(filename, postProcessFlags);
	if (!pScene || !pScene->mRootNode || pScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE)
	{
		printf("Error loading model \"%s\": %s\n", filename.c_str(), importer.GetErrorString());
		return;
	}

	m_meshes.reserve(pScene->mNumMeshes);
	ProcessAssimpNode(pScene->mRootNode, pScene);
}

void Model::Draw() const
{
	for (const auto it : m_meshes)
	{
		it->Draw();
	}

	//static int i = 0;
	//static bool bWaitForUp = false;
	//if (bWaitForUp)
	//{
	//	bWaitForUp = InputManager::GetInstance()->GetKeyState(Key::KEY_ENTER) == KeyState::KS_DOWN;
	//}
	//else if (InputManager::GetInstance()->GetKeyState(Key::KEY_ENTER) == KeyState::KS_DOWN)
	//{
	//	i = (i + 1) % m_meshes.size();
	//	bWaitForUp = true;
	//}

	//m_meshes[i].Draw();
}

// TEMP
void Model::SetTransform(const glm::mat4& transform)
{
	m_transform = transform;

	for (auto it : m_meshes)
	{
		it->GetMaterial().SetMat4("model", m_transform);
	}
}

void Model::ProcessAssimpNode(const aiNode* pNode, const aiScene* pScene)
{
	for (unsigned int i = 0; i < pNode->mNumMeshes; ++i)
	{
		m_meshes.push_back(ProcessAssimpMesh(pScene->mMeshes[pNode->mMeshes[i]], pScene));
	}

	for (unsigned int i = 0; i < pNode->mNumChildren; ++i)
	{
		ProcessAssimpNode(pNode->mChildren[i], pScene);
	}
}

Mesh* Model::ProcessAssimpMesh(const aiMesh* pMesh, const aiScene* pScene)
{
	std::vector<Mesh::Vertex> vertices;
	vertices.reserve(pMesh->mNumVertices);
	for (unsigned int i = 0; i < pMesh->mNumVertices; ++i)
	{
		glm::vec3 pos(0.0f);
		pos.x = pMesh->mVertices[i].x;
		pos.y = pMesh->mVertices[i].y;
		pos.z = pMesh->mVertices[i].z;

		glm::vec3 norm(0.0f, 0.0f, 1.0f);
		norm.x = pMesh->mNormals[i].x;
		norm.y = pMesh->mNormals[i].y;
		norm.z = pMesh->mNormals[i].z;

		glm::vec2 texCoords(0.0f);
		if (pMesh->mTextureCoords[0])
		{
			texCoords.x = pMesh->mTextureCoords[0][i].x;
			texCoords.y = pMesh->mTextureCoords[0][i].y;
		}

		vertices.emplace_back(pos, norm, texCoords);
	}

	std::vector<unsigned int> indices(pMesh->mNumFaces * 3);
	unsigned int* pIndex = indices.data();
	for (unsigned int i = 0; i < pMesh->mNumFaces; ++i)
	{
		*(pIndex++) = pMesh->mFaces[i].mIndices[0];
		*(pIndex++) = pMesh->mFaces[i].mIndices[1];
		*(pIndex++) = pMesh->mFaces[i].mIndices[2];
	}

	Mesh* mesh = new Mesh(vertices, indices);
	if (pMesh->mMaterialIndex >= 0)
	{
		aiMaterial* aiMat = pScene->mMaterials[pMesh->mMaterialIndex];
		Material& material = mesh->GetMaterial();

		float shininess = 0.0f;
		aiMat->Get(AI_MATKEY_SHININESS, shininess);
		material.SetFloat("material.shininess", shininess);

		if (aiMat->GetTextureCount(aiTextureType_DIFFUSE) > 0)
		{
			aiString diffuseTexture;
			aiMat->GetTexture(aiTextureType_DIFFUSE, 0, &diffuseTexture);
			TextureParams params;
			params.filterMode = FM_TRILINEAR;
			Texture* pDiffuse = TextureManager::GetInstance()->CreateTexture(m_directory + diffuseTexture.C_Str(), params, true);
			material.SetInteger("material.diffuse", 0);
			material.SetTexture("material.diffuse", pDiffuse);
		}

		if (aiMat->GetTextureCount(aiTextureType_SPECULAR) > 0)
		{
			aiString specularTexture;
			aiMat->GetTexture(aiTextureType_SPECULAR, 0, &specularTexture);
			TextureParams params;
			params.filterMode = FM_TRILINEAR;
			params.forceComponents = 3;
			Texture* pSpecular = TextureManager::GetInstance()->CreateTexture(m_directory + specularTexture.C_Str(), params, true);
			material.SetInteger("material.specular", 1);
			material.SetTexture("material.specular", pSpecular);
		}
	}

	return mesh;
}