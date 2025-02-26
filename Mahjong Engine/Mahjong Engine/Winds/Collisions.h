#pragma once
#include <glm.hpp>
#include <iostream>

namespace Winds
{
	glm::mat3 ComputeMomentOfInertiaBox(float mass, glm::vec3 extents, glm::vec3 aPosition);
	glm::mat3 ComputeMomentOfInertiaSphere(float mass, float radius);

	// This class does not init all vals as it is to be inherited from
	class Collider
	{
	public: virtual ~Collider() {}
		  template<typename T>
		  bool IsOf() { return dynamic_cast<T*>(this) != NULL; } // Will be used for dynamic/static checks
	
		  virtual void ComputeInertia() = 0;

		  glm::vec3 Center;
		  glm::vec3 Position;
		  glm::mat4 Transform;

		  glm::mat3 MomentOfInertia;
		  glm::mat3 InverseMomentOfInertia;

		  bool HasGravity;
		  bool IsKinematic;
		  glm::vec3 Velocity;
		  glm::vec3 AngularVelocity;
#pragma region Potential Upgrades
		  // linear drag
		  // angular drag
		  // friction
#pragma endregion
		  float Mass;
	};

	struct Collision
	{
		Collider* Col1;
		Collider* Col2;

		glm::vec3 Point;
		glm::vec3 Normal;
	};

	class SphereCollider : public Collider
	{
	public:
		float Radius;

		SphereCollider(const glm::vec3& aCenter, const float& aRadius)
		{
			Velocity = glm::vec3(0, 0, 0);
			AngularVelocity = glm::vec3(0, 0, 0);
			Position = glm::vec3(0, 0, 0);
			HasGravity = false;
			Center = aCenter;
			Radius = aRadius;
			Mass = 1;

			MomentOfInertia = ComputeMomentOfInertiaSphere(Mass, Radius);
			InverseMomentOfInertia = glm::inverse(MomentOfInertia);
		}

		void ComputeInertia() override {}
	};

	class BoxCollider : public Collider
	{
	public:
		glm::vec3 Extents;

		BoxCollider(const glm::vec3& aCenter, const glm::vec3& someExtents)
		{
			Velocity = glm::vec3(0, 0, 0);
			AngularVelocity = glm::vec3(0, 0, 0);
			Position = glm::vec3(0, 0, 0);
			HasGravity = false;
			Center = aCenter;
			Extents = someExtents;
			Mass = 1;
		}

		void ComputeInertia() override
		{
			MomentOfInertia = ComputeMomentOfInertiaBox(Mass, Extents, Position);
			InverseMomentOfInertia = glm::inverse(MomentOfInertia);
		}
	};

	class PlaneCollider : public Collider
	{
	public:
		glm::vec3 Normal;
		float Distance;

		PlaneCollider(const glm::vec3& aNormal, float aDistance)
		{
			Velocity = glm::vec3(0, 0, 0);
			AngularVelocity = glm::vec3(0, 0, 0);
			Position = glm::vec3(0, 0, 0);
			HasGravity = false;
			IsKinematic = true;

			Normal = glm::normalize(aNormal);
			Distance = aDistance;
			Mass = 1;
		}

		void ComputeInertia() {}
	};
}