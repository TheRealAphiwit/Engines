#include "ResourceHandler.h"
#include "Shader.h"
#include "Texture.h"
#include "Mesh.h"
#include "ObjLoader.h"

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
	Shader* newShader = new Shader(aVertexPath, aFragmentPath);
	if (newShader->myShaderProgram != 0)
	{
		myShaders.emplace(aName, newShader);
	}
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
