#pragma once
class Texture // new tweak
{
public:
	Texture(const char* path, const bool& shouldAlpha);
	~Texture();

	void SetUseMipmaps(bool enabled);
	bool GetUseMipmaps() const { return UsesMipmaps; }
	unsigned int GetID() const { return TextureObject; }

	bool UsesMipmaps;
	int Width;
	int Height;

	unsigned int TextureObject;

	// --- Methods ---
	void Bind() const;
	void Unbind() const;
};