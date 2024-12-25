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
#include "../Mahjong Engine/ResourceHandler.h"

GLFWwindow* window;

Shader* myShader;
Shader* myBillboard;

Square* mySquare;
Triangle* myTriangle;
Cube* myCube;

Texture* myTexture;
Texture* myGrassTexture;
Texture* myConcreteTexture;
Mesh* FlagMesh;

Mesh* mySphere;
Mesh* myPlane;

float myWidth;
float myHeight;

float lastTime;
float currentTime;
float DeltaTime;

std::vector<VirtualObject*> myObjects;
VirtualObject* myBillboardObject = nullptr;
ResourceHandler& resourceHandler = ResourceHandler::GetInstance();

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
	// #! Make 3 lists for loaded resources; meshes, textures, shaders - we'll be able to use this list to get resources simpler im editor if needed
	// How do we make it so that texture loads different .png's later in engine while running?
	myGrassTexture = new Texture("../Assets/Images/Grass.png", true);
	myConcreteTexture = new Texture("../Assets/Images/Concrete.png", false);
	myTexture = new Texture("../Assets/Images/Default.png", false);
	myShader = new Shader("../Assets/Shaders/VertexShader.glsl", "../Assets/Shaders/FragmentShader.glsl");
	myBillboard = new Shader("../Assets/Shaders/VertexBillboard.glsl", "../Assets/Shaders/FragmentShader.glsl");
	FlagMesh = LoadObjMesh("../Assets/Models/Flag.obj");

	myCube = new Cube();
	mySquare = new Square();
	myTriangle = new Triangle();

	// This section can be moved to ResourceHandler
	resourceHandler.CreateTexture("../Assets/Images/Grass.png", true, "Grass");
	resourceHandler.CreateTexture("../Assets/Images/Concrete.png", false, "Concrete");
	resourceHandler.CreateTexture("../Assets/Images/Default.png", false, "Default");
	resourceHandler.CreateShader("../Assets/Shaders/VertexShader.glsl", "../Assets/Shaders/FragmentShader.glsl", "myShader");
	resourceHandler.CreateShader("../Assets/Shaders/VertexBillboard.glsl", "../Assets/Shaders/FragmentShader.glsl", "myBillboard");
	resourceHandler.CreateMesh("../Assets/Models/Flag.obj", "FlagMesh");
	resourceHandler.RegisterMesh(myCube, "Cube");
	resourceHandler.RegisterMesh(mySquare, "Square");
	resourceHandler.RegisterMesh(myTriangle, "Triangle");
	#pragma endregion

	Camera* camera = new Camera(width, height);

	initData.camera = camera;
	initData.window = window;

	glEnable(GL_DEPTH_TEST);
	glfwSwapInterval(1);

	for (size_t i = 0; i < 3; i++)
	{
		CreateVirtualObject(std::make_shared<std::string>("Cube"), myCube, myTexture, myShader);
	}

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

void DotsRendering::CreateVirtualObject(std::shared_ptr<std::string> name, Mesh* aMesh, Texture* aTexture, Shader* aShader)
{
	// How I currently have done it
	VirtualObject* newObject = new VirtualObject(name, aMesh, aTexture, aShader);
	
	// This currently doesn't work since type diff
	newObject->SetMeshName(resourceHandler.GetMeshName(aMesh));
	newObject->SetTextureName(resourceHandler.GetTextureName(aTexture));
	newObject->SetShaderName(resourceHandler.GetShaderName(aShader));
	myObjects.push_back(newObject);
}

void DotsRendering::CreateVirtualObject(std::shared_ptr<std::string> name, std::string meshName, std::string textureName, std::string shaderName)
{
	// This is the way I want to do it
	VirtualObject* newObject = new VirtualObject(name, resourceHandler.GetMesh(meshName), resourceHandler.GetTexture(textureName), resourceHandler.GetShader(shaderName));
	newObject->SetMeshName(meshName);
	newObject->SetTextureName(textureName);
	newObject->SetShaderName(shaderName);
	myObjects.push_back(newObject);
}

void DotsRendering::DeleteVirtualObject(VirtualObject* object)
{
	if (!object) {return;}

	for (int i = 0; i < myObjects.size(); i++)
	{
		if (myObjects[i] == object)
		{
			myObjects.erase(myObjects.begin() + i);
			delete object;
			return;
		}
	}
}

void DotsRendering::CreateDefaultCube()
{
	VirtualObject* newObject = new VirtualObject(myCube, myTexture, myShader);
	myObjects.push_back(newObject);
}

void DotsRendering::CreateDefaultSphere()
{
	VirtualObject* newObject = new VirtualObject(mySphere, myTexture, myShader);
	myObjects.push_back(newObject);
}

std::vector<VirtualObject*> DotsRendering::GetObjects()
{
	return myObjects;
}