#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

#include <string>
#include <unordered_map>

#include "Texture.h"

class TextureManager
{
public:
	~TextureManager();

	static TextureManager* GetInstance();

	Texture* CreateTexture(const std::string& filename, bool isSRGB = false);
	Texture* CreateTexture(const std::string& filename, const TextureParams& params, bool isSRGB = false);

private:
	TextureManager();

	std::unordered_map<std::string, Texture*> m_textures;

	static TextureManager* s_instance;
};

#endif