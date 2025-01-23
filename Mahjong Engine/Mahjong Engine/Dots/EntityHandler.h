#pragma once
#include <vector>
#include <memory>
#include <string>
#include <thread>
#include <future>
#include <mutex>
#include "Messages.h"

// Needed components for entity handling
class VirtualObject;
class Mesh;
class Texture;
class Shader;

namespace DotsRendering
{
	class EntityHandler
	{
	public:
		// Singelton
		static EntityHandler& GetInstance()
		{
			static EntityHandler instance;
			return instance;
		}

		// Prevent copy and overwriting
		EntityHandler(const EntityHandler&) = delete;
		EntityHandler& operator=(const EntityHandler&) = delete;

		void ProcessMessages(MessageSystem::Message* aMessage);

		void Initialize();
		void CreateVirtualObject(std::shared_ptr<std::string> name, Mesh* aMesh, Texture* aTexture, Shader* aShader); // NEW
		void CreateVirtualObject(std::shared_ptr<std::string> name, std::string meshName, std::string textureName, std::string shaderName); // ALT VERSION
		// void ThreadCreateVirtualObject(std::shared_ptr<std::string> name, Mesh* aMesh, Texture* aTexture, Shader* aShader);
		std::future<VirtualObject*> ThreadCreateVirtualObject(std::shared_ptr<std::string> name, Mesh * aMesh, Texture * aTexture, Shader * aShader);
		void DeleteVirtualObject(VirtualObject* object);
		void CreateDefaultCube();
		void CreateDefaultSphere();

		std::vector<VirtualObject*> GetObjects();

	private:
		EntityHandler();
		~EntityHandler();

		std::vector<VirtualObject*> myObjects;

		// New thread stuff
		std::vector<std::shared_ptr<VirtualObject>> myObjects_sPtr;
		std::mutex myObjectsMutex;
	};
}