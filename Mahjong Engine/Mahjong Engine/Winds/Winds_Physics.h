#pragma once

#include <glm.hpp>
#include <mutex>
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

	class Winds_Physics
	{
	public:
		//Singelton setup
		static Winds_Physics& GetInstance()
		{
			static Winds_Physics instance;
			return instance;
		}

		Winds_Physics(Winds_Physics const&) = delete;
		Winds_Physics& operator=(Winds_Physics const&) = delete;

		void Initialize(Engine::DragonEngine* aEngine);
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

		std::vector<Collider*> GetColliders() { return colliders; }
		void AddCollider(Collider* aCollider) { colliders.push_back(aCollider); }

	private:
		Winds_Physics() = default;
		~Winds_Physics() = default;

		std::vector<Collider*> colliders;
		Engine::DragonEngine* myEngine = nullptr;
		PlaneCollider* main_plane = nullptr;
		std::mutex myMutex;
	};
}