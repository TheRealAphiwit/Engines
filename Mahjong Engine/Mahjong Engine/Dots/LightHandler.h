#pragma once
#include <vector>

class LightComponent;
class Shader;
class VirtualObject;

class LightHandler
{
public:
	static LightHandler& GetInstance()
	{
		static LightHandler instance;
		return instance;
	}
	LightHandler(const LightHandler&) = delete;
	LightHandler& operator=(const LightHandler&) = delete;

	void RegisterLight(LightComponent* light);
	void UnregisterLight(LightComponent* light);

	void UploadLightsToShader(Shader& shader) const;

	const std::vector<LightComponent*>& GetLights() const { return myLights; };

private:
	LightHandler() = default;
	~LightHandler() = default;

	std::vector<LightComponent*> myLights;
};