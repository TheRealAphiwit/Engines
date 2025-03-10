#pragma once
#include <vector>
#include "Camera.h"

class VirtualObject;
class ResourceHandler;
struct GLFWwindow;
class GameObject;
class ResourceEditor;
class ShaderEditor;

namespace Engine
{
	class DragonEngine;
}

namespace Characters
{
	enum class ECurrentEditor
	{
		EGameObjectHierarchy,
		EObjectHierarchy,
		EShaderEditor,
		EResourceViewer,
		ECameraSettings,
		COUNT
	};

	class ObjectEntry;

	class EditorGUI
	{
	public:
		EditorGUI(GLFWwindow* aWindow, ResourceHandler* aResourceHandler, DotsRendering::Camera* aCamera, Engine::DragonEngine* aEngine);
		~EditorGUI();
		void Render(std::vector<VirtualObject*> someObjects);

	private:
		void UpdateHieracrhy(std::vector<VirtualObject*> someObjects);
		void UpdateGameObjHierarchy(std::vector<GameObject*> someObjects);
		void UpdateCameraSettings();
		void UpdateSimulationPanel();
		void RepopulateEntries(std::vector<VirtualObject*> someObjects);
		
		std::vector<ObjectEntry*> myObjectEntries;
		
		Engine::DragonEngine* myEngine;
		DotsRendering::Camera* myCamera;
		ResourceHandler* myResourceHandler;
		ECurrentEditor myCurrentEditor;
		ResourceEditor* myResourceEditor;
	};
}