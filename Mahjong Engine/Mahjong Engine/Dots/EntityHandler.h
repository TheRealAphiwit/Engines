#pragma once
#include <vector>
#include <memory>
#include <string>

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

	private:
		EntityHandler();
		~EntityHandler();

		std::vector<VirtualObject*> myObjects;
	};
}