#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cassert>
#include <thread>
#include <vector>


inline const int WIN_WIDTH = 800;
inline const int WIN_HEIGHT = 600;

GLFWwindow* StartOpenGL() {
    assert( glfwInit() );

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

//    auto monitorsCount = 0;
//    GLFWmonitor** monitors = glfwGetMonitors(&monitorsCount);

    GLFWwindow* win = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, "bruh", nullptr, nullptr);

    assert(win);

    glfwMakeContextCurrent(win);

    assert(gladLoadGLLoader((GLADloadproc) glfwGetProcAddress) );

    return win;
}

void FramebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width/2, height/2);
}

void ProcessInput(GLFWwindow* window) {
    if( glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS ) {
        glfwSetWindowShouldClose(window, true);
    }
}


int main() {
    auto win = StartOpenGL();

    glfwSetFramebufferSizeCallback(win, FramebufferSizeCallback);

    FramebufferSizeCallback(win, WIN_WIDTH, WIN_HEIGHT);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    while ( !glfwWindowShouldClose(win) ) {
        ProcessInput(win);

        glClearColor(0.1f, 0.3f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(win);

        glfwPollEvents();
    }

    glfwTerminate();
}
