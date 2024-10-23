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
	void DeleteTexture(Texture* pTexture);

private:
	struct Entry
	{
		Texture* pTexture = nullptr;
		unsigned int refCount = 0;

		Entry(Texture* pTexture, int refCount) : pTexture(pTexture), refCount(refCount) {}
	};

	TextureManager();

	std::unordered_map<std::string, Entry> m_textures;

	static TextureManager* s_instance;
};

#endif