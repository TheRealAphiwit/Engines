#include "Winds_Physics.h"
#include "../Mahjong Engine/DragonEngine.h" // Not so much for these
#include "../Mahjong Engine/GameObject.h"
#include "Collisions.h"
#include "Intersections.h"
#include <iostream>
#include "VirtualObject.h" // This is fine
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
		main_plane->Position = glm::vec3(0, 0, 0);
	}

	void Winds_Physics::Simulate(const float aDeltaTime)
	{
		float limitDt = glm::min(aDeltaTime, 0.01f);

		colliders = UpdatePhysicsScene();
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
			if (!c->IsKinematic && c->HasGravity)
			{
				c->Velocity += glm::vec3(0, GravityMultiplier, 0) * dt;
			}
		}
	}

	void Winds_Physics::ApplyVelocity(std::vector<Collider*> colliders, const float& dt)
	{
		for (Collider* c : colliders)
		{
			if (!c->IsKinematic)
			{
				c->Position += c->Velocity * dt;
				c->Transform[3] = glm::vec4(c->Position, 1.0f);

				float maxAngularVelocity = 3;
				if (glm::length(c->AngularVelocity) > maxAngularVelocity)
				{
					c->AngularVelocity = glm::normalize(c->AngularVelocity) * maxAngularVelocity;
				}

				if (glm::length(c->AngularVelocity) > 0.0001f)
				{
					glm::vec3 angularVelocityNorm = glm::normalize(c->AngularVelocity);
					glm::quat angularRotation = glm::angleAxis(glm::length(c->AngularVelocity) * dt, angularVelocityNorm);
					glm::mat3 rotationDelta = glm::mat3_cast(angularRotation);
					c->Transform = glm::mat4(rotationDelta) * c->Transform;
				}

				if (c->Mass > 0)
				{
					c->Velocity *= glm::pow(1.0f - LinearDrag, dt);
				}

				if (c->Mass > 0)
				{
					c->AngularVelocity *= glm::exp(-AngularDrag * dt);
				}

				// inertia tensor in world space
				glm::mat3 rotationMatrix = glm::mat3(c->Transform);
				glm::mat3 inertiaTensorInWorldSpace = rotationMatrix * c->MomentOfInertia * glm::transpose(rotationMatrix);
				c->InverseMomentOfInertia = glm::inverse(inertiaTensorInWorldSpace);
			}
			else
			{
				c->Velocity = glm::vec3(0, 0, 0);
				c->AngularVelocity = glm::vec3(0, 0, 0);
			}
		}
	}

	void Winds_Physics::HandleCollisions(std::vector<Collision> collisions)
	{
		std::vector<Collision> dynamicDynamicCollisions;
		std::vector<Collision> staticDynamicCollisions;

		for (Collision c : collisions)
		{
			bool A_isDynamic = !c.Col1->IsKinematic;
			bool B_isDynamic = !c.Col2->IsKinematic;

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


		for (GameObject* c : myEngine->GetGameObject())
		{
			Collider* col = c->GetCollider();
			if (col != nullptr)
			{
				glm::mat4 trans = c->GetVirtualObject()->GetTrans();

				col->Transform = trans;
				col->Position = glm::vec3(trans[3]);

				cols.push_back(col);
			}
		}

		return cols;
	}
	std::vector<Collision> Winds_Physics::CheckIntersections(std::vector<Collider*> colliders)
	{
		std::vector<Collision> collisions;

		int count = colliders.size();

		for (int i = 0; i < count; i++)
		{
			for (int j = i + 1; j < count; j++)
			{
				Collision c = CheckIntersect(colliders[i], colliders[j]);
				if (c.Col1 != nullptr && c.Col2 != nullptr)
				{
					collisions.push_back(c);
				}
			}
		}
		return collisions;
	}
	void Winds_Physics::UpdateVisuals()
	{
		for (GameObject* c : myEngine->GetGameObjects())
		{
			Collider* col = c->GetCollider();
			if (col != nullptr)
			{
				c->GetVirtualObject()->SetTransform(col->Transform);
			}
		}
	}
}
