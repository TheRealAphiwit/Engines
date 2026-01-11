#include "Texture.h"
#include "stb_image.h"
#include <glad/glad.h>
#include <glfw3.h>
#include <iostream>

Texture::Texture(const char* path, const bool& shouldAlpha)
{
	UsesMipmaps = true;

	stbi_set_flip_vertically_on_load(true);

	int Channels = 0;
	unsigned char* data = stbi_load(path, &Width, &Height, &Channels, 0);

	if (!data)
	{
		std::cout << "Could not load texture: " << path << "\n";
		return;
	}

	GLenum format = GL_RGB;
	GLenum internalFormat = GL_RGB;

	if (Channels == 4)
	{
		format = GL_RGBA;
		internalFormat = GL_SRGB_ALPHA;
	}
	else
	{
		format = GL_RGB;
		internalFormat = GL_SRGB;
	}

	glGenTextures(1, &TextureObject);
	glBindTexture(GL_TEXTURE_2D, TextureObject);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		internalFormat,
		Width,
		Height,
		0,
		format,
		GL_UNSIGNED_BYTE,
		data
	);

	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);
	stbi_image_free(data);

	#pragma region Old
	//UsesMipmaps = true;

	//stbi_set_flip_vertically_on_load(true);

	//int Channels = 0;

	//unsigned char* data = stbi_load(path, &Width, &Height, &Channels, 0);

	////Width = 0;
	////Height = 0;

	//glGenTextures(1, &TextureObject);
	//glBindTexture(GL_TEXTURE_2D, TextureObject);

	//// Repeating S&T is like telling it to repeat X&Y for textures
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	///*glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);*/

	//// Old version
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	//if (data)
	//{
	//	if (shouldAlpha)
	//	{
	//		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Width, Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	//	}
	//	else
	//	{
	//		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Width, Height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	//	}
	//	glGenerateMipmap(GL_TEXTURE_2D);
	//}
	//else
	//{
	//	std::cout << "Could not load texture" << std::endl;
	//}

	//glBindTexture(GL_TEXTURE_2D, 0);
	//stbi_image_free(data);
#pragma endregion
}

Texture::~Texture()
{
	glDeleteTextures(1, &TextureObject);
}

// [!] This function causes linker errors when called from outside this file for some reason
void Texture::SetUseMipmaps(bool enabled)
{
	UsesMipmaps = enabled;

	glBindTexture(GL_TEXTURE_2D, TextureObject);
	if (enabled)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	}
	else
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	}

	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::Bind() const
{
	glBindTexture(GL_TEXTURE_2D, TextureObject);
}

void Texture::Unbind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}