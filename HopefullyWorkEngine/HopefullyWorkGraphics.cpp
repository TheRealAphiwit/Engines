#include "HopefullyWorkGraphics.h"
#include <iostream>
#include <glad/glad.h>
#include <GLFW\glfw3.h>
#include <direct.h>
#include "Shader.h"
#include <Windows.h>
#include <stdio.h>

GLFWwindow* window;
Shader* myShader;

void HopefullyWorkGraphics::Initialize(int Width, int Height)
{
	if (!glfwInit())
	{
		std::cout << "Failed to init glfw" << std::endl;
		return;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(640, 480, "Hopefully This Works", NULL, NULL);
	glfwMakeContextCurrent(window);

	if (window == NULL)
	{
		std::cout << "Failed to initialize window" << std::endl; 
		return;
	}

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return;
	}

	HMODULE module = GetModuleHandleW(NULL);
	WCHAR path[MAX_PATH];
	GetModuleFileNameW(module, path, MAX_PATH);

	myShader = new Shader();

	myShader->Initialize("C:/Users/lekph/OneDrive/Dokument/GitHub/HopefullyWorkEngine/HopefullyWorkEngine/shaders/fragment_shader.txt",
						 "C:/Users/lekph/OneDrive/Dokument/GitHub/HopefullyWorkEngine/HopefullyWorkEngine/shaders/vertex_shader.txt");
}

void HopefullyWorkGraphics::Render()
{

}
