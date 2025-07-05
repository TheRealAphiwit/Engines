#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "ObjectEntry.h"
#include "VirtualObject.h"
#include <iostream>
#include <gtc/constants.hpp>
#include "Shader.h"
#include <stdio.h>
#include <string.h>
#include <vector>
#include "EntityHandler.h"
#include "../Mahjong Engine/ResourceHandler.h"

Characters::ObjectEntry::ObjectEntry()
{
	myObject = nullptr;
	Opened = false;
}

Characters::ObjectEntry::ObjectEntry(VirtualObject* anObject)
{
	myObject = anObject;
	Opened = false; // #? Should jaybe be set to true
}

Characters::ObjectEntry::~ObjectEntry()
{
}

void Characters::ObjectEntry::Update()
{
	if (myObject == nullptr)
	{
		std::cout << "[EDITOR] Object entry has null object" << std::endl;
		return;
	}

	char nameBuffer[128]; // Create buffer for chars
	strncpy_s(nameBuffer, myObject->GetName().c_str(), sizeof(nameBuffer)); // Copying the name to the buffer
	nameBuffer[sizeof(nameBuffer) - 1] = '\0'; // Using null-termination to prevent buffer overflow

	#pragma region Transform

	float* pos[] =
	{
		&myObject->Position.x,
		&myObject->Position.y,
		&myObject->Position.z,
	};

	float* rot[] =
	{
		&myObject->Rotation.x,
		&myObject->Rotation.y,
		&myObject->Rotation.z,
	};

	float* scale[] =
	{
		&myObject->Scale.x,
		&myObject->Scale.y,
		&myObject->Scale.z,
	};
#pragma endregion

	// Creates our inputs fields
	if (ImGui::InputText("Name", nameBuffer, sizeof(nameBuffer))) // Is a if statement cause we want to check if the name has been changed
	{
		*myObject->GetNamePtr() = nameBuffer;
	}
	ImGui::InputFloat3("Position ", *pos);
	ImGui::InputFloat3("Scale ", *scale);
	ImGui::SliderFloat3("Rotation ", *rot, -glm::pi<float>(), glm::pi<float>());

#pragma region Model
	static std::string selectedModel = myObject->GetModelName();
	std::unordered_map<std::string, Mesh*> meshes = ResourceHandler::GetInstance().GetMeshes();

	if (ImGui::BeginCombo("Model", selectedModel.c_str()))
	{
		for (auto& mesh : meshes)
		{
			bool isSelected = (selectedModel == mesh.first);
			if (ImGui::Selectable(mesh.first.c_str(), isSelected))
			{
				selectedModel = mesh.first;
				myObject->SetMesh(*mesh.second, selectedModel);
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
	static std::string selectedTextureName = myObject->GetTextureName();
	std::unordered_map<std::string, Texture*> textures = ResourceHandler::GetInstance().GetTextures();

	if (ImGui::BeginCombo("Texture", selectedTextureName.c_str()))
	{
		for (auto& texture : textures)
		{
			bool isSelected = (selectedTextureName == texture.first);
			if (ImGui::Selectable(texture.first.c_str(), isSelected))
			{
				selectedTextureName = texture.first;
				myObject->SetTexture(*texture.second, selectedTextureName);
			}
			if (isSelected)
			{
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}
#pragma endregion

#pragma region DeleteButton
	if (ImGui::Button("Delete Object"))
	{
		DotsRendering::EntityHandler::GetInstance().DeleteVirtualObject(myObject);
		myObject = nullptr;
		Opened = false;
	}
#pragma endregion
}