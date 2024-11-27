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

}
