#pragma once

#include <glm.hpp>
#include <vector>
#include <optional>
#include "Raycast.h"

const float GravityMultiplier = -9.82f;
const float Restitution = 0.2f;
const float LinearDrag = 0.32f;
const float AngularDrag = 0.000005f;

namespace Engine
{
	class Input;
	class DragonEngine;
}

namespace DotsRendering
{
	class Camera;
	class OpenGLFrameBuffer; // Empty frwd dec
}

namespace Winds
{
	class Collider;
	class PlaneCollider;
	struct Collision;
}

namespace Winds
{
	class Winds_Physics
	{
	public:
		Winds_Physics(Engine::DragonEngine* aEngine);
		void Simulate(const float aDeltaTime);

		void ApplyGravity(std::vector<Collider*> colliders, const float& dt);
		void ApplyVelocity(std::vector<Collider*> colliders, const float& dt);

		void HandleCollisions(std::vector<Collision> collisions);
		void HandleStaticDynamic(std::vector<Collision> collisions);
		void HandleDynamicDynamic(std::vector<Collision> collisions);

		std::vector<Collider*> UpdatePhysicsScene();
		std::vector<Collision> CheckIntersections(std::vector<Collider*> colliders);
		void UpdateVisuals();

		bool MahjongRaycast(const Ray& aRay, RayHit& aHit);

	private:
		std::vector<Collider*> colliders;
		Engine::DragonEngine* myEngine;

		PlaneCollider* main_plane;
	};
}