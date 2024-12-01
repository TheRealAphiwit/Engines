#pragma once
#include <glm.hpp>

class Mesh;
class Texture;
class Shader;

namespace DotsRendering
{
	class Camera;
}

class VirtualObject
{
public:
	VirtualObject(Mesh* mesh, Texture* texture, Shader* shader);

	void SetMesh(Mesh& mesh);
	void SetTexture(Texture& texture);
	void SetShader(Shader& shader);

	void Draw(DotsRendering::Camera* camera);

	glm::vec3 Position;
	glm::vec3 Scale;
	glm::vec3 Rotation;

	Shader* GetShader();

	// Stray thought - how can quaternion be used here?

private:
	Mesh* myMesh;
	Texture* myTexture;
	Shader* myShader;
};