#pragma once
#include <string>
#include <memory>
#include <vector>
#include <glm.hpp>
#include <thread>
#include "ISerializable.h"

class Mesh;
class Texture;
class Shader;
class Component;

namespace DotsRendering
{
	class Camera;
}

class VirtualObject : public ISerializable
{
public:
	VirtualObject(std::shared_ptr<std::string> name, Mesh* mesh, Texture* texture, Shader* shader); // This is the updated one we'll try to use

	const std::string& GetName() const;
	std::shared_ptr<std::string> GetNamePtr();
	void SetName(const std::string& name); // #? Might be used when I make a handler that oversees objects with same name

#pragma region Setters
	// Visuals
	void SetMesh(Mesh& mesh, std::string& name);
	void SetTexture(Texture& texture, std::string& name);
	void SetShader(Shader& shader, std::string& name);

	// Transforms
	void SetRotation(const glm::vec3& aRotation);
	void SetTransform(const glm::mat4& aTransform);
#pragma endregion

#pragma region Setters For Names
	void SetMeshName(std::string name);
	void SetTextureName(std::string name);
	void SetShaderName(std::string name);
#pragma endregion


	void Draw(DotsRendering::Camera* camera);

	glm::vec3 Position;
	glm::vec3 Scale;
	glm::vec3 Rotation;

	Shader* GetShader();
	Texture* GetTexture();
	Mesh* GetMesh();
	glm::mat4 GetTrans();
	glm::vec3 GetExtents(); // No logic yet

	const std::string& GetTextureName() const;
	const std::string& GetModelName() const;
	const std::string& GetShaderName() const;

	glm::mat4 GetModelMatrix() const;

	// Stray thought - how can quaternion be used here?

	#pragma region Serialization, Saving and Loading
	bool WriteTo(std::ofstream& outFile) const override;
	bool ReadFrom(std::ifstream& inFile) override;

	void SaveToFile(const std::string& filePath);
	void LoadFromFile(const std::string& filePath);
	#pragma endregion

	#pragma region Component Setup
	// REREAD
	template <typename T, typename... Args>
	T* AddComponent(Args&&... args) {
		T* comp = new T(std::forward<Args>(args)...);
		components.push_back(std::unique_ptr<Component>(comp));
		return comp;
	}

	template <typename T>
	T* GetComponent() {
		for (auto& comp : components) {
			if (T* match = dynamic_cast<T*>(comp.get()))
				return match;
		}
		return nullptr;
	}
	#pragma endregion

private:
	std::shared_ptr<std::string> myName;
	std::string myTextureName;
	std::string myModelName;
	std::string myShaderName;
	
	Mesh* myMesh;
	Texture* myTexture;
	Shader* myShader;

	std::vector<std::unique_ptr<Component>> components;
};