#include "Winds_Physics.h"
#include "../Mahjong Engine/DragonEngine.h"
// #include "GameObject.h"
#include "Collisions.h"
// #include "Intersections.h"
#include <iostream>
#include "../Dots/VirtualObject.h"
#include <gtc/matrix_transform.hpp>
// #include "MonkeyMath.h"
// #include "Raycast.h"
#include <gtx/string_cast.hpp>

namespace Winds
{
	Winds_Physics::Winds_Physics(Engine::DragonEngine* aEngine)
	{
		myEngine = aEngine;

		main_plane = new PlaneCollider(glm::vec3(0, 1, 0), 1);
		main_plane->position = glm::vec3(0, 0, 0);
	}

	void Winds_Physics::Simulate(const float aDeltaTime)
	{
		float limitDt = glm::min(aDeltaTime, 0.01f);

		//colliders = UpdatePhysicsScene();
		//std::vector<Collision> collisions = CheckIntersections(colliders);

		//ApplyGravity(colliders, limitDt);

		//HandleCollisions(collisions);

		//ApplyVelocity(colliders, limitDt);

		UpdateVisuals();
	}

	void Winds_Physics::ApplyGravity(std::vector<Collider*> colliders, const float& dt)
	{
		for (Collider* c : colliders)
		{
			if (!c->isKinematic && c->hasGravity)
			{
				c->velocity += glm::vec3(0, GravityMultiplier, 0) * dt;
			}
		}
	}

	void Winds_Physics::ApplyVelocity(std::vector<Collider*> colliders, const float& dt)
	{
		for (Collider* c : colliders)
		{
			if (!c->isKinematic)
			{
				std::cout << glm::to_string(c->angularVelocity) << std::endl;
				c->position += c->velocity * dt;
				c->transform[3] = glm::vec4(c->position, 1.0f);

				if (glm::length(c->angularVelocity) > 0.0001f)
				{
					glm::vec3 angularVelocityNorm = glm::normalize(c->angularVelocity);
					glm::quat angularRotation = glm::angleAxis(glm::length(c->angularVelocity) * dt, angularVelocityNorm);
					glm::mat3 rotationDelta = glm::mat3_cast(angularRotation);
					c->transform = glm::mat4(rotationDelta) * c->transform;
				}

				if (c->mass > 0)
				{
					c->velocity *= glm::pow(1.0f - LinearDrag, dt);
				}
				if (c->mass > 0)
				{
					c->angularVelocity *= glm::exp(-AngularDrag * dt);
				}

				glm::mat3 rotationMatrix = glm::mat3(c->transform);
				glm::mat3 inertiaTensorInWorldSpace = rotationMatrix * c->momentOfInertia * glm::transpose(rotationMatrix);
				c->inverseMomentOfInertia = glm::inverse(inertiaTensorInWorldSpace);
			}
			else
			{
				c->velocity = glm::vec3(0, 0, 0);
				c->angularVelocity = glm::vec3(0, 0, 0);
			}
		}
	}

	void Winds_Physics::HandleCollisions(std::vector<Collision> collisions)
	{
		std::vector<Collision> dynamicDynamicCollisions;
		std::vector<Collision> staticDynamicCollisions;

		for (Collision c : collisions)
		{
			bool A_isDynamic = !c.col1->isKinematic;
			bool B_isDynamic = !c.col2->isKinematic;

			if (A_isDynamic && B_isDynamic)
			{
				dynamicDynamicCollisions.push_back(c);
			}
			else
			{
				staticDynamicCollisions.push_back(c);
			}
		}

		HandleDynamicDynamic(dynamicDynamicCollisions);
		HandleStaticDynamic(staticDynamicCollisions);
	}

	std::vector<Collider*> Winds_Physics::UpdatePhysicsScene()
	{
		// Get colliders
		std::vector<Collider*> cols;
		cols.push_back(main_plane);

		/*for (GameObject* c : c->GetCollider())
		{

		}*/

		return std::vector<Collider*>();
	}


}
