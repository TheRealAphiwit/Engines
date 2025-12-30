#include "Winds_Physics.h"
#include "../Mahjong Engine/DragonEngine.h" // Not so much for these
#include "../Mahjong Engine/GameObject.h"
#include "Collisions.h"
#include "Intersections.h"
#include "../Mahjong Engine/GameObjectHandler.h"
#include <iostream>
#include "VirtualObject.h" // This is fine
#include <gtc/matrix_transform.hpp>
#include "MahjongMath.h"
#include "Raycast.h"
#include <gtx/string_cast.hpp>

namespace Winds
{
	void Winds_Physics::Initialize(Engine::DragonEngine* aEngine)
	{
		myEngine = aEngine;

		main_plane = new PlaneCollider(glm::vec3(0, 1, 0), 1);
		main_plane->Position = glm::vec3(0, 0, 0);
	}

	void Winds_Physics::Simulate(const float aDeltaTime)
	{
		float limitDt = glm::min(aDeltaTime, 0.02f);

		for (Collider* collider : colliders) {
			collider->ComputeInertia();
		}

		colliders = UpdatePhysicsScene();
		std::vector<Collision> collisions = CheckIntersections(colliders);

		// Raycast check by applying gravity to first hit object - Martin example
		Ray ray(glm::vec3(0, 0, 0), glm::vec3(0, 0, 1));
		RayHit hit;
		if (MahjongRaycast(ray, hit))
		{
			hit.Collider->HasGravity = true; // This is Martin's example of raycast usage - I made a std::cout inside MahjongRaycast to verify its hits
		}

		ApplyGravity(colliders, limitDt);

		HandleCollisions(collisions);

		ApplyVelocity(colliders, limitDt);

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

	void Winds_Physics::RemoveCollider(Collider* aCollider)
	{
		// NEW
		auto it = std::find(colliders.begin(), colliders.end(), aCollider);
		if (it != colliders.end())
		{
			colliders.erase(it);
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

	void Winds_Physics::HandleStaticDynamic(std::vector<Collision> collisions)
	{
		const float SlidingFriction = 0.5f;

		for (Collision c : collisions)
		{
			Collider* A = c.Col1;
			Collider* B = c.Col2;

			bool A_isDynamic = !A->IsKinematic;
			bool B_isDynamic = !B->IsKinematic;

			if (!A_isDynamic && !B_isDynamic) continue;

			Collider* dynamicCollider = A_isDynamic ? A : B;
			Collider* staticCollider = A_isDynamic ? B : A;

			glm::vec3 n = glm::normalize(c.Normal);
			glm::vec3 r = c.Point - dynamicCollider->Position;

			glm::vec3 v = dynamicCollider->Velocity + glm::cross(dynamicCollider->AngularVelocity, r);
			float vRelDotN = glm::dot(v, n);

			if (vRelDotN > 0) continue;

			float invMass = (dynamicCollider->Mass > 0) ? 1.0f / dynamicCollider->Mass : 0;
			glm::vec3 r_cross_n = glm::cross(r, n);
			float angularEffect = glm::dot(r_cross_n, dynamicCollider->InverseMomentOfInertia * r_cross_n);

			float impulseMagnitude = -(1 + Restitution) * vRelDotN / (invMass + angularEffect);
			glm::vec3 impulse = impulseMagnitude * n;

			// Apply impulse to linear velocity
			dynamicCollider->Velocity += impulse * invMass;

			// angular velocity (considering moment of inertia)
			dynamicCollider->AngularVelocity += dynamicCollider->InverseMomentOfInertia * glm::cross(r, impulse);

			// sliding friction
			glm::vec3 tangentVelocity = v - (n * glm::dot(v, n));
			if (glm::length(tangentVelocity) > 0.0001f)
			{
				glm::vec3 frictionDirection = -glm::normalize(tangentVelocity);
				glm::vec3 frictionImpulse = frictionDirection * SlidingFriction * glm::length(tangentVelocity);

				dynamicCollider->Velocity += frictionImpulse * invMass;
				dynamicCollider->AngularVelocity += dynamicCollider->InverseMomentOfInertia * glm::cross(r, frictionImpulse);
			}
		}
	}

	void Winds_Physics::HandleDynamicDynamic(std::vector<Collision> collisions)
	{
		for (Collision c : collisions)
		{
			glm::vec3 normal = c.Normal;

			glm::vec3 relativeVelocity = c.Col2->Velocity - c.Col1->Velocity;
			float velocityAlongNormal = glm::dot(relativeVelocity, normal);

			if (velocityAlongNormal > 0) continue;

			float impulse = (1 + Restitution) * velocityAlongNormal;

			glm::vec3 impulseVector = impulse * normal;

			c.Col1->Velocity += impulseVector;
			c.Col2->Velocity -= impulseVector;

			glm::vec3 r1 = c.Point - c.Col1->Position;
			glm::vec3 r2 = c.Point - c.Col2->Position;

			glm::vec3 torque1 = glm::cross(r1, impulseVector);
			glm::vec3 torque2 = glm::cross(r2, impulseVector);

			// Rotate! sort of....
			c.Col1->AngularVelocity += c.Col1->InverseMomentOfInertia * torque1;
			c.Col2->AngularVelocity -= c.Col2->InverseMomentOfInertia * torque2;
		}
	}

	std::vector<Collider*> Winds_Physics::UpdatePhysicsScene()
	{
		// Get colliders
		std::vector<Collider*> cols;
		cols.push_back(main_plane);

		for (GameObject* c : Engine::GameObjectHandler::GetInstance().GetObjects())
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
		for (GameObject* c : Engine::GameObjectHandler::GetInstance().GetObjects())
		{
			Collider* col = c->GetCollider();
			if (col != nullptr)
			{
				c->GetVirtualObject()->SetTransform(col->Transform);
			}
		}
	}
	bool Winds_Physics::MahjongRaycast(const Ray& aRay, RayHit& aHit)
	{
		// Martin version
		for (Collider* c : colliders)
		{
			if (CheckRayIntersect(aRay, c, aHit))
			{
				aHit.Collider = c;
				aHit.Point = glm::vec3(0, 0, 0);
				aHit.Distance = 10;

				std::cout << "Raycast hit collider at position: " << glm::to_string(c->Position) << std::endl;

				return true;
			}
		}
		return false;

		// Alt version
		/*bool hitAnything = false;
		float closestT = FLT_MAX;

		for (Collider* c : colliders)
		{
			RayHit hit;
			if (CheckRayIntersect(aRay, c, hit))
			{
				if (hit.Distance < closestT)
				{
					closestT = hit.Distance;
					aHit = hit;
					hitAnything = true;
				}
			}
		}
		return hitAnything;*/
	}

	// New UI collider select support functions - not needed?
	void Winds_Physics::RegisterDefaultColliders()
	{
		// Register cube collider
		RegisterCollider("Plane Collider", main_plane);
		RegisterCollider("Sphere Collider", new SphereCollider(glm::vec3(0, 0, 0), 1.0f));
	}

	void Winds_Physics::RegisterCollider(const std::string& name, Collider* collider)
	{
		RegisteredColliders[name] = collider;
	}
}
