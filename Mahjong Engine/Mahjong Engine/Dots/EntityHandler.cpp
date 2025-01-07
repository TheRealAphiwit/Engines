#include "EntityHandler.h"
#include "VirtualObject.h"
#include "Shader.h"
#include "Triangle.h"
#include "Cube.h"
#include "Square.h"
#include "DotGraphics.h"

#include "../Mahjong Engine/ResourceHandler.h"
#include "../Utility_Library/MessageHandler.h"

DotsRendering::EntityHandler::EntityHandler()
{
	
}

DotsRendering::EntityHandler::~EntityHandler()
{
}

void DotsRendering::EntityHandler::ProcessMessages(MessageSystem::Message* aMessage)
{
	std::string& message = aMessage->message;

	if (message == "CreateDefaultCube")
	{
		CreateDefaultCube();
	}
	else if (message == "CreateDefaultSphere")
	{
		CreateDefaultSphere();
	}
}

void DotsRendering::EntityHandler::Initialize()
{
	// Move this to EntityHandler::Initialize() to see if it works
	for (size_t i = 0; i < 3; i++)
	{
		CreateVirtualObject(std::make_shared<std::string>("Cube"), myCube, myTexture, myShader);
	}

	// Send request to create default cube
	MessageSystem::MessageHandler::GetInstance().CreateMessage("Entity", "Request: Create Default Cube Entity");
}

void DotsRendering::EntityHandler::CreateVirtualObject(std::shared_ptr<std::string> name, Mesh* aMesh, Texture* aTexture, Shader* aShader)
{
	// How I currently have done it
	VirtualObject* newObject = new VirtualObject(name, aMesh, aTexture, aShader);

	// This currently doesn't work since type diff
	newObject->SetMeshName(ResourceHandler::GetInstance().GetMeshName(aMesh));
	newObject->SetTextureName(ResourceHandler::GetInstance().GetTextureName(aTexture));
	newObject->SetShaderName(ResourceHandler::GetInstance().GetShaderName(aShader));
	myObjects.push_back(newObject);
}

void DotsRendering::EntityHandler::CreateVirtualObject(std::shared_ptr<std::string> name, std::string meshName, std::string textureName, std::string shaderName)
{
	// This is the way I want to do it
	VirtualObject* newObject = new VirtualObject(name, ResourceHandler::GetInstance().GetMesh(meshName), ResourceHandler::GetInstance().GetTexture(textureName), ResourceHandler::GetInstance().GetShader(shaderName));
	newObject->SetMeshName(meshName);
	newObject->SetTextureName(textureName);
	newObject->SetShaderName(shaderName);
	myObjects.push_back(newObject);
}

void DotsRendering::EntityHandler::DeleteVirtualObject(VirtualObject* object)
{
	if (!object) { return; }

	for (int i = 0; i < myObjects.size(); i++)
	{
		if (myObjects[i] == object)
		{
			myObjects.erase(myObjects.begin() + i);
			delete object;
			return;
		}
	}
}

void DotsRendering::EntityHandler::CreateDefaultCube()
{
	VirtualObject* newObject = new VirtualObject(std::make_shared<std::string>("Cube"), myCube, myTexture, myShader);
	myObjects.push_back(newObject);
}

void DotsRendering::EntityHandler::CreateDefaultSphere()
{
	VirtualObject* newObject = new VirtualObject(std::make_shared < std::string>("Sphere"), mySphere, myTexture, myShader);
	myObjects.push_back(newObject);
}

std::vector<VirtualObject*> DotsRendering::EntityHandler::GetObjects()
{
	return myObjects;
}
