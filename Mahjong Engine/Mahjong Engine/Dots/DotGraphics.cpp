#include "DotGraphics.h"
#include "VirtualObject.h"
#include <iostream>
#include <glad/glad.h>
#include <glfw3.h>
#include <direct.h>
#include "Shader.h"
#include <Windows.h>
#include "Triangle.h"
#include "Cube.h"
#include <glm.hpp>
#include <fwd.hpp>
#include <gtc/matrix_transform.hpp>
#include "Camera.h"
#include "stb_image.h"
#include "Texture.h"
#include "Square.h"
#include "EntityHandler.h"

#include <fstream>
#include <sstream>
#include "../Mahjong Engine/ResourceHandler.h"

// Can these pointers be stored in a .h file and still be acceseble in the .cpp file and for other scripts?

namespace DotsRendering
{
	GLFWwindow* window = nullptr;
	Shader* myShader = nullptr;
	Shader* myBillboard = nullptr;
	Square* mySquare = nullptr;
	Triangle* myTriangle = nullptr;
	Cube* myCube = nullptr;
	Texture* myTexture = nullptr;
	Texture* myGrassTexture = nullptr;
	Texture* myConcreteTexture = nullptr;
	Mesh* FlagMesh = nullptr;
	Mesh* mySphere = nullptr;
	Mesh* myPlane = nullptr;

	float myWidth = 0.0f;
	float myHeight = 0.0f;

	float lastTime = 0.0f;
	float currentTime = 0.0f;
	float DeltaTime = 0.0f;
}

VirtualObject* myBillboardObject = nullptr;

DotsRendering::DotsInitData DotsRendering::Initialize(int width, int height)
{
	DotsInitData initData;
	initData.camera = NULL;
	initData.window = NULL;

	if (!glfwInit())
	{
		std::cout << "Failed to initialize glfw" << std::endl;
		return initData;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(width, height, "MAHJONG!", NULL, NULL);
	glfwMakeContextCurrent(window);

	myWidth = width;
	myHeight = height;

	if (window == NULL)
	{
		std::cout << "Failed to initialize window" << std::endl;
		glfwTerminate();
		return initData;
	}

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return initData;
	}

	myCube = new Cube();
	mySquare = new Square();
	myTriangle = new Triangle();

	#pragma region Asset Loadings
	// #! Make 3 lists for loaded resources; meshes, textures, shaders - we'll be able to use this list to get resources simpler im editor if needed
	// How do we make it so that texture loads different .png's later in engine while running?
	myGrassTexture = new Texture("../Assets/Images/Grass.png", true);
	myConcreteTexture = new Texture("../Assets/Images/Concrete.png", false);
	myTexture = new Texture("../Assets/Images/Default.png", false);
	myShader = new Shader("../Assets/Shaders/VertexShader.glsl", "../Assets/Shaders/FragmentShader.glsl");
	myBillboard = new Shader("../Assets/Shaders/VertexBillboard.glsl", "../Assets/Shaders/FragmentShader.glsl");
	FlagMesh = LoadObjMesh("../Assets/Models/Flag.obj");

	// This section can be moved to ResourceHandler
	ResourceHandler::GetInstance().CreateTexture("../Assets/Images/Grass.png", true, "Grass");
	ResourceHandler::GetInstance().CreateTexture("../Assets/Images/Concrete.png", false, "Concrete");
	ResourceHandler::GetInstance().CreateTexture("../Assets/Images/Default.png", false, "Default");
	ResourceHandler::GetInstance().CreateShader("../Assets/Shaders/VertexShader.glsl", "../Assets/Shaders/FragmentShader.glsl", "myShader");
	ResourceHandler::GetInstance().CreateShader("../Assets/Shaders/VertexBillboard.glsl", "../Assets/Shaders/FragmentShader.glsl", "myBillboard");
	ResourceHandler::GetInstance().CreateMesh("../Assets/Models/Flag.obj", "FlagMesh");
	ResourceHandler::GetInstance().RegisterMesh(myCube, "Cube");
	ResourceHandler::GetInstance().RegisterMesh(mySquare, "Square");
	ResourceHandler::GetInstance().RegisterMesh(myTriangle, "Triangle");
	#pragma endregion

	Camera* camera = new Camera(width, height);

	initData.camera = camera;
	initData.window = window;

	glEnable(GL_DEPTH_TEST);
	glfwSwapInterval(1);

	myTexture = ResourceHandler::GetInstance().GetTexture("Default");
	myShader = ResourceHandler::GetInstance().GetShader("myShader");

	return initData;
}

void DotsRendering::BeginRender(Camera* camera)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	std::vector<VirtualObject*> objects = EntityHandler::GetInstance().GetObjects();

	for (int i = 0; i < objects.size(); i++)
	{
		objects[i]->Draw(camera);
	}

	float time = glfwGetTime();
	myBillboard->SetFloat(time, "time");

	camera->CameraUpdate();
}

void DotsRendering::End()
{
	glfwSwapBuffers(window);
	ClosingInput(window);
	glfwPollEvents();

	currentTime = glfwGetTime();
	DeltaTime = currentTime - lastTime;
	lastTime = currentTime;
}

bool DotsRendering::ShouldClose()
{
	if (glfwWindowShouldClose(window))
	{
		glfwTerminate();
		return true;
	}
	return false;
}

void DotsRendering::ClosingInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}