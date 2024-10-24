#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>

#include <glad/glad.h>
#include <glm/glm.hpp>

typedef unsigned int textureId_t;

enum TextureWrapMode
{
	WM_REPEAT,
	WM_CLAMP,
	WM_BORDER,
	WM_MIRROR,
};

enum TextureFilterMode
{
	FM_NEAREST,		// nearest neighbor
	FM_BILINEAR,	// linear interpolation between texels within closest mipmap
	FM_TRILINEAR,	// linear interpolation between texels and between closest mipmaps
};

struct TextureParams
{
	TextureFilterMode filterMode = FM_BILINEAR;
	TextureWrapMode wrapMode = WM_REPEAT;
	glm::vec4 borderColor = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	int forceComponents = 0;
	bool bGenerateMips = true;
	bool bFlipVerticallyOnLoad = true;
};

class Texture
{
	friend class TextureManager;

public:
	void Bind();

	unsigned int GetWidth() const { return m_width; }
	unsigned int GetHeight() const { return m_height; }

private:
	std::string m_filename;
	textureId_t m_id;
	int m_width;
	int m_height;

	static textureId_t s_currentTexture;

	Texture();
	Texture(const std::string& filename, const TextureParams& params, bool isSRGB = false);
	~Texture();

	void Load(const std::string& filename, const TextureParams& params, bool isSRGB = false);
	void ApplyParams(const TextureParams& params);
};

#endif