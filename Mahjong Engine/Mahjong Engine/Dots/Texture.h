#pragma once
class Texture
{
public:
	Texture(const char* path, const bool& shouldAlpha);

	int Width;
	int Height;

	unsigned int TextureObject;
};