#pragma once
#include <string>
#include <memory>
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
	VirtualObject(std::shared_ptr<std::string> name, Mesh* mesh, Texture* texture, Shader* shader); // This is the updated one we'll try to use

	const std::string& GetName() const;
	std::shared_ptr<std::string> GetNamePtr();
	void SetName(const std::string& name); // #? Might be used when I make a handler that oversees objects with same name

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
	std::shared_ptr<std::string> myName;
	Mesh* myMesh;
	Texture* myTexture;
	Shader* myShader;
};