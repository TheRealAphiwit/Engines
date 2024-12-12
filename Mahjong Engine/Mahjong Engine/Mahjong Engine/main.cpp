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
	ResourceHandler* resources = new ResourceHandler();
	DotsRendering::DotsInitData initData = DotsRendering::Initialize(1200, 720);
    // Issue: initData.window is not being initialized for some reason?? Solved!
    if (!initData.window) {
        std::cerr << "Failed to initialize rendering window" << std::endl;
        return -2;
    }
    Engine::DragonEngine* engine = new Engine::DragonEngine(initData.window, initData.camera);
    Characters::EditorGUI* Gui = new Characters::EditorGUI(initData.window, resources);

    glfwSetInputMode(initData.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    float lastTime = 0;
    float currentTime = 0;
    float delta = 0;

    std::vector<VirtualObject*> objects;

    while (!DotsRendering::ShouldClose())
    {
        objects = DotsRendering::GetObjects();
        currentTime = (float)glfwGetTime();
        delta = currentTime - lastTime;
        lastTime = currentTime;

        DotsRendering::BeginRender(engine->myCamera);
        Gui->Render(objects);
        DotsRendering::End();

        engine->Update(delta);

        // std message to know that it's looping
        // std::cout << "Engine is looping" << std::endl;
    }

    return 0;

    /*if (!glfwInit()) {
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
    return 0;*/
}