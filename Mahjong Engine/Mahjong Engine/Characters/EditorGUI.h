#pragma once
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <glfw3.h>

class EditorGUI
{
public:
	void Init(GLFWwindow* YOUR_WINDOW);
	void GUIMain();
	void Render();
	void Shutdown();
};