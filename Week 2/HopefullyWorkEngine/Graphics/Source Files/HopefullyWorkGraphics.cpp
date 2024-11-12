#include "HopefullyWorkGraphics.h"
#include <iostream>
#include <glad/glad.h>
#include <GLFW\glfw3.h>
#include <direct.h>
#include <Windows.h>
#include <stdio.h>
#include "Shader.h"

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
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);

	window = glfwCreateWindow(Width, Height, "Hopefully This Works", NULL, NULL);
	
	if (window == NULL)
	{
		std::cout << "Failed to initialize window" << std::endl;
		return;
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return;
	}

	HMODULE module = GetModuleHandleW(NULL);
	WCHAR path[MAX_PATH];
	GetModuleFileNameW(module, path, MAX_PATH);

	myShader = new Shader();

	myShader->Initialize("C:/Users/lekph/OneDrive/Dokument/GitHub/HopefullyWorkEngine/Week 2/HopefullyWorkEngine/Assets/shaders/vertex_shader.glsl.txt",
					     "C:/Users/lekph/OneDrive/Dokument/GitHub/HopefullyWorkEngine/Week 2/HopefullyWorkEngine/Assets/shaders/fragment_shader.glsl.txt");
}

void HopefullyWorkGraphics::Render()
{
	glClear(GL_COLOR_BUFFER_BIT);

	// Vertices for a triangle - with additional color values
	float vertices[] =
	{
		-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
		0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f
	};

	unsigned int VBO;
	glGenBuffers(1, &VBO);

	unsigned int VAO;
	glGenVertexArrays(1, &VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindVertexArray(VAO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(0));
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	myShader->Use();

	glDrawArrays(GL_TRIANGLES, 0, 3);

	glfwSwapBuffers(window);
	glfwPollEvents();
}

bool HopefullyWorkGraphics::ShouldClose()
{
	if (glfwWindowShouldClose(window))
	{
		glfwTerminate();
		return true;
	}

	return false;
}