#include "EntityHandler.h"
#include "VirtualObject.h"
#include "Shader.h"
#include "Triangle.h"
#include "Cube.h"
#include "Square.h"
#include "DotGraphics.h"

#include "../Mahjong Engine/ResourceHandler.h"

ResourceHandler& resourceHandler = ResourceHandler::GetInstance();

DotsRendering::EntityHandler::EntityHandler()
{
	myCube = new Cube();
	myTexture = resourceHandler.GetTexture("Grass");
	myShader = resourceHandler.GetShader("Basic");
}

void DotsRendering::EntityHandler::CreateVirtualObject(std::shared_ptr<std::string> name, Mesh* aMesh, Texture* aTexture, Shader* aShader)
{
	// How I currently have done it
	VirtualObject* newObject = new VirtualObject(name, aMesh, aTexture, aShader);

	// This currently doesn't work since type diff
	newObject->SetMeshName(resourceHandler.GetMeshName(aMesh));
	newObject->SetTextureName(resourceHandler.GetTextureName(aTexture));
	newObject->SetShaderName(resourceHandler.GetShaderName(aShader));
	myObjects.push_back(newObject);
}

void DotsRendering::EntityHandler::CreateVirtualObject(std::shared_ptr<std::string> name, std::string meshName, std::string textureName, std::string shaderName)
{
	// This is the way I want to do it
	VirtualObject* newObject = new VirtualObject(name, resourceHandler.GetMesh(meshName), resourceHandler.GetTexture(textureName), resourceHandler.GetShader(shaderName));
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
