#pragma once
#include <vector>

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
		COUNT
	};

	class ObjectEntry;

	class EditorGUI
	{
	public:
		EditorGUI(GLFWwindow* aWindow, ResourceHandler* aResourceHandler);
		~EditorGUI();
		void Render(std::vector<VirtualObject*> someObjects);

	private:
		void UpdateHieracrhy(std::vector<VirtualObject*> someObjects);

		void RepopulateEntries(std::vector<VirtualObject*> someObjects);
		std::vector<ObjectEntry*> myObjectEntries;
		ResourceHandler* myResourceHandler;

		ECurrentEditor myCurrentEditor;

		ResourceEditor* myResourceEditor;
	};
}