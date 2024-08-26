#include <cstdio>
#include <string>
#include <cerrno>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/vec3.hpp>

#include "Renderer/ShaderProgram.h"

void OnFramebufferResize(GLFWwindow* pWindow, int width, int height);
void ProcessInput(GLFWwindow* pWindow);

const float vertices[] = {
    // position             color
    0.0f, 0.5f, 0.0f,       1.0f, 0.0f, 0.0f,     // top middle     red
    -0.5f, -0.5f, 0.0f,     0.0f, 1.0f, 0.0f,     // lower left     green
    0.5f, -0.5f, 0.0f,      0.0f, 0.0f, 1.0f,     // lower right    blue
};

const unsigned int indices[] = {
    0, 1, 2,
};

int main(int argc, char** argv)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // create our window
    const int WINDOW_WIDTH = 1280;
    const int WINDOW_HEIGHT = 720;
    GLFWwindow* pWindow = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Orca", nullptr, nullptr);
    if (!pWindow)
    {
        printf("Failed to create OpenGL window\n");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(pWindow);

    // initialize GLAD to get subsequent OpenGL functions
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        printf("Failed to initialize GLAD\n");
        glfwTerminate();
        return -1;
    }

    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glfwSetFramebufferSizeCallback(pWindow, OnFramebufferResize);

    // compile and link the shader program
    // -----------------------------------
    ShaderProgram shaderProgram("assets/shaders/basic.vert", "assets/shaders/basic.frag");

    // set up vertex data and attributes
    // ------------------------------
    GLuint vao, vbo, ebo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glClearColor(0.15f, 0.15f, 0.15f, 1.0f);

    while (!glfwWindowShouldClose(pWindow))
    {
        // input
        // -----
        ProcessInput(pWindow);

        // render
        // ------
        glClear(GL_COLOR_BUFFER_BIT);
        glBindVertexArray(vao);
        shaderProgram.Bind();
        shaderProgram.SetUniform("time", glfwGetTime());
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

        // swap buffers and poll IO events
        // -------------------------------
        glfwSwapBuffers(pWindow);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void OnFramebufferResize(GLFWwindow* pWindow, int width, int height)
{
    glViewport(0, 0, width, height);
}

void ProcessInput(GLFWwindow* pWindow)
{
    if (glfwGetKey(pWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(pWindow, GLFW_TRUE);
    }
}