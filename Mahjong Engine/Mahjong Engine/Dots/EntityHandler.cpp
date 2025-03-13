#include "EntityHandler.h"
#include "VirtualObject.h"
#include "Shader.h"
#include "Triangle.h"
#include "Cube.h"
#include "Square.h"
#include "DotGraphics.h"

#include "../Mahjong Engine/ResourceHandler.h"
#include "../Utility_Library/MessageHandler.h"
#include <iostream>

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
	else if (message == "ThreadCreateDefaultCube")
	{
		ThreadCreateDefaultCube();
	}
}

void DotsRendering::EntityHandler::Initialize()
{
	// Move this to EntityHandler::Initialize() to see if it works
	for (size_t i = 0; i < 3; i++)
	{
		// Normal version
		//CreateVirtualObject(std::make_shared<std::string>("Cube"), myCube, myTexture, myShader);

		// Thread version
		ThreadCreateVirtualObject(std::make_shared<std::string>("Cube"), myCube, myTexture, myShader);
	}

	// Send request to create default cube
	MessageSystem::MessageHandler::GetInstance().CreateMessage("Entity", "Request: Thread Create Default Cube Entity");
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

// Alt version
void DotsRendering::EntityHandler::CreateVirtualObject(std::shared_ptr<std::string> name, std::string meshName, std::string textureName, std::string shaderName)
{
	// This is the way I want to do it
	VirtualObject* newObject = new VirtualObject(name, ResourceHandler::GetInstance().GetMesh(meshName), ResourceHandler::GetInstance().GetTexture(textureName), ResourceHandler::GetInstance().GetShader(shaderName));
	newObject->SetMeshName(meshName);
	newObject->SetTextureName(textureName);
	newObject->SetShaderName(shaderName);
	myObjects.push_back(newObject);
}

// 1st version of threading
// void DotsRendering::EntityHandler::ThreadCreateVirtualObject(std::shared_ptr<std::string> name, Mesh* aMesh, Texture* aTexture, Shader* aShader)
//{
//}

std::future<VirtualObject*> DotsRendering::EntityHandler::ThreadCreateVirtualObject(std::shared_ptr<std::string> name, Mesh* aMesh, Texture* aTexture, Shader* aShader)
{
	return std::async
	(std::launch::async, [this, name, aMesh, aTexture, aShader]() -> VirtualObject* 
		{
			// Create the VirtualObject
			VirtualObject* newObject = new VirtualObject(name, aMesh, aTexture, aShader);

			// Lock and update the shared resource
			{
				std::lock_guard<std::mutex> lock(myObjectsMutex);
				std::cout << "Created VirtualObject: " << newObject->GetName() << std::endl;
				myObjects.push_back(newObject);
			}

			// Return the created object
			return newObject;
		}
	);

	// Usage example:
	/*
	auto future = EntityHandler::GetInstance().ThreadCreateVirtualObject(name, mesh, texture, shader);

	// Do some other work while the VirtualObject is being created...

	// Retrieve the created object when it's ready
	try {
		VirtualObject* createdObject = future.get(); // Blocks until the object is ready
		std::cout << "Created VirtualObject: " << createdObject->GetName() << std::endl;
	}
	catch (const std::exception& e) {
		std::cerr << "Error creating VirtualObject: " << e.what() << std::endl;
	}
	*/
}

void DotsRendering::EntityHandler::AddVirtualObject(VirtualObject* object)
{
	if (object == nullptr)
	{
		std::cout << "Error: Trying to add a nullptr VirtualObject to EntityHandler!\n";
		return;
	}

	std::lock_guard<std::mutex> lock(myObjectsMutex);
	myObjects.push_back(object);

	std::cout << "VirtualObject added: " << object << " Total objects: " << myObjects.size() << "\n";
}

std::future<void> DotsRendering::EntityHandler::ThreadDeleteVirtualObject(VirtualObject* object)
{
	return std::async
	(std::launch::async, [this, object]()
		{
			if (!object) return;

			// Lock mutex to safely access and modify myObjects
			std::lock_guard<std::mutex> lock(myObjectsMutex);

			// auto can be VirtualObject
			auto it = std::find(myObjects.begin(), myObjects.end(), object);
			if (it != myObjects.end())
			{
				myObjects.erase(it);
				delete object;
				std::cout << "Deleted VirtualObject\n";
			}
		}
	);
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

std::future<VirtualObject*> DotsRendering::EntityHandler::ThreadCreateDefaultCube()
{
	return std::async
	(
		std::launch::async, [this]() 
		{
			// Create the cube object
			VirtualObject* newObject = new VirtualObject(std::make_shared<std::string>("Cube"), myCube, myTexture, myShader);

			// Lock the mutex to safely modify the shared myObjects vector
			std::lock_guard<std::mutex> lock(myObjectsMutex);
			myObjects.push_back(newObject);

			return newObject; // Return the created VirtualObject
		}
	);
}

void DotsRendering::EntityHandler::CreateDefaultSphere()
{
	VirtualObject* newObject = new VirtualObject(std::make_shared < std::string>("Sphere"), mySphere, myTexture, myShader);
	myObjects.push_back(newObject);
}

std::vector<VirtualObject*> DotsRendering::EntityHandler::GetObjects()
{
	std::lock_guard<std::mutex> lock(myObjectsMutex);

	//Print out all objects
	for (int i = 0; i < myObjects.size(); i++)
	{
		std::cout << "Object: " << myObjects[i]->GetName() << std::endl;
	}

	return myObjects;
}
