#include "HopefullyWorkGraphics.h"
#include <iostream>
// #include <glad/glad.h>
#include <glfw3.h>
#include <direct.h>
// #include "Shader.h"
#include <Windows.h>
#include <stdio.h>

GLFWwindow* window;
// Shader* shader;

void HopefullyWorkGraphics::Initialize(int Width, int Height)
{
	if (!glfwInit())
	{
		std::cout << "Failed to init glfw" << std::endl;
		return;
	}
}