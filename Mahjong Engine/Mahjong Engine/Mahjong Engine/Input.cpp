#include "Input.h"
#include <glfw3.h>
#include <unordered_map>
#include <stdexcept>
#include <iostream>

std::unordered_map<int, bool> myKeyStates;
float lastX, lastY;

Engine::Input::Input(GLFWwindow* aWindow)
{
	if (!aWindow) 
	{
		std::cerr << "GLFWwindow pointer is null!" << std::endl;
		throw std::invalid_argument("GLFWwindow pointer is null");
	}

	std::cout << "Initializing Input with GLFWwindow: " << aWindow << std::endl;
	myWindow = aWindow;

	GLFWvidmode return_struct;

	int width, height = 0;
	glfwGetWindowSize(aWindow, &width, &height);
	std::cout << "Window size: " << width << "x" << height << std::endl;

	lastX = width / 2;
	lastY = height / 2;

	glfwSetKeyCallback(aWindow, KeyCallBack);
	glfwSetCursorPosCallback(aWindow, MouseCallBack);
	std::cout << "Input initialized successfully!" << std::endl;
}

bool Engine::Input::IsKeyPressed(const int& aKey)
{
	if (myKeyStates[aKey])
	{
		// Single press detection to always return true
		myKeyStates[aKey] = false;
		return true;
	}

    return false;
}

bool Engine::Input::IsKeyDown(const int& aKey)
{
	return glfwGetKey(myWindow, aKey) == GLFW_PRESS;
}

float Engine::Input::GetCursorX()
{
	return lastX;
}

float Engine::Input::GetCursorY()
{
	return lastY;
}

void Engine::Input::KeyCallBack(GLFWwindow* aWindow, int aKey, int aScanCode, int anAction, int aMod)
{
	if (anAction == GLFW_PRESS)
	{
		myKeyStates[aKey] = true;
	}
	else if (anAction == GLFW_RELEASE)
	{
		myKeyStates[aKey] = false;
	}
}

void Engine::Input::MouseCallBack(GLFWwindow* aWindow, double xPos, double yPos)
{
	lastX = xPos;
	lastY = yPos;
}