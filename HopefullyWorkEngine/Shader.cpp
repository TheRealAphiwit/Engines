#include "Shader.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <glad\glad.h>

std::string Shader::LoadShader(const char* Path)
{
	std::string shaderCode;
	std::ifstream shaderFile;

	shaderFile.open(Path);

	std::stringstream shaderStream;
	shaderStream << shaderFile.rdbuf();

	shaderFile.close();
	shaderCode = shaderStream.str();

	return shaderCode;
}

unsigned int Shader::LoadVertexShader(const char* Path)
{
	int result;
	char Log[512];

	// Load and convert shader to const char*
	std::string shaderCodeString = LoadShader(Path); // Loading the shader as string
	const char* shaderCode = shaderCodeString.c_str(); // Converting string to const char*

	// We'll make a OpenGL shader object that'll contain the vertex data
	unsigned int shaderObject; 

	// Read up on this - but in a nutshell; this part makes the shaderObject be treated as a shader
	shaderObject = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(shaderObject, 1, &shaderCode, NULL);
	glCompileShader(shaderObject);

	glGetShaderiv(shaderObject, GL_COMPILE_STATUS, &result);

	if (!result)
	{
		glGetShaderInfoLog(shaderObject, 512, NULL, Log);
		std::cout << "Failed to compile a vertex shader \n" << Log << std::endl;
	}

	return shaderObject;
}

// LoadingFragmentShader follows the same logic as loading a vertex shader; load file->convert from string to const char* -> create shader object
unsigned int Shader::LoadFragmentShader(const char* Path)
{
	int result;
	char Log[512];

	std::string shaderCodeString = LoadShader(Path);
	const char* shaderCode = shaderCodeString.c_str();

	unsigned int shaderObject;

	shaderObject = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(shaderObject, 1, &shaderCode, NULL);
	glCompileShader(shaderObject);

	glGetShaderiv(shaderObject, GL_COMPILE_STATUS, &result);

	if (!result)
	{
		glGetShaderInfoLog(shaderObject, 512, NULL, Log);
		std::cout << "Failed to compile a vertex shader \n" << Log << std::endl;
	}

	return shaderObject;
}

void Shader::Initialize(const char* VertexPath, const char* FragmentPath)
{
	int result;
	char Log[512];

	// Load both shaders needed
	unsigned int VertexShader = LoadVertexShader(VertexPath);
	unsigned int FragmentShader = LoadFragmentShader(FragmentPath);

	// Read more on - but in a nutshell; we're creting a actual shader program
	myShaderProgram = glCreateProgram();
	glAttachShader(myShaderProgram, VertexShader);
	glAttachShader(myShaderProgram, FragmentShader);
	glLinkProgram(myShaderProgram);

	// -glLinkProgram -
	/*
	* If any shader object of type GL_VERTEX_SHADER are attached to program,
	* they'll be used to create an exe that will run on the programmable vertex processor.
	*/

	glGetProgramiv(myShaderProgram, GL_LINK_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(myShaderProgram, 512, NULL, Log);
		std::cout << "Shader program could not compile\n" << Log << std::endl;
	}

	glDeleteShader(VertexShader);
	glDeleteShader(FragmentShader);
}

void Shader::Use()
{
	glUseProgram(myShaderProgram);
}