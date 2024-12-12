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

Characters::ObjectEntry::ObjectEntry()
{
	myObject = nullptr;
	Opened = false;
}

Characters::ObjectEntry::ObjectEntry(VirtualObject* anObject)
{
	myObject = anObject;
	Opened = false;
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

	// Creates our inputs fields
	ImGui::InputFloat3("Position ", *pos);
	ImGui::InputFloat3("Scale ", *scale);
	ImGui::SliderFloat3("Rotation ", *rot, -glm::pi<float>(), glm::pi<float>());
}