#include "EditorGUI.h"
#include "DragonEngine.h"
#include "ResourceHandler.h"
#include "VirtualObject.h"
#include "DotGraphics.h"

#include <vector>
#include <iostream>
#include <glfw3.h>
#include <stdio.h>

int main()
{
	/*ResourceHandler* resources = new ResourceHandler();
	DotsRendering::DotsInitData initData = DotsRendering::Initialize(1200, 720);
	Engine::DragonEngine* engine = new Engine::DragonEngine(initData.window, initData.camera);*/

    if (!glfwInit()) {
        std::cout << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    GLFWwindow* window;
    window = glfwCreateWindow(1200, 720, "Hello World", NULL, NULL);

    glfwMakeContextCurrent(window);

    EditorGUI myEditor;
    myEditor.Init(window);

    while (true) {
        myEditor.GUIMain();

        glClear(GL_COLOR_BUFFER_BIT);

        glClearColor(0.2f, 0.3f, 0.8f, 1.0f);

        myEditor.Render();

        glfwSwapBuffers(window);
    }

    myEditor.Shutdown();
    return 0;
}