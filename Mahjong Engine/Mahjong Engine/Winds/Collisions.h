#pragma once
#include <glm.hpp>
#include <iostream>

namespace Winds
{
	glm::mat3 ComputeMomentOfInertiaBox(float mass, glm::vec3 extents);
	glm::mat3 ComputeMomentOfInertiaSphere(float mass, float radius);

	// This class does not init all vals as it is to be inherited from
	class Collider
	{
	public: virtual ~Collider() {}
		  template<typename T>
		  bool isOf() { return dynamic_cast<T*>(this) != nullptr; }
	
		  glm::vec3 center;
		  glm::vec3 position;
		  glm::mat4 transform;

		  glm::mat3 momentOfInertia;
		  glm::mat3 inverseMomentOfInertia;

		  bool hasGravity;
		  bool isKinematic;
		  glm::vec3 velocity;
		  glm::vec3 angularVelocity;
		  float mass;
	};

	struct Collision
	{
		Collider* col1;
		Collider* col2;

		glm::vec3 point;
		glm::vec3 normal;
	};

	class SphereCollider : public Collider
	{
	public:
		float radius;

		SphereCollider(const glm::vec3& aCenter, const float& aRadius)
		{
			velocity = glm::vec3(0, 0, 0);
			angularVelocity = glm::vec3(0, 0, 0);
			position = glm::vec3(0, 0, 0);
			hasGravity = false;
			center = aCenter;
			radius = aRadius;
			mass = 1;

			momentOfInertia = ComputeMomentOfInertiaSphere(mass, radius);
			inverseMomentOfInertia = glm::inverse(momentOfInertia);
		}
	};

	class BoxCollider : public Collider
	{
	public:
		glm::vec3 extents;

		BoxCollider(const glm::vec3& aCenter, const glm::vec3& someExtents)
		{
			velocity = glm::vec3(0, 0, 0);
			angularVelocity = glm::vec3(0, 0, 0);
			position = glm::vec3(0, 0, 0);
			hasGravity = false;
			center = aCenter;
			extents = someExtents;
			mass = 1;

			momentOfInertia = ComputeMomentOfInertiaBox(mass, extents);
			inverseMomentOfInertia = glm::inverse(momentOfInertia);
		}
	};

	class PlaneCollider : public Collider
	{
	public:
		glm::vec3 normal;
		float distance;

		PlaneCollider(const glm::vec3& aNormal, float aDistance)
		{
			velocity = glm::vec3(0, 0, 0);
			angularVelocity = glm::vec3(0, 0, 0);
			position = glm::vec3(0, 0, 0);
			hasGravity = false;
			isKinematic = true;

			normal = glm::normalize(aNormal);
			distance = aDistance;
			mass = 1;
		}
	};
}