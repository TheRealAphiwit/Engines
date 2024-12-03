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
// #include "stb_image.h"
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
Texture* myConcreteTexture;
Mesh* MonkeyMesh;

float myWidth;
float myHeight;

float lastTime;
float currentTime;
float DeltaTime;

std::vector<VirtualObject*> myObject;
VirtualObject* myBillboardObject = nullptr;

DotsRendering::DotsInitData DotsRendering::Initialize(int width, int height)
{
	DotsInitData someData;
	someData.camera = NULL;
	someData.camera = NULL;

	if (!glfwInit())
	{
		std::cout << "Failed to initialize glfw" << std::endl;
		return someData;
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
		return someData;
	}

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return someData;
	}

	// Texture img need to be implemented in before (aswell as texture code)
	//myConcreteTexture = new Texture("../Assets/Images/Grass.png", true);
	// myTexture = new Texture("../Assets/Images/Default.png", false);


	return DotsInitData();
}

void DotsRendering::BeginRender(Camera* camera)
{
}
