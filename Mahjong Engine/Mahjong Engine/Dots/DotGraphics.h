#pragma once
#include <vector>
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

	void Input(GLFWwindow* window);

	std::vector<VirtualObject> GetObjects();
	std::string LoadObjRaw(const char* path);
}