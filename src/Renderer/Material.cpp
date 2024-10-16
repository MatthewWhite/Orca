#include "Material.h"

#include <cstdio>

Material::Material()
	: textures()
	, shininess(1.0f)
{
}

Material::~Material()
{
}

bool Material::Load(const std::string& filename)
{
	FILE* pFile = fopen(filename.c_str(), "rb");
	if (!pFile)
	{
		printf("Failed to load mtl file \"%s\". %s\n", filename.c_str(), strerror(errno));
		return false;
	}

	textures.reserve(2);

	size_t directoryEndPos = filename.find_last_of('/');
	if (directoryEndPos == filename.npos)
	{
		directoryEndPos = filename.find_last_of('\\');
		if (directoryEndPos == filename.npos)
		{
			printf("Failed to determine diretory of material file \"%s\"", filename.c_str());
			return false;
		}
	}
	const std::string directory = filename.substr(0, ++directoryEndPos);

	char buff[256];
	while (fgets(buff, sizeof(buff), pFile))
	{
		if (buff[0] == 'N' && buff[1] == 's')
		{
			shininess = strtof(buff + 2, nullptr);
		}
		else if (buff[0] == 'm' && buff[1] == 'a' && buff[2] == 'p')
		{
			char texName[64];

			// TODO: deal with random order maps
			if (buff[4] == 'K' && (buff[5] == 'd' || buff[5] == 's'))
			{
				const char* c = &buff[7];
				char* t = texName;
				while (!isspace(*c))
				{
					*t++ = *c++;
				}
				*t = '\0';

				textures.emplace_back(directory + texName);
			}
		}
	}

	fclose(pFile);
	return true;
}