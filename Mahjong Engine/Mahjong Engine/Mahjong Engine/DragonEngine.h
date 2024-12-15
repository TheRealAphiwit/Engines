#pragma once
struct GLFWwindow;

namespace DotsRendering
{
	class Camera;
}

namespace Engine
{
	class ETime;
	class Input;
	class FlyingCamera;

	class DragonEngine
	{
	public:
		DragonEngine(GLFWwindow* aWindow, DotsRendering::Camera* aCamera);

		void Update(const float& aDeltaTime);

		DotsRendering::Camera* myCamera;
		FlyingCamera* myFlyingCamera;

	private:
		Input* myInput;
		ETime* myTime;

		GLFWwindow* myWindow;
	};
}