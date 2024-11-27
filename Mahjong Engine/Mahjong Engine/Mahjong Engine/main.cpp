#include <iostream>
#include <glfw3.h>
#include "EditorGUI.h"

int main()
{
    if (!glfwInit()) {
        std::cout << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    GLFWwindow* window;
    window = glfwCreateWindow(1920, 1080, "Hello World", NULL, NULL);

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