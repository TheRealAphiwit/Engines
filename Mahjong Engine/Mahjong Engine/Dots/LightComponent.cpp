#include "LightComponent.h"
#include "LightHandler.h"
#include <MahjongMath.h>

LightComponent::LightComponent()
{
	// Auto register to the light handler
	LightHandler::GetInstance().RegisterLight(this);
}

LightComponent::~LightComponent()
{
	// Auto unregister from the light handler
	LightHandler::GetInstance().UnregisterLight(this);
}

void LightComponent::Update()
{
	// Update position, direction, etc. based on the owner VirtualObject's transform
	Position = Owner->Position;
	LocalDirection = Owner->Rotation; // Lamp model will be faced downwards

	// Clamping intensity and range to avoid negative values
	Intensity = glm::max(0.0f, Intensity);
	Range = glm::max(0.01f, Range);

	// Clamping cone angles for spot lights
	InnerCone = glm::clamp(InnerCone, 0.0f, glm::pi<float>() / 2.0f);
	OuterCone = glm::clamp(OuterCone, InnerCone, glm::pi<float>() / 2.0f);
}