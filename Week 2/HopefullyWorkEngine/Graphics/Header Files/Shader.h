#pragma once
#include <string>

class Shader
{
private:
	std::string LoadShader(const char* Path);

	// Both shaders are used to return data in a way that can be used to create the shader program!
	unsigned int LoadVertexShader(const char* Path);
	unsigned int LoadFragmentShader(const char* Path);

public:
	unsigned int myShaderProgram; // Will contain both vertex- and fragment shader
	void Initialize(const char* VertexPath, const char* FragmentPath);
	void Use();
};