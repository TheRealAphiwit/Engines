#include "VirtualObject.h"
#include <glad/glad.h>
#include "Camera.h"
#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"
#include <gtc/matrix_transform.hpp>

VirtualObject::VirtualObject(Mesh* mesh, Texture* texture, Shader* shader)
{
	myMesh = mesh;
	myTexture = texture;
	myShader = shader;

	Position = glm::vec3(0, 0, 0);
	Rotation = glm::vec3(0, 0, 0);
	Scale = glm::vec3(1, 1, 1);
}

void VirtualObject::SetMesh(Mesh& mesh)
{
	myMesh = &mesh;
}

void VirtualObject::SetTexture(Texture& texture)
{
	myTexture = &texture;
}

void VirtualObject::SetShader(Shader& shader)
{
	myShader = &shader;
}

void VirtualObject::Draw(DotsRendering::Camera* camera)
{
	glm::mat4 trans = glm::mat4(1.0f);

	trans = glm::translate(trans, Position);

	trans = glm::rotate(trans, Rotation.x, glm:: vec3(1, 0, 0));
	trans = glm::rotate(trans, Rotation.y, glm::vec3(0, 1, 0));
	trans = glm::rotate(trans, Rotation.z, glm::vec3(0, 0, 1));

	trans = glm::scale(trans, Scale);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, myTexture->TextureObject);

	myShader->SetMatrix4(trans, "transform");
	myShader->SetMatrix4(camera->myView, "view");
	myShader->SetMatrix4(camera->myProjection, "project");
	myMesh->Draw(myShader);

	glBindTexture(GL_TEXTURE_2D, 0);
}

Shader* VirtualObject::GetShader()
{
	return myShader;
}