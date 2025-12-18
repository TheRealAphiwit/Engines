#include "GameObject.h"
#include "Winds_Physics.h"
#include <algorithm>

GameObject::GameObject(VirtualObject* anObject, Winds::Collider* aCollider) : myVirtualObject(anObject), myCollider(aCollider)
{
	myVirtualObject = anObject;
	myCollider = aCollider;

	// Register the same VirtualObject in EntityHandler
	DotsRendering::EntityHandler::GetInstance().AddVirtualObject(myVirtualObject);

	// Add collider to Winds_Physics
	Winds::Winds_Physics::GetInstance().AddCollider(myCollider);
}

GameObject::GameObject(std::shared_ptr<std::string> name, Mesh* mesh, Texture* texture, Shader* shader, Winds::Collider* aCollider)
{
	myVirtualObject = new VirtualObject(name, mesh, texture, shader);
	myCollider = aCollider;

	// Register the same VirtualObject in EntityHandler
	DotsRendering::EntityHandler::GetInstance().AddVirtualObject(myVirtualObject);

	// Add collider to Winds_Physics
	Winds::Winds_Physics::GetInstance().AddCollider(myCollider);
}

GameObject::~GameObject()
{
	if (myCollider)
	{
		RemoveCollider();
	}

	if (myVirtualObject)
	{
		// Let EntityHandler handle deletion
		DotsRendering::EntityHandler::GetInstance().DeleteVirtualObject(myVirtualObject);
		myVirtualObject = nullptr;
	}
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

	// Add collider to Winds_Physics
	Winds::Winds_Physics::GetInstance().AddCollider(myCollider);
}

void GameObject::CreateBoxCollider(const glm::vec3 someExtents)
{
	myCollider = new Winds::BoxCollider(myVirtualObject->Position, someExtents);

	Winds::Winds_Physics::GetInstance().AddCollider(myCollider);
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

ColliderData GameObject::GetData()
{
	ColliderData d{};

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

void GameObject::MatchCollider()
{
	if (!myVirtualObject || !myCollider)
		return;

	Mesh* mesh = myVirtualObject->GetMesh();

	if (!mesh)
		return;

	// Updates collider pos to virtual object pos
	if (myCollider != nullptr)
	{
		myCollider->Position = myVirtualObject->Position;
		
		//// Update the collider bounds based on mesh
		//if (myCollider->IsOf<Winds::SphereCollider>())
		//{
		//	Winds::SphereCollider* sc = dynamic_cast<Winds::SphereCollider*>(myCollider);
		//	sc->Radius = glm::length(mesh->GetBoundingSphereRadius() * myVirtualObject->Scale);
		//}
		//else if (myCollider->IsOf<Winds::BoxCollider>())
		//{
		//	Winds::BoxCollider* bc = dynamic_cast<Winds::BoxCollider*>(myCollider);
		//	bc->Extents = mesh->GetBoundingBoxExtents() * myVirtualObject->Scale;
		//}
	}
}

void GameObject::RemoveCollider()
{
	Winds::Winds_Physics::GetInstance().RemoveCollider(myCollider);
	delete myCollider;
	myCollider = nullptr;
}
