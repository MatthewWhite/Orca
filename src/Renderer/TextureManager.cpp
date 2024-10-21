#include "TextureManager.h"

TextureManager* TextureManager::s_instance = nullptr;

TextureManager::TextureManager()
	: m_textures()
{
}

TextureManager::~TextureManager()
{
	m_textures.clear();
}

TextureManager* TextureManager::GetInstance()
{
	if (!s_instance)
	{
		s_instance = new TextureManager();
	}

	return s_instance;
}

Texture* TextureManager::CreateTexture(const std::string& filename, bool isSRGB)
{
TextureParams defaultParams;
return CreateTexture(filename, defaultParams, isSRGB);
}

Texture* TextureManager::CreateTexture(const std::string& filename, const TextureParams& params, bool isSRGB)
{
	auto it = m_textures.find(filename);
	if (it != m_textures.end())
	{
		return it->second;
	}

	Texture* pTexture = new Texture(filename, params, isSRGB);
	if (pTexture)
	{
		m_textures[filename] = pTexture;
	}
	return pTexture;
}