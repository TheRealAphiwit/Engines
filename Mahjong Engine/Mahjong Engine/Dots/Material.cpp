#include "Material.h"

Material::Material(std::string materialName, const char* albedoPath, const char* specularPath, const char* normalPath, float shininess) : MaterialName(materialName), Shininess(shininess), AlbedoMapName(albedoPath), SpecularMapName(specularPath), NormalMapName(normalPath)
{
	AlbedoMap = new Texture(albedoPath, true);
	SpecularMap = new Texture(specularPath, true);
	NormalMap = new Texture(normalPath, true);
}

Material::~Material()
{
	delete AlbedoMap;
	delete SpecularMap;
	delete NormalMap;
}