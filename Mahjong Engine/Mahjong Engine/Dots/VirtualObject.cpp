#include "VirtualObject.h"
#include <glad/glad.h>
#include "Camera.h"
#include "Mesh.h"
#include "Cube.h"
#include "Material.h"
#include "Shader.h"
#include "Texture.h"
#include "MahjongMath.h"
#include <gtc/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <gtx/quaternion.hpp>

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

void VirtualObject::SetRotation(const glm::vec3& aRotation)
{
	Rotation = aRotation;
}

void VirtualObject::SetTransform(const glm::mat4& aTransform)
{
	glm::vec3 translation, rotation, scale;
	MahjongMath::DecomposeTransform(aTransform, translation, rotation, scale);

	Position = translation;
	Rotation = rotation;
	Scale = scale;
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

#pragma region Phong Version
	//// Bind transformation matrices
	//myShader->SetMatrix4(trans, "modelMatrix");
	//myShader->SetMatrix4(camera->myView, "viewMatrix");
	//myShader->SetMatrix4(camera->myProjection, "projectionMatrix");

	//// Camera position (for specular reflections)
	//myShader->SetVector3(camera->GetCameraPosition(), "eyePosition");
	//
	//// Light properties
	//glm::vec3 lightPos(0.0f, 10.0f, 5.0f);
	//glm::vec3 lightAttenuation(1.0f, 0.09f, 0.032f); // Example values
	//myShader->SetVector3(lightPos, "light_position");
	//myShader->SetVector3(lightAttenuation, "light_attenuation");

	//// Light colors
	//glm::vec4 ambientLight(0.2f, 0.2f, 0.2f, 1.0f);
	//glm::vec4 diffuseLight(0.8f, 0.8f, 0.8f, 1.0f);
	//glm::vec4 specularLight(1.0f, 1.0f, 1.0f, 1.0f);

	//myShader->SetVector4(ambientLight, "light_ambient");
	//myShader->SetVector4(diffuseLight, "light_diffuse");
	//myShader->SetVector4(specularLight, "light_specular");

	//// Material properties
	//glm::vec4 materialDiffuse(1.0f, 1.0f, 1.0f, 1.0f);
	//glm::vec4 materialSpecular(0.5f, 0.5f, 0.5f, 1.0f);
	//float materialShininess = 32.0f;

	//myShader->SetVector4(materialDiffuse, "materialDiffuse");
	//myShader->SetVector4(materialSpecular, "materialSpecular");
	//myShader->SetFloat(materialShininess, "materialShininess");

	//// Set primary color for debugging or tinting
	//glm::vec4 primaryColor(1.0f, 1.0f, 1.0f, 1.0f); // White by default
	//myShader->SetVector4(primaryColor, "primaryColorVec4");
#pragma endregion

	// Camera pos = light pos
	//myShader->SetVector3(camera->GetCameraPosition(), "light_position");

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

Mesh* VirtualObject::GetMesh()
{
	return myMesh;
}

glm::mat4 VirtualObject::GetTrans()
{
	glm::mat4 rotation = glm::toMat4(glm::quat(Rotation));

	return glm::translate(glm::mat4(1.0f), Position) * rotation * glm::scale(glm::mat4(1.0f), Scale);
}

glm::vec3 VirtualObject::GetExtents()
{
	return Scale * 0.5f;
}

const std::string& VirtualObject::GetTextureName() const
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
	std::string tempName(nameSize, '\0'); // Pre-allocate space in the string
	inFile.read(&tempName[0], nameSize);
	myName = std::make_shared<std::string>(tempName);

	// Deserialize position, scale and rotation
	inFile.read(reinterpret_cast<char*>(&Position), sizeof(Position));
	inFile.read(reinterpret_cast<char*>(&Scale), sizeof(Scale));
	inFile.read(reinterpret_cast<char*>(&Rotation), sizeof(Rotation));

	// Deserialize model, texture and shader names
	size_t modelNameSize = 0;
	inFile.read(reinterpret_cast<char*>(&modelNameSize), sizeof(modelNameSize));
	myModelName.resize(modelNameSize);
	inFile.read(&myModelName[0], modelNameSize);

	size_t textureNameSize = 0;
	inFile.read(reinterpret_cast<char*>(&textureNameSize), sizeof(textureNameSize));
	myTextureName.resize(textureNameSize);
	inFile.read(&myTextureName[0], textureNameSize);

	size_t shaderNameSize = 0;
	inFile.read(reinterpret_cast<char*>(&shaderNameSize), sizeof(shaderNameSize));
	myShaderName.resize(shaderNameSize);
	inFile.read(&myShaderName[0], shaderNameSize);

	return inFile.good();
}

void VirtualObject::SaveToFile(const std::string& filePath)
{
	std::ofstream outFile(filePath, std::ios::binary);
	if (!outFile)
	{
		throw std::runtime_error("Failed to open file for saving: " + filePath);
	}

	if (!WriteTo(outFile))
	{
		throw std::runtime_error("Failed to write to file: " + filePath);
	}
	
	outFile.close();
}

void VirtualObject::LoadFromFile(const std::string& filePath)
{
	std::ifstream inFile(filePath, std::ios::binary);
	if (!inFile)
	{
		throw std::runtime_error("Failed to open file for loading: " + filePath);
	}

	if (!ReadFrom(inFile))
	{
		throw std::runtime_error("Failed to read from file: " + filePath);
	}

	inFile.close();
}