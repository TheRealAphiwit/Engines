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

	char nameBuffer[128];
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

#pragma region Texture
	std::vector<std::string> textures;
	// Get resourcehandler
	// Get all textures to the list
#pragma endregion

	// Creates our inputs fields
	if (ImGui::InputText("Name", nameBuffer, sizeof(nameBuffer))) // Is a if statement cause we want to check if the name has been changed
	{
		*myObject->GetNamePtr() = nameBuffer;
	}

	ImGui::InputFloat3("Position ", *pos);
	ImGui::InputFloat3("Scale ", *scale);
	ImGui::SliderFloat3("Rotation ", *rot, -glm::pi<float>(), glm::pi<float>());

	// Texture selection - this is a temp version currently
	/*if (ImGui::BeginCombo("Texture", textures[0].c_str()))
	{
		for (int n = 0; n < textures.size(); n++)
		{
			const bool is_selected = (textures[0] == textures[n]);
			if (ImGui::Selectable(textures[n].c_str(), is_selected))
			{
				textures[0] = textures[n];
			}

			if (is_selected)
			{
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}*/
}