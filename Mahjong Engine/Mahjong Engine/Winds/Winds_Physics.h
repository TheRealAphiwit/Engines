#pragma once

#include <glm.hpp>
#include <vector>
#include <optional>

namespace Engine
{
	class DragonEngine;
}

namespace Winds
{
	class Winds_Physics
	{
	public:
		Winds_Physics();
		void Simulate(const float deltaTime);

		void ApplyGravity();
		void ApplyVelocity();

		void HandleCollisions();
		void HandleStaticDynamic();
		void HandleDynamicDynamic();

		void UpdateVisuals();

	private:
		Engine::DragonEngine* myEngine;
	};
}