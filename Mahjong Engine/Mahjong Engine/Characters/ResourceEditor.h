#pragma once

class ResourceHandler;
class Texture; // Forward declaration if it maybe helps

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