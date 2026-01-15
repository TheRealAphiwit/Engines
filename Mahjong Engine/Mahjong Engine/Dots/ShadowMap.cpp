#include "ShadowMap.h"
#include <iostream>

ShadowMap::ShadowMap() {}

ShadowMap::~ShadowMap()
{
	Destroy();
}

void ShadowMap::Destroy()
{
	if (FBO != 0)
	{
		glDeleteFramebuffers(1, &FBO);
		FBO = 0;
	}
	if (DepthTexture != 0)
	{
		glDeleteTextures(1, &DepthTexture);
		DepthTexture = 0;
	}
}

void ShadowMap::Init()
{
	// Create depth texture
	glGenTextures(1, &DepthTexture);
	glBindTexture(GL_TEXTURE_2D, DepthTexture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, Width, Height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

	// Shadow params
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// Prevent outside shadow edges
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	// Create framebuffer
	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, DepthTexture, 0);

	// No color
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "ERROR: Shadow framebuffer not complete!" << std::endl;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ShadowMap::BindForWriting()
{
	glViewport(0, 0, Width, Height);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glClear(GL_DEPTH_BUFFER_BIT); // Clear for next use
}

void ShadowMap::Unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0); // Bind default framebuffer - meaning; we're back to normal rendering
}