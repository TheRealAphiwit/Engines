#include "LightHandler.h"
#include "Shader.h"
#include "LightComponent.h"
#include <iostream>

void LightHandler::RegisterLight(LightComponent* light)
{
	myLights.push_back(light);
}

void LightHandler::UnregisterLight(LightComponent* light)
{
	auto it = std::find(myLights.begin(), myLights.end(), light);
	if (it != myLights.end())
	{
		myLights.erase(it);
	}
}

void LightHandler::UploadLightsToShader(Shader& shader) const
{
	// Upload light data to the shader
	// shader.Use(); - currently using per object 
	int lightCount = static_cast<int>(myLights.size());
	shader.SetInt(lightCount, "lightCount");

	// std::cout << "[LightHandler] Uploading " << lightCount << " lights to shader.\n";

	for (int i = 0; i < lightCount; ++i)
	{
		const LightComponent* light = myLights[i];
		std::string baseName = "lights[" + std::to_string(i) + "]";
		shader.SetInt(static_cast<int>(light->GetLightType()), baseName + ".Type");
		shader.SetVector3(light->Owner->Position, baseName + ".Position"); // Use Owner's position
		shader.SetVector3(light->Color, baseName + ".Color");
		shader.SetFloat(light->Intensity, baseName + ".Intensity");
		if (light->IsPoint() || light->IsSpot())
		{
			shader.SetFloat(light->Range, baseName + ".Range");
		}
		if (light->IsSpot())
		{
			shader.SetFloat(light->InnerCone, baseName + ".InnerCone");
			shader.SetFloat(light->OuterCone, baseName + ".OuterCone");
		}
		if (light->IsDirectional())
		{
			shader.SetVector3(light->LocalDirection, baseName + ".Direction");
		}
	}
}
