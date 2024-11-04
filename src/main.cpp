#include <iostream>
#include <bit>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <cassert>
#include <thread>
#include <ranges>
#include <string>
#include <vector>


#include "gl_vertexShader.h"
#include "gl_fragmentShader.h"


inline const int WIN_WIDTH = 800;
inline const int WIN_HEIGHT = 600;


GLFWwindow* StartOpenGL() {
    assert( glfwInit() );

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwWindowHint(GLFW_SAMPLES, 4);

//    auto monitorsCount = 0;
//    GLFWmonitor** monitors = glfwGetMonitors(&monitorsCount);

    GLFWwindow* win = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, "bruh", nullptr, nullptr);

    assert(win);

    glfwMakeContextCurrent(win);

    assert(gladLoadGLLoader((GLADloadproc) glfwGetProcAddress) );

    return win;
}

void FramebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(width/4, height/4, width/2, height/2);
}

void ProcessInput(GLFWwindow* window) {
    if( glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS ) {
        glfwSetWindowShouldClose(window, true);
    }
}


GLuint CompileVertexShader(const char* vertexShaderSource) {
    // here we compile our vertex shader
    auto vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);
    // ~

    {    // here we check whether the compilation was OK
        int success;
        char infoLog[512];
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        }
        // ~
    }
    return vertexShader;
}

GLuint CompileFragmentShader(const char* fragmentShaderSource) {
    // here we compile our fragment shader
    auto fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);
    // ~

    {
        // here we check whether the compilation was OK
        int success;
        char infoLog[512];
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
            std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        }
        // ~
    }
    return fragmentShader;
}

GLuint CompileProgram(const char *vertexShaderCode, const char *fragmentShaderCode) {
    // here we link all shaders with shader program
    auto shaderProgram = glCreateProgram();

    auto vertexShader = CompileVertexShader(vertexShaderCode);
    auto fragmentShader = CompileFragmentShader(fragmentShaderCode);

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    {
        // here we check whether the compilation was OK
        int success;
        char infoLog[512];
        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
            // ~
        }
    }
    // ~

    // here we tell the opengl to use our fresh-created program
    glUseProgram(shaderProgram);
    // !

    // here we delete the shaders we linked, as we don`t need them anymore,
    // `cause they are in our shader program already
    // glDeleteShader(vertexShader);
    // glDeleteShader(fragmentShader);
    // ~
    return shaderProgram;
}

struct VBOandVAO {
    unsigned int VBO;
    unsigned int VAO;
};

VBOandVAO GenerateVBOandVAO(const std::vector<float>& vertices,  const std::vector<int>& indices) {
    // 0. generate(allocate) VAO and VBO and EBO
    unsigned int VBO, VAO; //Vertex Buffer Object

    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);

    unsigned int EBO; // Element Buffer Object
    glGenBuffers(1, &EBO);

    // 1. bind VAO, so all next actions will be applied to current VAO

    glBindVertexArray(VAO);
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


    // 2. bind and set VBO

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // here we transfer our array data to our VBO buffer
    // GL_ARRAY_BUFFER -> target buffer that we are going to use
    // GL_STATIC_DRAW -> tip for GPU how to use memory of VBO
    // see also other tips for GPU

    const auto vsize = sizeof(float) * vertices.size();
    glBufferData(GL_ARRAY_BUFFER, vsize, vertices.data(), GL_STATIC_DRAW);
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    // 2.1 bind and set EBO
    const auto isize = sizeof(int) * indices.size();

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, isize, indices.data(), GL_STATIC_DRAW);


    // 3. configure Vertex Attributes

    // with this function, we say to opengl how to interpret vertices data
    // in this case, we want to configure the location attribute of vertex,
    // as in our shader program before we defined
    // location = 0, so location attribute in our program will have
    // index 0
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
    // ~

    // enable vertex attribute we defined before for index 0
    glEnableVertexAttribArray(0);
    // ~
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


    // 4. clear VBO and VAO
    // VBO was bound to Vertex Attribute=0 (location)
    // as buffer object, with glVertexAttribPointer()
    // so this VBO is saved in VAO, and we can now unbind it from GL_ARRAY_BUFFER

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // unbind now VAO, as we saved it and it no longer have to be active VAO
    glBindVertexArray(0);

    return {VBO, VAO};
}

int main() {

    auto win = StartOpenGL();

    // bruh, it is not working on wayland
    GLFWimage icons[1];
    icons[0].pixels = stbi_load("./img/icon.png", &icons[0].width, &icons[0].height, 0, 4); //rgba channels
    glfwSetWindowIcon(win, 1, icons);
    stbi_image_free(icons[0].pixels);


    glEnable(GL_MULTISAMPLE);
    glfwSwapInterval(0);

    glfwSetFramebufferSizeCallback(win, FramebufferSizeCallback);
    FramebufferSizeCallback(win, WIN_WIDTH, WIN_HEIGHT);

    auto shaderProgram1 = CompileProgram(vertexShaderSource, fragmentShaderSource);


    std::vector<float> vertices = {
        -0.5f, -0.5f, 0.0f,
        -0.5f, 0.5f, 0.0f,
        0.5f, 0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
    };

    for (int i = 0; i < 2500000; ++i) {
        vertices.push_back(std::rand());
    }


    std::vector indices = {  // note that we start from 0!
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9,    // first triangle
    };


    auto first = GenerateVBOandVAO(vertices, indices);

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    std::vector modes = {
        GL_LINE,
        // GL_FILL,
        // GL_POINT,
    };

    auto currMode = modes.begin();
    auto i = 0;
    while ( !glfwWindowShouldClose(win) ) {
        ProcessInput(win);

        if( currMode == modes.end() ) currMode = modes.begin();

        glPolygonMode(GL_FRONT_AND_BACK, *currMode);

        glClearColor(0.1f, 0.3f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram1);

        glBindVertexArray(first.VAO);
        glDrawElements(GL_LINES, 5, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glfwSwapBuffers(win);
        glfwPollEvents();

        if( !(i % 1000) )++currMode;
        ++i;
    }

    // free our VAO and VBO
    glDeleteVertexArrays(1, &first.VAO);

    glDeleteBuffers(1, &first.VBO);

    glfwTerminate();
}
