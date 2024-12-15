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
	DotsRendering::DotsInitData renderData = DotsRendering::Initialize(1200, 720);
    // Here
    Engine::DragonEngine* engine = new Engine::DragonEngine(renderData.window, renderData.camera);
    Characters::EditorGUI* Gui = new Characters::EditorGUI(renderData.window, resources);

    glfwSetInputMode(renderData.window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

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
    }

	glfwTerminate();
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