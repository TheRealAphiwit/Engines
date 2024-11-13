#pragma once
#include <string>
#include <Math/glm/glm.hpp>

class Texture;

class Shader
{
private:
	std::string LoadShader(const char* shaderPath);
	unsigned int LoadVertexShader(const char* vertexShaderPath);
	unsigned int LoadFragmentShader(const char* fragmentShaderPath);
	void ApplyTexture(Texture* texture);

public:
	Shader(const char* vertexPath, const char* fragmentPath);

	void SetMatrix(glm::mat4 matrix, const std::string& name);

	// Utility methods - These are usefull to alter data later on
	void SetVector4(glm::vec4 vector4Value, const std::string& name);
	void SetVector3(glm::vec3 vector3Value, const std::string& name);
	void SetVector2(glm::vec2 vector2Value, const std::string& name);
	void SetFloat(float floatValue, const std::string& name);
	void SetInt(int intValue, const std::string& name);

	unsigned int myShaderProgram;
	void Use();
};