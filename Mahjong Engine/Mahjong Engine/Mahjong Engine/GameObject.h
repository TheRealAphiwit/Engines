#pragma once
#include <glm.hpp>
#include <memory>
#include <string>

#include "../Winds/Collisions.h"
#include "VirtualObject.h"
#include "EntityHandler.h"

struct ColliderData
{
	float Mass;
	float Radius;
	glm::vec3 Extents;
	glm::vec3 Center;
	bool HasGravity;
	bool IsKinematic;
};

namespace Winds
{
	class Collider;
	class SphereCollider;
	class BoxCollider;
}

class VirtualObject;

class GameObject
{
public:
	GameObject(VirtualObject* anObject, Winds::Collider* aCollider);
	GameObject(std::shared_ptr<std::string> name, Mesh* mesh, Texture* texture, Shader* shader, Winds::Collider* aCollider);
	~GameObject();

	void Update();
	VirtualObject* GetVirtualObject() { return myVirtualObject; };

	// -----------------------Collision properties-----------------------

	Winds::Collider* GetCollider();

	void CreateSphereCollider(const float& aRadius);
	void CreateBoxCollider(const glm::vec3 someExtents);

	void SetData(const ColliderData& colData);
	const ColliderData& GetData();
	bool IsSphereCollider();
	bool IsBoxCollider();

	void RemoveCollider();

private:
	// Self refs
	Winds::Collider* myCollider;
	VirtualObject* myVirtualObject;
};