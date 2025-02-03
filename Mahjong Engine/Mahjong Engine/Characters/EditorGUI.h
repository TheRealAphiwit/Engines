#pragma once
#include <vector>
#include "Camera.h"

class VirtualObject;
class ResourceHandler;
struct GLFWwindow;

class ResourceEditor;
class ShaderEditor;

namespace Characters
{
	enum class ECurrentEditor
	{
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
		EditorGUI(GLFWwindow* aWindow, ResourceHandler* aResourceHandler, DotsRendering::Camera* aCamera);
		~EditorGUI();
		void Render(std::vector<VirtualObject*> someObjects);

	private:
		void UpdateHieracrhy(std::vector<VirtualObject*> someObjects);
		void UpdateCameraSettings();

		void RepopulateEntries(std::vector<VirtualObject*> someObjects);
		std::vector<ObjectEntry*> myObjectEntries;
		
		DotsRendering::Camera* myCamera;
		ResourceHandler* myResourceHandler;
		ECurrentEditor myCurrentEditor;
		ResourceEditor* myResourceEditor;
	};
}