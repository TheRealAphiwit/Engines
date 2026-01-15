#pragma once
#include "ShadowMap.h"
#include "EntityHandler.h"
#include <vector>

class Shader;

class ShadowHandler
{
public:
	static ShadowHandler& GetInstance()
	{
		static ShadowHandler instance;
		return instance;
	}

	ShadowHandler(const ShadowHandler&) = delete;
	ShadowHandler& operator=(const ShadowHandler&) = delete;

	void Init(Shader* shadowShader); // Call this in DotGraphics.cpp after creating shadow shader - works like a setter
	
	void RegisterShadowMap(ShadowMap* shadowMap); // Can get LightComponent from ShadowMap
	void UnregisterShadowMap(ShadowMap* shadowMap);
	
	void DrawShadow(Shader* myShadowShader);
	void ShadowPass();

	void UploadShadowData(Shader& mainShader); 

	Shader* myShadowShader; // Created in DotGraphics.cpp - needs Init() to set
	std::vector<std::unique_ptr<ShadowMap>> Shadows; // Handle multiple shadows for multiple lights
	
	glm::mat4 ComputeLightSpaceMatrix(ShadowMap& shadowMap);

private:
	ShadowHandler() = default;
	~ShadowHandler() = default;
};