#pragma once
class Texture
{
public:
	Texture(const char* path, const bool& shouldAlpha);
	~Texture();

	int Width;
	int Height;

	unsigned int TextureObject;

	// --- Methods ---
	void Bind() const;
	void Unbind() const;
};