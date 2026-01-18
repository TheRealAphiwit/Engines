#include "ShadowHandler.h"
#include "Shader.h"
#include <MahjongMath.h>
#include "EntityHandler.h" 
#include <iostream>
#include <gtx/string_cast.hpp>


void ShadowHandler::Init(Shader* shadowShader)
{
	myShadowShader = shadowShader;
}

void ShadowHandler::RegisterShadowMap(ShadowMap* shadowMap)
{
	Shadows.emplace_back(shadowMap);
}

void ShadowHandler::UnregisterShadowMap(ShadowMap* shadowMap)
{
	Shadows.erase(std::remove_if(Shadows.begin(), Shadows.end(),
		[shadowMap](const std::unique_ptr<ShadowMap>& ptr) { return ptr.get() == shadowMap; }),
		Shadows.end());
}

void ShadowHandler::ShadowPass()
{
	if(!myShadowShader || Shadows.empty())
		return;

	myShadowShader->Use();

	std::vector<VirtualObject*> objects = DotsRendering::EntityHandler::GetInstance().GetObjects();

	// For each shadow map, render the scene from the light's perspective
	for (const auto& shadowMap : Shadows)
	{
		shadowMap->BindForWriting();
		
		// Compute light space matrix
		glm::mat4 lightSpaceMatrix = ComputeLightSpaceMatrix(*shadowMap);
		myShadowShader->SetMatrix4(lightSpaceMatrix, "lightSpaceMatrix");
		
		// Draw shadow
		for (const auto& object : objects)
		{
			object->DrawShadow(myShadowShader); 
		}

		shadowMap->Unbind();
	}
}

// [DONE]
void ShadowHandler::UploadShadowData(Shader& mainShader)
{
	// Is being called [check]
	// std::cout << "Uploading shadow data for " << Shadows.size() << " shadows.\n";

	int shadowIndex = 0;

	for (auto& shadow : Shadows)
	{
		std::string index = std::to_string(shadowIndex);

		// Bind shadow map texture to appropriate texture unit
		glActiveTexture(GL_TEXTURE0 + shadowIndex);
		glBindTexture(GL_TEXTURE_2D, shadow->DepthTexture);

		// Set the shadow map in the shader GLSL
		mainShader.SetInt(shadowIndex, "shadowMaps[" + index + "]"); // frag part

		// Compute and upload light space matrix
		glm::mat4 lightSpaceMatrix = ComputeLightSpaceMatrix(*shadow);
		mainShader.SetMatrix4(lightSpaceMatrix, "lightSpaceMatrices[" + index + "]"); // vert part

		shadowIndex++;
	}

	mainShader.SetInt(static_cast<int>(Shadows.size()), "shadowCount");
}

glm::mat4 ShadowHandler::ComputeLightSpaceMatrix(ShadowMap& shadowMap)
{
	LightComponent* light = shadowMap.OwnerLight; 

	// A little bit more complex math (regular vec3 did not work and so I had to do normalization w added security checks)
	glm::vec3 lightDir = light->LocalDirection;
	
	/*if (glm::length(lightDir) < 0.0001f)
		lightDir = glm::vec3(0, -1, 0);*/

	lightDir = glm::normalize(lightDir);

	float range = glm::max(light->Range, 0.1f);
	glm::vec3 lightPos = -lightDir * range;

	// sus
	glm::vec3 up = glm::abs(lightDir.y) > 0.99f ? glm::vec3(0, 0, 1) : glm::vec3(0, 1, 0);

	glm::mat4 lightView = glm::lookAt(lightPos, glm::vec3(0.0f), up); 

	float nearPlane = 0.1f;
	float farPlane = light->Range;

	// Only doing ortho right now - extend later for point & spot if it looks bad
	if (light->IsDirectional())
	{
		float orthoSize = light->Range;
		glm::mat4 lightProjection = glm::ortho(-orthoSize, orthoSize, -orthoSize, orthoSize, nearPlane, farPlane);
		return lightProjection * lightView;
	}
	//else if (light->IsSpot())
	//{
	//	float aspectRatio = 1.0f; // Assuming square shadow map
	//	float fov = light->OuterCone; 
	//	glm::mat4 lightProjection = glm::perspective(fov, aspectRatio, nearPlane, farPlane);
	//	return lightProjection * lightView;
	//}
	//else if (light->IsPoint())
	//{
	//	// Temporary placeholder for point lights
	//	return glm::mat4(1.0f);
	//}

	// Default projection (orthographic)
	glm::mat4 lightProjection = glm::ortho(-light->Range, light->Range, -light->Range, light->Range, nearPlane, farPlane);

	return lightProjection * lightView;
}