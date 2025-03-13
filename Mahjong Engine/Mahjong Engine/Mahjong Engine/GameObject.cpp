#include "GameObject.h"

GameObject::GameObject(VirtualObject* anObject, Winds::Collider* aCollider) : myVirtualObject(anObject), myCollider(aCollider)
{
	myVirtualObject = anObject;
	myCollider = aCollider;
}

GameObject::GameObject(std::shared_ptr<std::string> name, Mesh* mesh, Texture* texture, Shader* shader, Winds::Collider* aCollider)
{
	myVirtualObject = new VirtualObject(name, mesh, texture, shader);
	myCollider = aCollider;

	// Register the same VirtualObject in EntityHandler
	DotsRendering::EntityHandler::GetInstance().AddVirtualObject(myVirtualObject);
}

GameObject::~GameObject()
{
	myCollider = nullptr;

	// Remove VirtualObject from EntityHandler before deleting it
	DotsRendering::EntityHandler::GetInstance().DeleteVirtualObject(myVirtualObject);
	delete myVirtualObject;
}

void GameObject::Update()
{
	// Updates the existing cols to virtual objects pos
	if (myCollider != nullptr)
	{
		myCollider->Position = myVirtualObject->Position;
	}
}

Winds::Collider* GameObject::GetCollider()
{
	return myCollider;
}

void GameObject::CreateSphereCollider(const float& aRadius)
{
	myCollider = new Winds::SphereCollider(myVirtualObject->Position, aRadius);
}

void GameObject::CreateBoxCollider(const glm::vec3 someExtents)
{
	myCollider = new Winds::BoxCollider(myVirtualObject->Position, someExtents);
}

void GameObject::SetData(const ColliderData& colData)
{
	// We'll haved to downcast type specific colliderss down to regular cols (myCollider = Collider) that we use to set vals with 'dynamic_cast'
	if (myCollider->IsOf<Winds::SphereCollider>())
	{
		Winds::SphereCollider* sc = dynamic_cast<Winds::SphereCollider*>(myCollider);
		sc->Mass = colData.Mass;
		sc->Center = colData.Center;
		sc->HasGravity = colData.HasGravity;
		sc->Radius = colData.Radius;
		sc->IsKinematic = colData.IsKinematic;
	}
	else if (myCollider->IsOf<Winds::BoxCollider>())
	{
		Winds::BoxCollider* bc = dynamic_cast<Winds::BoxCollider*>(myCollider);
		bc->Mass = colData.Mass;
		bc->Center = colData.Center;
		bc->HasGravity = colData.HasGravity;
		bc->Extents = colData.Extents;
		bc->IsKinematic = colData.IsKinematic;
	}
}

const ColliderData& GameObject::GetData()
{
	ColliderData d;

	// Returns diff type of data depending on collider type
	// Downcasting with dynamic_cast is used once again
	if (myCollider->IsOf<Winds::SphereCollider>())
	{
		Winds::SphereCollider* sc = dynamic_cast<Winds::SphereCollider*>(myCollider);
		d.Mass = sc->Mass;
		d.Center = sc->Center;
		d.HasGravity = sc->HasGravity;
		d.Radius = sc->Radius;
		d.IsKinematic = sc->IsKinematic;
	}
	else if (myCollider->IsOf<Winds::BoxCollider>())
	{
		Winds::BoxCollider* bc = dynamic_cast<Winds::BoxCollider*>(myCollider);
		d.Mass = bc->Mass;
		d.Center = bc->Center;
		d.HasGravity = bc->HasGravity;
		d.Extents = bc->Extents;
		d.IsKinematic = bc->IsKinematic;
	}

	return d;
}

bool GameObject::IsSphereCollider()
{
	return myCollider->IsOf<Winds::SphereCollider>();
}

bool GameObject::IsBoxCollider()
{
	return myCollider->IsOf<Winds::BoxCollider>();
}

void GameObject::RemoveCollider()
{
	delete myCollider;
	myCollider = nullptr;
}
