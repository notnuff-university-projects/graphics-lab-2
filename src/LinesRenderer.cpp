#include "LinesRenderer.h"

#include <glm/gtc/type_ptr.hpp>

#include "defines.h"
#include "glad/glad.h"

void LinesRenderer::Init() {

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Set up the vertex attribute pointers for the position data
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Unbind for now
    glBindVertexArray(0);

    linesShader_ = Shader("./rsc/shaders/gl_lineVertexShader.glsl", "./rsc/shaders/gl_lineFragmentShader.glsl");
}

void LinesRenderer::DrawLine(float x1, float y1, float x2, float y2) {
    // Convert screen coordinates to normalized device coordinates (NDC)

    // Vertex data for the line in NDC
    float vertices[] = {
        x1, y1, 0.0f, 1.0f, // Start point
        x2, y2, 0.0f, 1.0f, // End point
    };

    // Bind the VAO and VBO, update vertex data
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

    // Use the shader program and draw the line
    linesShader_.use();
    // glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(WIN_WIDTH), 0.0f, static_cast<float>(WIN_HEIGHT));
    // glUniformMatrix4fv(glGetUniformLocation(linesShader_.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    //

    glUseProgram(linesShader_.ID);
    glDrawArrays(GL_LINES, 0, 2);

    glBindVertexArray(0); // Unbind VAO for safety
}
