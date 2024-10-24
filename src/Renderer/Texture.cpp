#include "Texture.h"

#include <glm/gtc/type_ptr.hpp>
#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif
#include <stb_image.h>

textureId_t Texture::s_currentTexture = 0;

Texture::Texture()
	: m_filename("")
	, m_id(0)
	, m_width(0)
	, m_height(0)
{
}

Texture::Texture(const std::string& filename, const TextureParams& params, bool isSRGB)
	: m_filename(filename)
	, m_id(0)
	, m_width(0)
	, m_height(0)
{
	Load(filename, params, isSRGB);
}

Texture::~Texture()
{
	glDeleteTextures(1, &m_id);
}

void Texture::Load(const std::string& filename, const TextureParams& params, bool isSRGB)
{
	stbi_set_flip_vertically_on_load(params.bFlipVerticallyOnLoad);

	int numChannels;
	unsigned char* pTextureData = stbi_load(filename.c_str(), &m_width, &m_height, &numChannels, params.forceComponents);
	if (!pTextureData)
	{
		printf("Failed to load texture \"%s\"\n", filename.c_str());
		return;
	}
	numChannels = params.forceComponents == 0 ? numChannels : params.forceComponents;

	GLenum format = GL_RGBA;
	GLenum internalFormat = format;
	switch (numChannels)
	{
	case 1:
		format = GL_RED;
		internalFormat = GL_R8;
		break;
	case 2:
		format = GL_RG;
		internalFormat = GL_RG8;
		break;
	case 3:
		format = GL_RGB;
		internalFormat = isSRGB ? GL_SRGB8 : GL_RGB8;
		break;
	case 4:
		format = GL_RGBA;
		internalFormat = isSRGB? GL_SRGB8_ALPHA8 : GL_RGBA8;
		break;
	default:
		printf("Error. Unsupported number of channels (%i) in texture \"%s\"\n", numChannels, filename.c_str());
		break;
	}

	glGenTextures(1, &m_id);
	Bind();
	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, m_width, m_height, 0, format, GL_UNSIGNED_BYTE, (void*)pTextureData);

	glGenerateMipmap(GL_TEXTURE_2D);
	ApplyParams(params);

	stbi_image_free(pTextureData);
}

void Texture::ApplyParams(const TextureParams& params)
{
	switch (params.filterMode)
	{
	case FM_NEAREST:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		break;
	case FM_BILINEAR:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		break;
	case FM_TRILINEAR:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		break;
	default:
		printf("Error. Invalid filter mode %i specified for texture \"%s\"\n", params.filterMode, m_filename.c_str());
		break;
	}

	switch (params.wrapMode)
	{
	case WM_REPEAT:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		break;
	case WM_CLAMP:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		break;
	case WM_BORDER:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, glm::value_ptr(params.borderColor));
		break;
	case WM_MIRROR:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
		break;
	default:
		printf("Error. Invalid wrap mode %i specified for texture \"%s\"\n", params.wrapMode, m_filename.c_str());
		break;
	}
}

void Texture::Bind()
{
	if (s_currentTexture == m_id)
	{
		return;
	}

	glBindTexture(GL_TEXTURE_2D, m_id);
	s_currentTexture = m_id;
}