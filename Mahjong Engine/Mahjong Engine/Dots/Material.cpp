#include "Material.h"

Material::Material(const char* albedoPath, const char* specularPath, const char* normalPath, float shininess) : Shininess(shininess)
{
	SetAlbedoMap(new Texture(albedoPath, true));
	SetSpecularMap(new Texture(specularPath, false));
	SetNormalMap(new Texture(normalPath, false));
}
