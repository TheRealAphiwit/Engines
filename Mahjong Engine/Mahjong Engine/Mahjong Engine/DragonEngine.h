#pragma once
#include "GameObject.h"
#include <vector>

struct GLFWwindow;

namespace DotsRendering
{
	class Camera;
	class Graphics; // Not imp
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

		// No defs
		void StartSimulating();
		void StopSimulating();
		const bool& IsSimulating();

		std::vector<GameObject*> GetGameObjects();

		DotsRendering::Camera* myCamera;
		FlyingCamera* myFlyingCamera;

	private:
		bool ShouldSimulate = false;
		Input* myInput;
		ETime* myTime;
		std::vector<GameObject*> myGameObjects;

		GLFWwindow* myWindow;
	};
}