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

VirtualObject::VirtualObject(Mesh* mesh, Texture* texture, Shader* shader)
{
	myMesh = mesh;
	myTexture = texture;
	myShader = shader;

	Position = glm::vec3(0, 0, 0);
	Rotation = glm::vec3(0, 0, 0);
	Scale = glm::vec3(1, 1, 1);
}

VirtualObject::VirtualObject(std::shared_ptr<std::string> name, Mesh * mesh, Texture * texture, Shader * shader) : myName(std::move(name)), myMesh(mesh), myTexture(texture), myShader(shader)
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

void VirtualObject::SetMeshName(const std::string name)
{
	myModelName = name;
}

void VirtualObject::SetTextureName(const std::string name)
{
	myTextureName = name;
}

void VirtualObject::SetShaderName(const std::string name)
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

	myShader->SetMatrix4(trans, "transform");
	myShader->SetMatrix4(camera->myView, "view");
	myShader->SetMatrix4(camera->myProjection, "projection");
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
