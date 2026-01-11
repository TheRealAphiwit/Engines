#include "LightComponent.h"
#include "LightHandler.h"
#include <MahjongMath.h>
#include <iostream>

LightComponent::LightComponent()
{
	// Auto register to the light handler
	LightHandler::GetInstance().RegisterLight(this);

	std::cout << "LightComponent created and registered.\n";
}

LightComponent::~LightComponent()
{
	// Auto unregister from the light handler
	LightHandler::GetInstance().UnregisterLight(this);
}

void LightComponent::Update()
{
	// Position
	Position = Owner->Position;

	// Direction (convert rotation Å® forward vector)
	glm::mat4 rotMat(1.0f);
	rotMat = glm::rotate(rotMat, Owner->Rotation.x, glm::vec3(1, 0, 0));
	rotMat = glm::rotate(rotMat, Owner->Rotation.y, glm::vec3(0, 1, 0));
	rotMat = glm::rotate(rotMat, Owner->Rotation.z, glm::vec3(0, 0, 1));

	LocalDirection = glm::normalize(glm::vec3(rotMat * glm::vec4(0, 0, -1, 0)));

	// Safety clamps
	Intensity = glm::max(0.0f, Intensity);
	Range = glm::max(0.05f, Range);

	// Spot cones (store radians Å® cosines)
	InnerCone = glm::clamp(InnerCone, 0.0f, glm::half_pi<float>());
	OuterCone = glm::clamp(OuterCone, InnerCone, glm::half_pi<float>());

	InnerCone = cos(InnerCone);
	OuterCone = cos(OuterCone);

	// Og light code
	/*
	// Update position, direction, etc. based on the owner VirtualObject's transform
	Position = Owner->Position;
	LocalDirection = Owner->Rotation; // Lamp model will be faced downwards

	glm::mat4 rotMat = glm::mat4(1.0f);
	rotMat = glm::rotate(rotMat, Owner->Rotation.x, glm::vec3(1, 0, 0));
	rotMat = glm::rotate(rotMat, Owner->Rotation.y, glm::vec3(0, 1, 0));
	rotMat = glm::rotate(rotMat, Owner->Rotation.z, glm::vec3(0, 0, 1));

	LocalDirection = glm::normalize(glm::vec3(rotMat * glm::vec4(0, 0, -1, 0)));

	// Clamping intensity and range to avoid negative values
	Intensity = glm::max(0.0f, Intensity);
	Range = glm::max(0.01f, Range);

	// Clamping cone angles for spot lights
	InnerCone = glm::clamp(InnerCone, 0.0f, glm::pi<float>() / 2.0f);
	OuterCone = glm::clamp(OuterCone, InnerCone, glm::pi<float>() / 2.0f);
	*/
}