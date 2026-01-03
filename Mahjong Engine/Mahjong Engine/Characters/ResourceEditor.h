#pragma once

class ResourceHandler;
class __declspec(dllimport) Texture;

enum class EResource
{
	Texture,
	Shader,
	Mesh,
	COUNT
};

class ResourceEditor
{
public:
	ResourceEditor(ResourceHandler* aHandler);
	~ResourceEditor();

	void Update();

private:
	ResourceHandler* myResources;
	EResource myCurrentResource;
};