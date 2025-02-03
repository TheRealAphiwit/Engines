#pragma once
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "EditorGUI.h"
#include <glm.hpp>
#include "VirtualObject.h"
#include <string>
#include "ObjectEntry.h"
#include "EntityHandler.h"
#include "ResourceEditor.h"
#include <iostream>

#define itoc(a) ((char*)(intptr_t)(a)) // Line here allows us to convert int's into char*'s

Characters::EditorGUI::EditorGUI(GLFWwindow* aWindow, ResourceHandler* aResourceHandler)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io; // void cast prevents unused variable warning and this row as a whole gives access to input and output data
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(aWindow, true); // Initializes ImGui to work with a GLFW window and OpenGL for rendering.
	ImGui_ImplOpenGL3_Init("#version 330");

	myResourceHandler = aResourceHandler;

	myCurrentEditor = ECurrentEditor::COUNT;

	myResourceEditor = new ResourceEditor(myResourceHandler);
}

Characters::EditorGUI::~EditorGUI()
{
	// std destructor logic
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

int selectedItem = -1;
bool alwaysTrue = true;

void Characters::EditorGUI::Render(std::vector<VirtualObject*> someObjects)
{
	ImGui_ImplOpenGL3_NewFrame(); // Graphics setup
	ImGui_ImplGlfw_NewFrame(); // Input setup
	ImGui::NewFrame(); // Creates a new frame - Allows us to customize the GUI

	ImGui::Begin("Mahjong!", &alwaysTrue, ImGuiWindowFlags_MenuBar);

	if (ImGui::BeginMenuBar()) // Checks if menu can be created before we go on with creating it
	{
		if (ImGui::BeginMenu("File")) // Enables menu bar creation of type "file"
		{
			// Here we create menu items
			if (ImGui::MenuItem("Shader Editor", "Ctrl+O")) { myCurrentEditor = ECurrentEditor::EShaderEditor; }
			if (ImGui::MenuItem("Resource Viewer", "Ctrl+S")) { myCurrentEditor = ECurrentEditor::EResourceViewer; }
			if (ImGui::MenuItem("Object Hierarchy", "Ctrl+W")) { myCurrentEditor = ECurrentEditor::EObjectHierarchy; }
			ImGui::EndMenu(); // Closes currrent menu context
		}
		ImGui::EndMenuBar(); // Closes menubar context
	}

	switch (myCurrentEditor)
	{
	case Characters::ECurrentEditor::EObjectHierarchy:

		UpdateHieracrhy(someObjects);

		break;
	case Characters::ECurrentEditor::EShaderEditor:
		break;
	case Characters::ECurrentEditor::EResourceViewer:
		myResourceEditor->Update();
		break;
	case Characters::ECurrentEditor::COUNT:
		break;
	default:
		break;
	}


	ImGui::End();
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Characters::EditorGUI::UpdateHieracrhy(std::vector<VirtualObject*> someObjects)
{
	if (ImGui::Button("Create Object"))
	{
		// Call the threaded function to create a default cube
		std::future<VirtualObject*> future = DotsRendering::EntityHandler::GetInstance().ThreadCreateDefaultCube();

		// Optionally, handle the future if needed (e.g., wait for result, check errors)
		try {
			VirtualObject* createdObject = future.get(); // Blocks until the object is ready
			std::cout << "Created VirtualObject: " << createdObject->GetName() << std::endl;
		}
		catch (const std::exception& error) {
			std::cerr << "Error creating VirtualObject: " << error.what() << std::endl;
		}
	}

	// Check if existing items are correct
	if (someObjects.size() != myObjectEntries.size())
	{
		RepopulateEntries(someObjects);
	}

	ImGui::BeginChild("Scrolling");
	for (size_t i = 0; i < myObjectEntries.size(); i++)
	{
		// #! Here we want to update to use VirtualObject->Name + number
		std::string number = std::to_string(i);
		// OLD version
		// std::string Title = "Object " + number;
		// NEW version
		std::string Title = number + ". " + * myObjectEntries[i]->GetObject()->GetNamePtr();

		if (ImGui::Button(Title.c_str()))
		{
			myObjectEntries[i]->Opened = !myObjectEntries[i]->Opened;
			selectedItem = i;
		}

		if (!myObjectEntries[i]->Opened)
		{
			continue;
		}

		if (selectedItem == i)
		{
			myObjectEntries[i]->Update();
		}
		else
		{
			myObjectEntries[i]->Opened = false;
		}
	}
	ImGui::EndChild();
}

void Characters::EditorGUI::RepopulateEntries(std::vector<VirtualObject*> someObjects)
{
	for (size_t i = 0; i < myObjectEntries.size(); i++)
	{
		delete myObjectEntries[i];
	}

	myObjectEntries.clear();

	for (size_t i = 0; i < someObjects.size(); i++)
	{
		ObjectEntry* e = new ObjectEntry(someObjects[i]);

		myObjectEntries.push_back(e);
	}
}
