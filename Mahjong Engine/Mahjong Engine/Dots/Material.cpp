#include "Material.h"

Material::Material(const char* albedoPath, const char* specularPath, const char* normalPath, float shininess) 
	: Shininess(shininess),
	AlbedoMapName(albedoPath),
	SpecularMapName(specularPath),
	NormalMapName(normalPath)
{
	AlbedoMap = new Texture(albedoPath, true);
	SpecularMap = new Texture(specularPath, true);
	NormalMap = new Texture(normalPath, true);
}