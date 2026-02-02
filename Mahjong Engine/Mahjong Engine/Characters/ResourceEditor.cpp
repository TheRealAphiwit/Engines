#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <string>

#include "ResourceEditor.h"
#include <../Mahjong Engine/ResourceHandler.h>

#include "Shader.h"
#include "../Dots/Texture.h"

char vPath[256];
char fPath[256];
char ResourceName[256];

ResourceEditor::ResourceEditor(ResourceHandler* aHandler)
{
	myResources = aHandler;
	myCurrentResource = EResource::COUNT;
}

ResourceEditor::~ResourceEditor()
{
}

void ResourceEditor::Update()
{
	// Updating menu logic
	if (ImGui::BeginMenu("Create a resource"))
	{
		if (ImGui::MenuItem("Create Texture", "")) { myCurrentResource = EResource::Texture; }
		if (ImGui::MenuItem("Create Shader", "")) { myCurrentResource = EResource::Shader; }
		if (ImGui::MenuItem("Object Mesh", "")) { myCurrentResource = EResource::Mesh; }
		ImGui::EndMenu();
	}

	// Switch statements based on my current resource
	switch (myCurrentResource)
	{
	case EResource::Texture:

		if (ImGui::Button("Create Texture"))
		{

		}

		break;

	case EResource::Shader:

		ImGui::InputText("Name", ResourceName, IM_ARRAYSIZE(ResourceName));
		ImGui::InputText("vertex path", vPath, IM_ARRAYSIZE(vPath));
		ImGui::InputText("fragment path", fPath, IM_ARRAYSIZE(fPath));

		// Shader creation seems done already
		if (ImGui::Button("Create Shader"))
		{
			std::string FinalVPath = "../Assets/Shaders/";
			std::string FinalFPath = "../Assets/Shaders/";

			std::string FinalName = ResourceName;

			FinalVPath += vPath;
			FinalFPath += fPath;

			myResources->CreateShader(FinalVPath.c_str(), FinalFPath.c_str(), FinalName.c_str());

			memset(vPath, '\0', sizeof(vPath));
			memset(fPath, '\0', sizeof(fPath));
			memset(ResourceName, '\0', sizeof(ResourceName));

			int kek = 0;
		}

		break;

	case EResource::Mesh:

		// NOt implemented
		if (ImGui::Button("Create Mesh"))
		{

		}

		break;

	default:
		break;
	}

	// List of resources
	ImGui::BeginChild("Scrolling");

	std::vector<std::string> allResources = myResources->GetAllResources();
	static std::string selectedResource;

	for (int n = 0; n < allResources.size(); n++)
	{
		// ImGui::Text(allResources[n].c_str(), n); - old unclickable version

		// New clickable versions - A
		/*if (ImGui::Button(allResources[n].c_str()))
		{
			std::string resourceName = allResources[n];
		}*/

		// New clickable versions - B
		if (ImGui::Selectable(allResources[n].c_str(), selectedResource == allResources[n]))
		{
			selectedResource = allResources[n];
		}
	}

	if (!selectedResource.empty())
	{
		Texture* tex = myResources->GetTexture(selectedResource);

		if (tex)
		{
			ImGui::Separator();
			ImGui::Text("Texture Settings");

			bool useMipmaps = tex->GetUseMipmaps();
			if (ImGui::Checkbox("Use Mipmaps", &useMipmaps))
			{
				tex->SetUseMipmaps(useMipmaps);
			}
		}
	}

	ImGui::EndChild();
}
