#pragma once
#include "EditorGUI.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "DragonEngine.h"
#include "VirtualObject.h"
#include "GameObject.h"
#include "ObjectEntry.h"
#include "EntityHandler.h"
#include "GameObjectHandler.h"
#include "ResourceEditor.h"

#include <glm.hpp>
#include <string>
#include <iostream>

#define itoc(a) ((char*)(intptr_t)(a)) // Line here allows us to convert int's into char*'s

Characters::EditorGUI::EditorGUI(GLFWwindow* aWindow, ResourceHandler* aResourceHandler, DotsRendering::Camera* aCamera, Engine::DragonEngine* aEngine)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io; // void cast prevents unused variable warning and this row as a whole gives access to input and output data
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(aWindow, true); // Initializes ImGui to work with a GLFW window and OpenGL for rendering.
	ImGui_ImplOpenGL3_Init("#version 330");

	myEngine = aEngine;
	myResourceHandler = aResourceHandler;
	myResourceEditor = new ResourceEditor(myResourceHandler);
	myCamera = aCamera;
	myCurrentEditor = ECurrentEditor::COUNT;
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

void Characters::EditorGUI::Render(std::vector<VirtualObject*> someObjects, std::vector<GameObject*> gameObjects)
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
			if (ImGui::MenuItem("Camera Settings", "Ctrl+C")) { myCurrentEditor = ECurrentEditor::ECameraSettings; }
			if (ImGui::MenuItem("GameObject Editor", "Ctrl+G")) { myCurrentEditor = ECurrentEditor::EGameObjectHierarchy; }  // New menu item
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
	case Characters::ECurrentEditor::ECameraSettings:
		UpdateCameraSettings();
		break;
	case Characters::ECurrentEditor::EGameObjectHierarchy:
		UpdateGameObjHierarchy(gameObjects);
		break;
	case Characters::ECurrentEditor::COUNT:
		break;
	default:
		break;
	}

	ImGui::End();

	UpdateSimulationPanel();

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

void Characters::EditorGUI::UpdateGameObjHierarchy(std::vector<GameObject*> someObjects)
{
	if (ImGui::Button("Create GameObject"))
	{
		// Create a new GameObject using the GameObjectHandler
		std::future<GameObject*> future = Engine::GameObjectHandler::GetInstance().CreateDefaultCube();

		try {
			GameObject* createdObject = future.get(); // Wait for the object to be ready
			std::cout << "Created GameObject: " << createdObject << std::endl;
		}
		catch (const std::exception& error) {
			std::cerr << "Error creating GameObject: " << error.what() << std::endl;
		}
	}

	// Ensure the list is up to date
	if (someObjects.size() != myGameObjectEntries.size())
	{
		RepopulateGameObjectEntries(someObjects);
	}

	ImGui::BeginChild("GameObject Scrolling");

	for (size_t i = 0; i < myGameObjectEntries.size(); i++)
	{
		std::string number = std::to_string(i);
		std::string Title = number + ". GameObject";

		if (ImGui::Button(Title.c_str()))
		{
			myGameObjectEntries[i]->Opened = !myGameObjectEntries[i]->Opened;
			selectedItem = i;
		}

		if (!myGameObjectEntries[i]->Opened)
		{
			continue;
		}

		if (selectedItem == i)
		{
			myGameObjectEntries[i]->Update();

			// Add a delete button for GameObjects
			if (ImGui::Button("Delete GameObject"))
			{
				Engine::GameObjectHandler::GetInstance().DeleteGameObject(someObjects[i]);
				RepopulateGameObjectEntries(someObjects);
			}
		}
		else
		{
			myGameObjectEntries[i]->Opened = false;
		}
	}

	ImGui::EndChild();
}

void Characters::EditorGUI::UpdateCameraSettings()
{
	if (ImGui::Begin("Camera Settings"))
	{
		glm::vec3 cameraPos = myCamera->GetCameraPosition();
		glm::vec3 cameraRot = myCamera->GetCameraRotation();
		float zoom = myCamera->GetZoom();

		if (ImGui::DragFloat3("Position", &cameraPos.x, 0.1f))
		{
			myCamera->SetPosition(cameraPos);
		}

		if (ImGui::DragFloat3("Rotation", &cameraRot.x, 0.1f))
		{
			myCamera->SetRotation(cameraRot);
		}

		if (ImGui::SliderFloat("Zoom(FOV)", &zoom, 10.0f, 90.0f))
		{
			myCamera->SetZoom(zoom);
		}
	}
	ImGui::End();
}

void Characters::EditorGUI::UpdateSimulationPanel()
{
	ImGui::Begin("Simulation Controls"); // Creates a separate window

	bool isSimulating = myEngine->IsSimulating();
	ImGui::Text("Simulation State: %s", isSimulating ? "Running" : "Paused");

	if (ImGui::Button("Start Simulation")) {
		myEngine->StartSimulating();
	}

	ImGui::SameLine();

	if (ImGui::Button("Stop Simulation")) {
		myEngine->StopSimulating();
	}

	ImGui::End();
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

void Characters::EditorGUI::RepopulateGameObjectEntries(std::vector<GameObject*> someObjects)
{
	for (size_t i = 0; i < myGameObjectEntries.size(); i++)
	{
		delete myGameObjectEntries[i];
	}

	myGameObjectEntries.clear();

	for (size_t i = 0; i < someObjects.size(); i++)
	{
		GameObjectEntry* e = new GameObjectEntry(someObjects[i]);

		myGameObjectEntries.push_back(e);
	}
}
