#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>

#include <glad/glad.h>

typedef unsigned int textureId_t;

class Texture
{
public:
	enum WrapMode
	{
		WM_REPEAT,
		WM_CLAMP,
		WM_MIRROR,

		WM_COUNT,
		WM_INVALID
	};

	enum FilterMode
	{
		FM_NEAREST,
		FM_LINEAR,
		FM_NEARESTMIPMAP,
		FM_TRILINIEARMIPMAP,

		FM_COUNT,
		FM_INVALID
	};

	Texture(const std::string& filename);
	~Texture();

	void Bind();

	void SetWrapMode(WrapMode wrapModeU, WrapMode wrapModeV = WM_INVALID);
	void SetFilterMode(FilterMode filterModeMin, FilterMode filterModeMag = FM_INVALID);

	unsigned int GetWidth() const { return mWidth; }
	unsigned int GetHeight() const { return mHeight; }

	static void InitTextureLoader();

private:
	textureId_t mId;

	WrapMode mWrapModeU;
	WrapMode mWrapModeV;
	FilterMode mFilterModeMin;
	FilterMode mFilterModeMag;

	int mWidth;
	int mHeight;

	static textureId_t s_currentTexture;
};

#endif