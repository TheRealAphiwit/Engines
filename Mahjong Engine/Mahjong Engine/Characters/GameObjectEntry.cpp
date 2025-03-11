#include "imgui.h"
#include "GameObjectEntry.h"
#include "GameObject.h"
#include "VirtualObject.h"
#include "../Mahjong Engine/ResourceHandler.h"
#include <iostream>
#include <gtc/constants.hpp>
#include <EntityHandler.h>

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
	ColliderData colData = myGameObject->GetData();

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

#pragma region Physics
	ImGui::Separator();
	ImGui::Text("Physics Properties");

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

#pragma region DeleteButton
	if (ImGui::Button("Delete GameObject"))
	{
		DotsRendering::EntityHandler::GetInstance().DeleteVirtualObject(vObject);
		myGameObject = nullptr;
		Opened = false;
	}
#pragma endregion
}