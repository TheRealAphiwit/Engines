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

#include <fstream>
#include <sstream>

GLFWwindow* window;

Shader* myShader;
Shader* myBillboard;

Square* mySquare;
Triangle* myTriangle;
Cube* myCube;

Texture* myTexture;
Texture* myGrassTexture;
Mesh* FlagMesh;

float myWidth;
float myHeight;

float lastTime;
float currentTime;
float DeltaTime;

std::vector<VirtualObject*> myObjects;
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

	#pragma region File loadings
	// How do we make it so that texture loads different .png's later in engine while running?
	myGrassTexture = new Texture("../Assets/Images/Grass.png", true);
	myTexture = new Texture("../Assets/Images/Default.png", false);
	myShader = new Shader("../Assets/Shaders/VertexShader.glsl", "../Assets/Shaders/FragmentShader.glsl");
	myBillboard = new Shader("../Assets/Shaders/VertexBillboard.glsl", "../Assets/Shaders/FragmentShader.glsl");

	FlagMesh = LoadObjMesh("../Assets/Models/Flag.obj");
	#pragma endregion


	myCube = new Cube();
	mySquare = new Square();

	Camera* camera = new Camera(width, height);

	initData.camera = camera;
	initData.window = window;

	glEnable(GL_DEPTH_TEST);
	glfwSwapInterval(1);

	return initData;
}

void DotsRendering::BeginRender(Camera* camera)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (int i = 0; i < myObjects.size(); i++)
	{
		myObjects[i]->Draw(camera);
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

void DotsRendering::CreateVirtualObject(Mesh* aMesh, Texture* aTexture, Shader* aShader)
{
	VirtualObject* newObject = new VirtualObject(aMesh, aTexture, aShader);
	myObjects.push_back(newObject);
}

std::vector<VirtualObject*> DotsRendering::GetObjects()
{
	return myObjects;
}