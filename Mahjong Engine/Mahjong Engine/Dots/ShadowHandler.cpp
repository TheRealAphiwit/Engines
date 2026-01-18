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

	// Enable culling
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

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

	// Restore default culling
	glCullFace(GL_FRONT);
	// glDisable(GL_CULL_FACE);
}

// [DONE]
void ShadowHandler::UploadShadowData(Shader& mainShader)
{
	int shadowIndex = 0;
	int startSlot = 10; // Adding shadows to a higher texture

	for (auto& shadow : Shadows)
	{
		std::string index = std::to_string(shadowIndex);

		// Bind shadow map texture to appropriate texture unit
		glActiveTexture(GL_TEXTURE0 + startSlot + shadowIndex);
		glBindTexture(GL_TEXTURE_2D, shadow->DepthTexture);

		// Set the shadow map in the shader GLSL
		mainShader.SetInt(startSlot + shadowIndex, "shadowMaps[" + index + "]"); // frag part

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
	
	if (glm::length(lightDir) < 0.0001f)
		lightDir = glm::vec3(0, -1, 0);

	lightDir = glm::normalize(lightDir);

	float range = glm::max(light->Range, 20.0f);
	glm::vec3 lightPos = -lightDir * range * 2.0f;

	glm::vec3 up = glm::abs(lightDir.y) > 0.95f ? glm::vec3(0, 0, 1) : glm::vec3(0, 1, 0);

	glm::mat4 lightView = glm::lookAt(lightPos, glm::vec3(0.0f), up); 

	float nearPlane = 0.1f;
	float farPlane = range * 4.0f;

	// Only doing ortho right now - extend later for point & spot if it looks bad
	if (light->IsDirectional())
	{
		// float orthoSize = light->Range;
		// [update: 26-01-18] we now use range instead of ortheSize
		glm::mat4 lightProjection = glm::ortho(-range, range, -range, range, nearPlane, farPlane);
		return lightProjection * lightView;
	}

	glm::mat4 lightProjection = glm::ortho(-light->Range, light->Range, -light->Range, light->Range, nearPlane, farPlane);

	return lightProjection * lightView;
}