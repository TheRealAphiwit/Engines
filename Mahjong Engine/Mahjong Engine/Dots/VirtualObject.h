#pragma once
#include <Math/glm/glm.hpp>

class Mesh;
class Texture;
class Shader;

// Wierd namespace
/*
namespace Gorilla
{
	class Camera;
}
*/

class VirtualObject
{
public:
	VirtualObject(Mesh* mesh, Texture* texture, Shader* shader);

	void SetMesh(Mesh* mesh);
	void SetTexture(Texture* texture);
	void SetShader(Shader* shader);

	// void Draw(Gorilla::Camera* camera);

	glm::vec3 Position;
	glm::vec3 Scale;
	glm::vec3 Rotation;

	// Stray thought - how can quaternion be used here?

private:
	Mesh* mesh;
	Texture* texture;
	Shader* shader;
};