#pragma once
#include "Component.h"
#include <glm.hpp>

enum class ELightType : uint8_t
{
	Point = 0,
	Directional = 1,
	Spot = 2
};

class LightComponent : public Component
{
public:
	LightComponent();
	~LightComponent();

	ELightType LightType = ELightType::Point; // Starting with point - following Emil task order

	glm::vec3 Position = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 Color = glm::vec3(1.0f, 1.0f, 1.0f);
	float Intensity = 1.0f;

	// For point and spot lights
	float Range = 10.0f; 

	// For spot lights
	float InnerCone = glm::radians(15.0f);
	float OuterCone = glm::radians(25.0f);

	// For directional lights
	glm::vec3 LocalDirection = glm::vec3(0.0f, -1.0f, 0.0f); // Now pointing up to match glm::lookAt() - since it needs up

	void Update() override;

	// Getters/Utils
	ELightType GetLightType() const { return LightType; };

	bool IsPoint() const { return LightType == ELightType::Point; }
	bool IsSpot() const { return LightType == ELightType::Spot; }
	bool IsDirectional() const { return LightType == ELightType::Directional; }
};