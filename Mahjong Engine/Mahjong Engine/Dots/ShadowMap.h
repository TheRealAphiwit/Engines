#pragma once
#include <glad/glad.h>
#include "LightComponent.h"

class ShadowMap // Equivalent of ShadowData
{
public:
	ShadowMap();
	~ShadowMap();
	void Destroy(); // Manual destructor for more control

	GLuint FBO = 0;
	GLuint DepthTexture = 0;
	int Width = 1024;
	int Height = 1024;

	void Init();
	void BindForWriting();
	void Unbind();

	LightComponent* OwnerLight = nullptr; // Light that owns this shadow map
};