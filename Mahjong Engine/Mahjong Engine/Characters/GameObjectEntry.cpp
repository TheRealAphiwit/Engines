#include "imgui.h"
#include "GameObjectEntry.h"
#include "GameObject.h"
#include "VirtualObject.h"
#include "../Winds/Collisions.h"
#include <iostream>
#include <gtc/constants.hpp>
#include <EntityHandler.h>
#include <GameObjectHandler.h>
#include "../Mahjong Engine/ResourceHandler.h"
#include "../Winds/Winds_Physics.h"
#include <LightComponent.h>

Characters::GameObjectEntry::GameObjectEntry() : myGameObject(nullptr), Opened(false) {}

Characters::GameObjectEntry::GameObjectEntry(GameObject* aGameObject) : myGameObject(aGameObject), Opened(false) {}

Characters::GameObjectEntry::~GameObjectEntry() {}

void Characters::GameObjectEntry::Update()
{
	if (!myGameObject || !myGameObject->GetVirtualObject())
	{
		std::cout << "[EDITOR] GameObjectEntry has null object" << std::endl;
		return;
	}

	VirtualObject* vObject = myGameObject->GetVirtualObject();
	ColliderData colData = myGameObject->GetData(); // This is great - maybe we can make a way to set collider data later

	char nameBuffer[128];
	strncpy_s(nameBuffer, vObject->GetName().c_str(), sizeof(nameBuffer));
	nameBuffer[sizeof(nameBuffer) - 1] = '\0';

#pragma region Transform
	float* pos[] = { &vObject->Position.x, &vObject->Position.y, &vObject->Position.z };
	float* rot[] = { &vObject->Rotation.x, &vObject->Rotation.y, &vObject->Rotation.z };
	float* scale[] = { &vObject->Scale.x, &vObject->Scale.y, &vObject->Scale.z };
#pragma endregion

	// Name field
	if (ImGui::InputText("Name", nameBuffer, sizeof(nameBuffer)))
	{
		*vObject->GetNamePtr() = nameBuffer;
	}

	// Transform controls
	ImGui::InputFloat3("Position", *pos);
	ImGui::InputFloat3("Scale", *scale);
	ImGui::SliderFloat3("Rotation", *rot, -glm::pi<float>(), glm::pi<float>());

#pragma region Model
	static std::string selectedModel = vObject->GetModelName();
	std::unordered_map<std::string, Mesh*> meshes = ResourceHandler::GetInstance().GetMeshes();

	if (ImGui::BeginCombo("Model", selectedModel.c_str()))
	{
		for (auto& mesh : meshes)
		{
			bool isSelected = (selectedModel == mesh.first);
			if (ImGui::Selectable(mesh.first.c_str(), isSelected))
			{
				selectedModel = mesh.first;
				vObject->SetMesh(*mesh.second, selectedModel);
			}
			if (isSelected)
			{
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}
#pragma endregion

#pragma region Texture
	static std::string selectedTextureName = vObject->GetTextureName();
	std::unordered_map<std::string, Texture*> textures = ResourceHandler::GetInstance().GetTextures();

	if (ImGui::BeginCombo("Texture", selectedTextureName.c_str()))
	{
		for (auto& texture : textures)
		{
			bool isSelected = (selectedTextureName == texture.first);
			if (ImGui::Selectable(texture.first.c_str(), isSelected))
			{
				selectedTextureName = texture.first;
				vObject->SetTexture(*texture.second, selectedTextureName);
			}
			if (isSelected)
			{
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}
#pragma endregion

#pragma region Physics
	ImGui::Separator();
	ImGui::Text("Physics Properties");

	// Add selectable collider type - this is super rushed and hard coded but I need to get this done for demo #1
	static std::string selectedCollider = myGameObject->GetColliderName();
	if (ImGui::BeginCombo("Collider", selectedCollider.c_str()))
	{
		if (ImGui::Selectable("None"))
		{
			std::string nullName = "Null";
			myGameObject->SetCollider(nullptr, nullName);
		}

		if (ImGui::Selectable("Box"))
		{
			std::string boxName = "Box";
			myGameObject->SetCollider(new Winds::BoxCollider(colData.Center, glm::vec3(0.5, 0.5, 0.5)), boxName); // Hardcoded extents for demo
		}

		// Demo #1
		if (ImGui::Selectable("Sphere"))
		{
			std::string sphereName = "Sphere";
			myGameObject->SetCollider(new Winds::SphereCollider(colData.Center, 0.5f), sphereName); // Hardcoded for demo

			// Print current collider type
			std::cout << "[EDITOR] Changed collider to Sphere" << std::endl;
			std::cout << "[EDITOR] Collider Radius: " << colData.Radius << std::endl;
		}

		ImGui::EndCombo();
	}

	if (ImGui::Checkbox("Is Kinematic", &colData.IsKinematic))
	{
		myGameObject->SetData(colData);
	}

	if (ImGui::Checkbox("Has Gravity", &colData.HasGravity))
	{
		myGameObject->SetData(colData);
	}

	if (ImGui::DragFloat("Mass", &colData.Mass, 0.1f, 0.1f, 100.0f))
	{
		myGameObject->SetData(colData);
	}
#pragma endregion

#pragma region Lighting
	ImGui::Separator();
	ImGui::Text("Lighting");

	// Check if object already has a light
	LightComponent* light = vObject->GetComponent<LightComponent>();

	bool hasLight = (light != nullptr);

	// Toggle light on/off
	if (ImGui::Checkbox("Enable Light", &hasLight))
	{
		if (hasLight && !light)
		{
			light = vObject->AddComponent<LightComponent>();
			light->Owner = vObject;
		}
		else if (!hasLight && light)
		{
			light->Intensity = 0.0f;
		}
	}

	if (light)
	{
		// --- Light Type ---
		const char* lightTypes[] = { "Point", "Directional", "Spot" };
		int currentType = static_cast<int>(light->LightType);

		if (ImGui::Combo("Light Type", &currentType, lightTypes, IM_ARRAYSIZE(lightTypes)))
		{
			light->LightType = static_cast<ELightType>(currentType);
		}

		// --- Common properties ---
		ImGui::ColorEdit3("Color", &light->Color.x);
		ImGui::DragFloat("Intensity", &light->Intensity, 0.1f, 0.0f, 100.0f);

		// --- Point & Spot ---
		if (light->IsPoint() || light->IsSpot())
		{
			ImGui::DragFloat("Range", &light->Range, 0.1f, 0.1f, 1000.0f);
		}

		// --- Spot only ---
		if (light->IsSpot())
		{
			ImGui::DragFloat("Inner Cone", &light->InnerCone, 0.01f, 0.0f, glm::pi<float>());
			ImGui::DragFloat("Outer Cone", &light->OuterCone, 0.01f, 0.0f, glm::pi<float>());
		}

		// --- Directional only ---
		if (light->IsDirectional())
		{
			ImGui::DragFloat3("Direction", &light->LocalDirection.x, 0.01f);
		}
	}
#pragma endregion
}