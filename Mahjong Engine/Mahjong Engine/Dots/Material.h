#pragma once

#include "Texture.h"
#include "Component.h"
#include <string>

// Future imp: not every material will have normal maps sent in -> we'll make some default files for a white material

class Material : public Component
{
public:
	// Constructor
	Material(std::string materialName, const char* albedoPath, const char* specularPath, const char* normalPath, float shininess);

	// Destructor (to free memory)
	~Material();

	// Getters - todo: add name getters
	float GetShininess() const { return Shininess; }
	Texture* GetAlbedoMap() const { return AlbedoMap; }
	std::string GetAlbedoMapName() const { return AlbedoMapName; }

	Texture* GetSpecularMap() const { return SpecularMap; }
	std::string GetSpecularMapName() const { return SpecularMapName; }

	Texture* GetNormalMap() const { return NormalMap; }
	std::string GetNormalMapName() const { return NormalMapName; }

protected:
	std::string MaterialName;

	float Shininess = 0.0f;
	Texture* AlbedoMap = nullptr;
	std::string AlbedoMapName;

	Texture* SpecularMap = nullptr;
	std::string SpecularMapName;

	Texture* NormalMap = nullptr;
	std::string NormalMapName;
};