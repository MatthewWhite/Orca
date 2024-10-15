#ifndef MODEL_H
#define MODEL_H

#include <string>
#include <vector>

#include "Mesh.h"

class Model
{
public:
	Model();
	~Model();

	//void Load(const std::string& filename);

private:
	std::vector<Mesh> mMeshes;
};

#endif