#include "DragonEngine.h"

#include "Camera.h"
#include <glfw3.h>
#include "Input.h"
#include "ETime.h"
#include <glm.hpp>
#include "FlyingCamera.h"
#include <iostream>

// Contructor diff - check what changes between these two
Engine::DragonEngine::DragonEngine(GLFWwindow* aWindow, DotsRendering::Camera* aCamera)
{
	myInput = new Input(aWindow);
	myTime = new ETime();
	myWindow = aWindow;

	myFlyingCamera = new FlyingCamera(aCamera, myInput, myTime);
	myCamera = aCamera;
}

void Engine::DragonEngine::Update(const float& aDeltaTime)
{
	myFlyingCamera->Update(myWindow);
	myTime->UpdateDeltaTime(aDeltaTime);
}

void Engine::DragonEngine::StartSimulating()
{
	std::cout << "START SIMULATING" << std::endl ;
	ShouldSimulate = true;
}

void Engine::DragonEngine::StopSimulating()
{
	std::cout << "STOP SIMULATING" << std::endl;
	ShouldSimulate = false;
}

const bool& Engine::DragonEngine::IsSimulating()
{
	return ShouldSimulate;
}

std::vector<GameObject*> Engine::DragonEngine::GetGameObjects()
{
	return myGameObjects;
}
