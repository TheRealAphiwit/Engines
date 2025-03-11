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
	static std::string selectedTextureName = vObject->GetTexureName();
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
}