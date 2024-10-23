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
		return it->second.pTexture;
	}

	Texture* pTexture = new Texture(filename, params, isSRGB);
	if (pTexture)
	{
		m_textures.emplace(std::piecewise_construct, std::forward_as_tuple(filename), std::forward_as_tuple(pTexture, 1));
	}
	return pTexture;
}

void TextureManager::DeleteTexture(Texture* pTexture)
{
	auto it = m_textures.find(pTexture->m_filename);
	if (it == m_textures.end())
	{
		printf("No texture \"%s\" found in TextureManager\n", pTexture->m_filename.c_str());
	}

	if (--it->second.refCount == 0)
	{
		delete it->second.pTexture;
		m_textures.erase(it);
	}
}