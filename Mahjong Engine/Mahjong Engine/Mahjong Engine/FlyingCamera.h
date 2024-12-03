#pragma once

struct GLFWwindow;

namespace DotsRendering
{
	class Camera;
}

namespace Engine
{
	class Input;
	class ETime;

	class FlyingCamera
	{
	public:
		FlyingCamera(DotsRendering::Camera* aCamera, Engine::Input* anInput, Engine::ETime* aTime);
		void Update(GLFWwindow* aWindow);

	private:
		void SetCamState(GLFWwindow* aWindow);
		Engine::Input* myInput;
		Engine::ETime* myTime;
		DotsRendering::Camera* myCamera;

		float yaw, pitch;
		#pragma region Same Thing As
		// float yaw;
		// float pitch;  
		#pragma endregion
		bool Editing = false;
		float lastX, lastY;
	};
}