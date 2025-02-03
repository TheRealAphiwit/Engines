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
class Cube;
class Triangle;
class Square;

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

	extern GLFWwindow* window;
	extern Shader* myShader;
	extern Shader* myBillboard;
	extern Square* mySquare;
	extern Triangle* myTriangle;
	extern Cube* myCube;
	extern Texture* myTexture;
	extern Texture* myGrassTexture;
	extern Texture* myConcreteTexture;
	extern Mesh* FlagMesh;
	extern Mesh* mySphere;
	extern Mesh* myPlane;

	extern float myWidth;
	extern float myHeight;
	extern float lastTime;
	extern float currentTime;
	extern float DeltaTime;

	std::vector<VirtualObject*> GetObjects();
}