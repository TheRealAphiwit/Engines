#include "VirtualObject.h"
#include <glad/glad.h>
#include "Camera.h"
#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"
#include <gtc/matrix_transform.hpp>

// VirtualObject::VirtualObject(const std::string& name, Mesh* mesh, Texture* texture, Shader* shader)
//{
//	myMesh = mesh;
//	myTexture = texture;
//	myShader = shader;
//
//	Position = glm::vec3(0, 0, 0);
//	Rotation = glm::vec3(0, 0, 0);
//	Scale = glm::vec3(1, 1, 1);
//}

VirtualObject::VirtualObject(std::shared_ptr<std::string> name, Mesh * mesh, Texture * texture, Shader * shader) : myName(std::move(name)), myMesh(mesh), myTexture(texture), myShader(shader), myModelName("None"), myShaderName("None"), myTextureName("None")
{
	Position = glm::vec3(0, 0, 0);
	Rotation = glm::vec3(0, 0, 0);
	Scale = glm::vec3(1, 1, 1);
}

const std::string& VirtualObject::GetName() const
{
	return *myName;
}

std::shared_ptr<std::string> VirtualObject::GetNamePtr()
{
	return myName;
}

void VirtualObject::SetName(const std::string& name)
{
	*myName = name;
}

void VirtualObject::SetMesh(Mesh& mesh, std::string& name)
{
	myMesh = &mesh;
	myModelName = name;
}

void VirtualObject::SetTexture(Texture& texture, std::string& name)
{
	myTexture = &texture;
	myTextureName = name;
}

void VirtualObject::SetShader(Shader& shader, std::string& name)
{
	myShader = &shader;
	myShaderName = name;
}

void VirtualObject::SetMeshName(std::string name)
{
	myModelName = name;
}

void VirtualObject::SetTextureName(std::string name)
{
	myTextureName = name;
}

void VirtualObject::SetShaderName(std::string name)
{
	myShaderName = name;
}

void VirtualObject::Draw(DotsRendering::Camera* camera)
{
	glm::mat4 trans = glm::mat4(1.0f);

	trans = glm::translate(trans, Position);

	trans = glm::rotate(trans, Rotation.x, glm::vec3(1, 0, 0));
	trans = glm::rotate(trans, Rotation.y, glm::vec3(0, 1, 0));
	trans = glm::rotate(trans, Rotation.z, glm::vec3(0, 0, 1));

	trans = glm::scale(trans, Scale);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, myTexture->TextureObject);

	// Default version
	myShader->SetMatrix4(trans, "transform");
	myShader->SetMatrix4(camera->myView, "view");
	myShader->SetMatrix4(camera->myProjection, "projection");
	myShader->SetVector3(camera->GetCameraPosition(), "light_position");

	//Phong version
	//myShader->SetMatrix4(trans, "modelMatrix");
	//myShader->SetMatrix4(camera->myView, "viewMatrix");
	//myShader->SetMatrix4(camera->myProjection, "projectionMatrix");
	//myShader->SetVector3(camera->GetCameraPosition(), "eyePosition");
	//myShader->SetVector4(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), "primaryColorVec4"); // White
	//myShader->SetVector4(glm::vec4(0.2f, 0.2f, 0.2f, 1.0f), "light_ambient");    // Dim gray
	//myShader->SetVector4(glm::vec4(1.0f, 0.9f, 0.8f, 1.0f), "light_diffuse");    // Warm white
	//myShader->SetVector4(glm::vec4(0.9f, 0.8f, 0.7f, 1.0f), "light_specular");   // Specular
	//myShader->SetVector3(camera->GetCameraPosition(), "light_position"); // Light position
	//myShader->SetVector3(glm::vec3(1.0f, 0.1f, 0.001f), "light_attenuation");    // Attenuation

	myMesh->Draw(myShader);

	glBindTexture(GL_TEXTURE_2D, 0);
}

Shader* VirtualObject::GetShader()
{
	return myShader;
}

Texture* VirtualObject::GetTexture()
{
	return myTexture;
}

const std::string& VirtualObject::GetTexureName() const
{
	return myTextureName;
}

const std::string& VirtualObject::GetModelName() const
{
	return myModelName;
}

const std::string& VirtualObject::GetShaderName() const
{
	return myShaderName;
}

glm::mat4 VirtualObject::GetModelMatrix() const
{
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, Position);
	model = glm::rotate(model, Rotation.x, glm::vec3(1, 0, 0));
	model = glm::rotate(model, Rotation.y, glm::vec3(0, 1, 0));
	model = glm::rotate(model, Rotation.z, glm::vec3(0, 0, 1));
	model = glm::scale(model, Scale);

	return model;
}

bool VirtualObject::WriteTo(std::ofstream& outFile) const
{
	if (!outFile) return false;

	// Serializa name
	size_t nameSize = myName->size();
	outFile.write(reinterpret_cast<const char*>(&nameSize), sizeof(nameSize));
	outFile.write(myName->c_str(), nameSize);

	//Serialize position, scale and rotation
	outFile.write(reinterpret_cast<const char*>(&Position), sizeof(Position));
	outFile.write(reinterpret_cast<const char*>(&Scale), sizeof(Scale));
	outFile.write(reinterpret_cast<const char*>(&Rotation), sizeof(Rotation));

	// Serialize model, texture and shader names - we'll use names as identifiers
	size_t modelNameSize = myModelName.size();
	outFile.write(reinterpret_cast<const char*>(&modelNameSize), sizeof(modelNameSize));
	outFile.write(myModelName.c_str(), modelNameSize);

	size_t textureNameSize = myTextureName.size();
	outFile.write(reinterpret_cast<const char*>(&textureNameSize), sizeof(textureNameSize));
	outFile.write(myTextureName.c_str(), textureNameSize);

	size_t shaderNameSize = myShaderName.size();
	outFile.write(reinterpret_cast<const char*>(&shaderNameSize), sizeof(shaderNameSize));
	outFile.write(myShaderName.c_str(), shaderNameSize);

	return outFile.good();
}

bool VirtualObject::ReadFrom(std::ifstream& inFile)
{
	if (!inFile) return false;

	// Deserialize name
	size_t nameSize = 0; // Setting 0 to avoid garbage data
	inFile.read(reinterpret_cast<char*>(&nameSize), sizeof(nameSize));
	char* buffer = new char[nameSize + 1];
	inFile.read(buffer, nameSize);
	buffer[nameSize] = '\0';
	myName = std::make_shared<std::string>(buffer);
	delete[] buffer;

	// Deserialize position, scale and rotation
	inFile.read(reinterpret_cast<char*>(&Position), sizeof(Position));
	inFile.read(reinterpret_cast<char*>(&Scale), sizeof(Scale));
	inFile.read(reinterpret_cast<char*>(&Rotation), sizeof(Rotation));

	// Deserialize model, texture and shader names
	size_t modelNameSize = 0;
	inFile.read(reinterpret_cast<char*>(&modelNameSize), sizeof(modelNameSize));
	buffer = new char[modelNameSize + 1];
	inFile.read(buffer, modelNameSize);
	buffer[modelNameSize] = '\0';
	myModelName = buffer;
	delete[] buffer;

	size_t textureNameSize = 0;
	inFile.read(reinterpret_cast<char*>(&textureNameSize), sizeof(textureNameSize));
	buffer = new char[textureNameSize + 1];
	inFile.read(buffer, textureNameSize);
	buffer[textureNameSize] = '\0';
	myTextureName = buffer;
	delete[] buffer;

	size_t shaderNameSize = 0;
	inFile.read(reinterpret_cast<char*>(&shaderNameSize), sizeof(shaderNameSize));
	buffer = new char[shaderNameSize + 1];
	inFile.read(buffer, shaderNameSize);
	buffer[shaderNameSize] = '\0';
	myShaderName = buffer;
	delete[] buffer;

	return inFile.good();
}
