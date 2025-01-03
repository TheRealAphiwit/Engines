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
	switch (aMessage->type)
	{
		case MessageSystem::MessageType::String:
		{
			if (message == "StartProcess1")
			{
				// StartProcess1();
			}
			else if (message == "StopProcess1")
			{
				// StopProcess1();
			}

			// Don't know exactly what this does
			/*std::string name = message.substr(0, message.find(" "));
			std::string path = message.substr(message.find(" ") + 1, message.size());
			if (name.find("Shader") != std::string::npos)
			{
				std::string vertexPath = path.substr(0, path.find(" "));
				std::string fragmentPath = path.substr(path.find(" ") + 1, path.size());
				CreateShader(vertexPath.c_str(), fragmentPath.c_str(), name);
			}
			else if (name.find("Texture") != std::string::npos)
			{
				std::string texturePath = path.substr(0, path.find(" "));
				std::string alpha = path.substr(path.find(" ") + 1, path.size());
				CreateTexture(texturePath.c_str(), alpha == "true", name);
			}
			else if (name.find("Mesh") != std::string::npos)
			{
				CreateMesh(path.c_str(), name);
			}
			break;*/
		}

		case MessageSystem::MessageType::Raycast:
		{
			// Do raycast immediately and reply result
			// or queue it up for processing
			// It's up to us when we reply asynchronusly
			// with a messaging system like this
			break;
		}
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
