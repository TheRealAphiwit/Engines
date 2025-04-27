#pragma once

#include "Texture.h"
#include <string>

class Material
{
public:
	// Constructor
	Material(const char* albedoPath, const char* specularPath, const char* normalPath, float shininess);

	// Setters
	void SetAlbedoMap(std::string TextureName, Texture* texture) { AlbedoMapName = TextureName; AlbedoMap = texture; }
	void SetSpecularMap(std::string TextureName, Texture* texture) { SpecularMapName = TextureName; SpecularMap = texture; }
	void SetNormalMap(std::string TextureName, Texture* texture) { NormalMapName = TextureName; NormalMap = texture; }

	// Getters - todo: add name getters
	float GetShininess() const { return Shininess; }
	Texture* GetAlbedoMap() const { return AlbedoMap; }
	std::string GetAlbedoMapName() const { return AlbedoMapName; }

	Texture* GetSpecularMap() const { return SpecularMap; }
	std::string GetSpecularMapName() const { return SpecularMapName; }
	
	Texture* GetNormalMap() const { return NormalMap; }
	std::string GetNormalMapName() const { return NormalMapName; }

protected:
	float Shininess = 0.0f;
	Texture* AlbedoMap = nullptr;
	std::string AlbedoMapName;

	Texture* SpecularMap = nullptr;
	std::string SpecularMapName;

	Texture* NormalMap = nullptr;
	std::string NormalMapName;
};