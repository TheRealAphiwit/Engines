#include <iostream>
#include <glfw3.h>

int main()
{
    if (!glfwInit()) {
        std::cout << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    GLFWwindow* window;
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);

    glfwMakeContextCurrent(window);

    while (true) {
        glClear(GL_COLOR_BUFFER_BIT);

        glClearColor(0.2f, 0.3f, 0.8f, 1.0f);

        glfwSwapBuffers(window);
    }

    return 0;
}