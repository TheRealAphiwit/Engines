#pragma once
#include <unordered_map>
#include <vector>
#include <string>
#include "Messages.h"

class Texture;
class Material;
class Shader;
class Mesh;

class ResourceHandler
{
public:
	#pragma region Singelton Setup
	// Singelton
	static ResourceHandler& GetInstance()
	{
		static ResourceHandler instance;
		return instance;
	}

	// Prevent copy and overwriting
	ResourceHandler(const ResourceHandler&) = delete;
	ResourceHandler& operator=(const ResourceHandler&) = delete;
	#pragma endregion

	void ProcessMessages(MessageSystem::Message* aMessage);

	void CreateShader(const char* aVertexPath, const char* aFragmentPath, std::string aName);
	void CreateTexture(const char* aTexturePath, bool shouldAlpha, std::string aName);
	void CreateMaterial(const char* albedoPath, const char* specularPath, const char* normalPath, float shininess, std::string aName);
	void CreateMesh(const char* aModelPath, std::string aName);
	void RegisterMesh(Mesh* aMesh, std::string meshName);
	void RegisterShader(Shader* aShader);

	Shader* GetShader(std::string	aName);
	Texture* GetTexture(std::string	aName);
	Material* GetMaterial(std::string aName);
	Mesh* GetMesh(std::string		aName);

	const std::string GetShaderName(Shader* aShader);
	const std::string GetTextureName(Texture* aTexture);
	const std::string GetMaterialName(Material* aMaterial);
	const std::string GetMeshName(Mesh* aMesh);

	std::vector<std::string> GetAllResources();
	std::unordered_map<std::string, Texture*> GetTextures() { return myTextures; }
	std::unordered_map<std::string, Material*> GetMaterials() { return myMaterials;  }
	std::unordered_map<std::string, Shader*> GetShaders() { return myShaders; }
	std::unordered_map<std::string, Mesh*> GetMeshes() { return myMeshes; }

private:
	ResourceHandler();
	~ResourceHandler();
	std::unordered_map<std::string, Texture* >	myTextures;
	std::unordered_map<std::string, Material* > myMaterials;
	std::unordered_map<std::string, Shader*	>	myShaders;
	std::unordered_map<std::string, Mesh*	>	myMeshes;
};