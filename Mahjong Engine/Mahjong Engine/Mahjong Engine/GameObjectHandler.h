#pragma once
#include "GameObject.h"
#include "Messages.h"

#include <vector>
#include <future>
#include <mutex>
#include <thread>

class GameObject;
class VirtualObject;
class Mesh;
class Texture;
class Shader;

namespace Winds
{
	class Collider;
	class SphereCollider;
	class BoxCollider;
}

namespace Engine
{
	class GameObjectHandler
	{
	public:
		#pragma region Singeton Setup
		// Singelton
		static GameObjectHandler& GetInstance()
		{
			static GameObjectHandler instance;
			return instance;
		}

		GameObjectHandler(const GameObjectHandler&) = delete;
		GameObjectHandler& operator=(const GameObjectHandler&) = delete;
		#pragma endregion

		void ProcessMessages(MessageSystem::Message* aMessage);

		std::future<GameObject*> CreateGameObject(std::shared_ptr<std::string> name, Mesh* mesh, Texture* texture, Shader* shader, Winds::Collider* collider);
		std::future<void> DeleteGameObject(GameObject* object);
		std::future<GameObject*> CreateDefaultCube();

		std::vector<GameObject*> GetObjects();

	private:
		GameObjectHandler();
		~GameObjectHandler();

		std::vector<GameObject*> myObjects;
		std::vector<std::shared_ptr<VirtualObject>> myObjects_sPtr;
		std::mutex myObjectsMutex;
	};
}