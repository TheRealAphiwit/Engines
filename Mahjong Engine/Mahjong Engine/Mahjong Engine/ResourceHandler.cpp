#include "ResourceHandler.h"
#include "Shader.h"
#include "Texture.h"
#include "Mesh.h"
#include "ObjLoader.h"
#include <iostream>

ResourceHandler::ResourceHandler()
{
}

ResourceHandler::~ResourceHandler()
{
}

void ResourceHandler::ProcessMessages(MessageSystem::Message* aMessage)
{
	std::string& message = aMessage->message;

	// Skip message type since it'll be handled by the messagehandler - focus on the message itself
	
	if (message == "StartProcess1")
	{
		// StartProcess1();
	}
	
}

void ResourceHandler::CreateShader(const char* aVertexPath, const char* aFragmentPath, std::string aName)
{
	if (myShaders.find(aName) != myShaders.end())
	{
		std::cerr << "Shader with name '" << aName << "' already exists!\n";
		return;
	}

	Shader* newShader = new Shader(aName, aVertexPath, aFragmentPath);

	if (!newShader)
	{
		std::cerr << "Failed to create shader: " << aName << std::endl;
		return;
	}

	myShaders[aName] = newShader; // Store shader with the correct name
	std::cout << "Shader '" << aName << "' created successfully!" << std::endl;
}

void ResourceHandler::CreateTexture(const char* aTexturePath, bool shouldAlpha, std::string aName)
{
	Texture* newTexture = new Texture(aTexturePath, shouldAlpha);

	myTextures.emplace(aName, newTexture);
}

void ResourceHandler::CreateMesh(const char* aModelPath, std::string aName)
{
	Mesh* newMesh = DotsRendering::LoadObjMesh(aModelPath);

	myMeshes.emplace(aName, newMesh);
}

void ResourceHandler::RegisterMesh(Mesh* aMesh, std::string meshName)
{
	myMeshes.emplace(meshName, aMesh);
}

void ResourceHandler::RegisterShader(Shader* aShader)
{
	myShaders.emplace(aShader->GetName(), aShader);
}

Shader* ResourceHandler::GetShader(std::string aName)
{
	return myShaders[aName];
}

Texture* ResourceHandler::GetTexture(std::string aName)
{
	return myTextures[aName];
}

Mesh* ResourceHandler::GetMesh(std::string aName)
{
	return myMeshes[aName];
}

const std::string ResourceHandler::GetShaderName(Shader* aShader)
{
	// Look through the unordered_map for the shader with the same pointer
	for (auto& shader : myShaders)
	{
		if (shader.second == aShader) // Check for same val
		{
			return shader.first; //Return the name of the shader
		}
	}
}

const std::string ResourceHandler::GetTextureName(Texture* aTexture)
{
	for (auto& texture : myTextures)
	{
		if (texture.second == aTexture) {return texture.first;}
	}
}

const std::string ResourceHandler::GetMeshName(Mesh* aMesh)
{
	for (auto& mesh : myMeshes)
	{
		if (mesh.second == aMesh) { return mesh.first; }
	}
}

std::vector<std::string> ResourceHandler::GetAllResources()
{
	std::vector<std::string> ReturnVector;

	for (auto& m : myMeshes)
	{
		ReturnVector.push_back(m.first);
	}

	for (auto& s : myShaders)
	{
		ReturnVector.push_back(s.first);
	}

	for (auto& t : myTextures)
	{
		ReturnVector.push_back(t.first);
	}

	return ReturnVector;
}
