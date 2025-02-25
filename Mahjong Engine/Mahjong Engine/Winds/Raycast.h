#pragma once
#include <glm.hpp>
#include <iostream>

namespace Winds
{
	class Collider;
}

class Ray
{
public:
	glm::vec3 Origin;
	glm::vec3 Direction;

	Ray(const glm::vec3& anOrigin, const glm::vec3& aDirection)
	{
		Origin = anOrigin;
		Direction = aDirection;
	}
};

struct RayHit
{
	glm::vec3 Point;
	Winds::Collider* Collider;
	float Distance;

	RayHit() {};
	RayHit(glm::vec3 aPoint, Winds::Collider* aCollider, float aDistance)
	{
		Point = aPoint;
		Collider = aCollider;
		Distance = aDistance;
	}
};