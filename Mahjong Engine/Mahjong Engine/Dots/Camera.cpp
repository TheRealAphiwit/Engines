#include "Camera.h"
#include "DotGraphics.h"
#include <iostream>
#include <gtc/matrix_transform.hpp>
#include <iostream>
#include <glm.hpp>

glm::vec3 WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);

DotsRendering::Camera::Camera(const float& aWidth, const float& aHeight)
{
	myProjection = glm::perspective(glm::radians(45.0f), aWidth / aHeight, 0.1f, 100.0f);
	myDirection = glm::vec3(0.0f, 0.0f, -1.0f);
	myPosition = glm::vec3(0.0f, 0.0f, 3.0f);
	myUp = glm::vec3(0.0f, 1.0f, 0.0f);
	myRight = glm::vec3(0, 0, 0);
	myView = glm::mat4(0);
}

void DotsRendering::Camera::CameraUpdate()
{
	myRight = glm::normalize(glm::cross(myDirection, WorldUp));
	myUp = glm::cross(myRight, myDirection);
	myView = glm::lookAt(myPosition, myPosition + myDirection, myUp);
}

void DotsRendering::Camera::SetPosition(const glm::vec3& aPosition)
{
}

void DotsRendering::Camera::SetRotation(const glm::vec3& aRotation)
{
}

void DotsRendering::Camera::Move(const glm::vec3& aMove)
{
	myPosition += myDirection * aMove.z;
	myPosition += myRight * aMove.x;
	myPosition += WorldUp * aMove.y;
	// std::cout << "Camera position: " << myPosition.x << " " << myPosition.y << " " << myPosition.z << std::endl;
}

void DotsRendering::Camera::SetDirection(const glm::vec3& aDirection)
{
	myDirection = glm::normalize(aDirection);
}

glm::vec3 DotsRendering::Camera::GetCameraPosition() const
{
	return myPosition;
}
