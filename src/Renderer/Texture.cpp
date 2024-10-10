#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace
{
	const GLint OGL_WRAP_MODES[Texture::WrapMode::WM_COUNT] =
	{
		GL_REPEAT,              // WM_REPEAT
		GL_CLAMP_TO_EDGE,       // WM_CLAMP
		GL_MIRRORED_REPEAT,     // WM_MIRROR
	};
}

textureId_t Texture::s_currentTexture = 0;

void Texture::InitTextureLoader()
{
	stbi_set_flip_vertically_on_load(true);
}

Texture::Texture()
	: mId(0)
	, mWrapModeU(WrapMode::WM_INVALID)
	, mWrapModeV(WrapMode::WM_INVALID)
	, mFilterModeMin(FilterMode::FM_INVALID)
	, mFilterModeMag(FilterMode::FM_INVALID)
	, mWidth(0)
	, mHeight(0)
{

}

Texture::Texture(const std::string& filename)
	: mId(0)
	, mWrapModeU(WrapMode::WM_INVALID)
	, mWrapModeV(WrapMode::WM_INVALID)
	, mFilterModeMin(FilterMode::FM_INVALID)
	, mFilterModeMag(FilterMode::FM_INVALID)
	, mWidth(0)
	, mHeight(0)
{
	Load(filename);
}

void Texture::Load(const std::string& filename)
{
	int numChannels;
	unsigned char* pTextureData = stbi_load(filename.c_str(), &mWidth, &mHeight, &numChannels, 0);
	if (!pTextureData)
	{
		printf("Failed to load texture \"%s\"\n", filename.c_str());
		return;
	}

	size_t nameEndPos = filename.find_last_of(".");
	const bool bGammaCorrect = filename[nameEndPos - 2] != '_' || filename[nameEndPos - 1] == 'd';

	GLenum format = GL_RGBA;
	GLenum internalFormat = format;
	if (numChannels == 3)
	{
		format = GL_RGB;
		internalFormat = bGammaCorrect ? GL_SRGB : format;
	}
	else
	{
		if (bGammaCorrect)
			internalFormat = GL_SRGB_ALPHA;
	}

	glGenTextures(1, &mId);
	Bind();

	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, mWidth, mHeight, 0, format, GL_UNSIGNED_BYTE, (void*)pTextureData);

	SetWrapMode(WrapMode::WM_REPEAT);
	glGenerateMipmap(GL_TEXTURE_2D);
	SetFilterMode(FilterMode::FM_LINEAR);

	stbi_image_free(pTextureData);
}

void Texture::SetWrapMode(WrapMode wrapModeU, WrapMode wrapModeV)
{
	if (wrapModeV == WM_INVALID)
	{
		wrapModeV = wrapModeU;
	}

	if (mWrapModeU == wrapModeU && mWrapModeV == wrapModeV)
	{
		// no change necessary
		return;
	}

	Bind();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, OGL_WRAP_MODES[wrapModeU]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, OGL_WRAP_MODES[wrapModeV]);
	mWrapModeU = wrapModeU;
	mWrapModeV = wrapModeV;
}

void Texture::SetFilterMode(FilterMode filterModeMin, FilterMode filterModeMag)
{
	if (filterModeMag == FM_INVALID)
	{
		filterModeMag = filterModeMin;
	}

	if (mFilterModeMin == filterModeMin && mFilterModeMag == filterModeMag)
	{
		// no change necessary
		return;
	}

	Bind();

	switch (filterModeMin)
	{
	case Texture::FM_NEAREST:
		glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		break;
	case Texture::FM_LINEAR:
		glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		break;
	case Texture::FM_NEARESTMIPMAP:
		glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
		break;
	case Texture::FM_TRILINIEARMIPMAP:
		glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		break;
	default:
		printf("Unknown minification filter mode %i\n", filterModeMin);
		break;
	}

	switch (filterModeMag)
	{
	case Texture::FM_NEAREST:
		glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		break;
	case Texture::FM_LINEAR:
		glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		break;
	default:
		printf("Unknown/unsupported magnification filter mode %d\n", filterModeMag);
		break;
	}

	mFilterModeMin = filterModeMin;
	mFilterModeMag = filterModeMag;
}

Texture::~Texture()
{
	glDeleteTextures(1, &mId);
}

void Texture::Bind()
{
	if (s_currentTexture == mId)
	{
		return;
	}

	glBindTexture(GL_TEXTURE_2D, mId);
	s_currentTexture = mId;
}