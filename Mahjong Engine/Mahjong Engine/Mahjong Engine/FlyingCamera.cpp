#include "FlyingCamera.h"
#include "ObjectEntry.h"
#include "Input.h"
#include "Camera.h"
#include "ETime.h"
#include <iostream>

const float Sensitivity = 0.5f;
const float MoveSpeed = 10.0f;

Engine::FlyingCamera::FlyingCamera(DotsRendering::Camera* aCamera, Engine::Input* anInput, Engine::ETime* aTime)
{
	myCamera = aCamera;
	myInput = anInput;
	myTime = aTime;

	lastX = 0;
	lastY = 0;

	pitch = 0;
	yaw = 0;
}

void Engine::FlyingCamera::Update(GLFWwindow* aWindow)
{
	#pragma region Keyboard Movement
	if (myInput->IsKeyPressed(GLFW_KEY_TAB))
	{
		SetCamState(aWindow);
	}

	if (Editing) return;

	glm::vec3 velocity = glm::vec3(0.0f);
	glm::vec3 direction = glm::vec3(0.0f);

	if (myInput->IsKeyDown(GLFW_KEY_W)) velocity.z = 1;
	if (myInput->IsKeyDown(GLFW_KEY_A)) velocity.x = -1;
	if (myInput->IsKeyDown(GLFW_KEY_S)) velocity.z = -1;
	if (myInput->IsKeyDown(GLFW_KEY_D)) velocity.x = 1;

	if (myInput->IsKeyDown(GLFW_KEY_SPACE)) velocity.y = 1;
	if (myInput->IsKeyDown(GLFW_KEY_LEFT_SHIFT)) velocity.y = -1;
	#pragma endregion

	#pragma region Mouse Movement
	float xPos = myInput->GetCursorX();
	float yPos = myInput->GetCursorY();

	// Calc ofset and then update last position
	float xOffset = xPos - lastX;
	float yOffset = lastY - yPos;
	lastX = xPos;
	lastY = yPos;

	xOffset *= Sensitivity;
	yOffset *= Sensitivity;

	yaw += xOffset;
	pitch += yOffset;

	pitch = glm::clamp(pitch, -89.0f, 89.0f);

	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

	myCamera->Move(velocity * MoveSpeed * myTime->DeltaTime());
	myCamera->SetDirection(direction);
	#pragma endregion
}

void Engine::FlyingCamera::SetCamState(GLFWwindow* aWindow)
{
	// Toggle operation that switches value when this method gets called
	Editing = !Editing;

	if (Editing)
	{
		glfwSetInputMode(aWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
	else
	{
		glfwSetInputMode(aWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
}