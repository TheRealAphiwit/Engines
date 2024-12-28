#pragma once
#include <vector>
#include <memory>
#include <string>

// Both of these are custom references we create
struct  GLFWwindow;
class VirtualObject;

class Mesh;
class Texture;
class Shader;

// Wierd namespace (again)
namespace DotsRendering
{
	// Self created camera class
	class Camera;

	struct DotsInitData
	{
		Camera* camera;
		GLFWwindow* window;
	};

	DotsInitData Initialize(int width, int height);
	void BeginRender(Camera* camera);
	void End();
	bool ShouldClose();
	void ClosingInput(GLFWwindow* window);

	void CreateVirtualObject(std::shared_ptr<std::string> name, Mesh* aMesh, Texture* aTexture, Shader* aShader); // NEW
	void CreateVirtualObject(std::shared_ptr<std::string> name, std::string meshName, std::string textureName, std::string shaderName); // ALT VERSION
	void DeleteVirtualObject(VirtualObject* object);
	void CreateDefaultCube();
	void CreateDefaultSphere();

	std::vector<VirtualObject*> GetObjects();
}