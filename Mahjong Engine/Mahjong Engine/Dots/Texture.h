#pragma once
class Texture
{
public:
	Texture(const char* path, const bool& shouldAlpha);
	~Texture();

	void SetUseMipmaps(bool enabled);
	unsigned int GetID() const { return TextureObject; }

	bool UsesMipmaps;
	int Width;
	int Height;

	unsigned int TextureObject;

	// --- Methods ---
	void Bind() const;
	void Unbind() const;
};