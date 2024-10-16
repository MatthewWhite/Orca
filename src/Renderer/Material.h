#ifndef MATERIAL_H
#define MATERIAL_H

#include <string>
#include <vector>

#include "Texture.h"

class Material
{
public:
	Material();
	~Material();

	bool Load(const std::string& filename);

	std::vector<Texture> textures;
	float shininess;

private:
};

#endif