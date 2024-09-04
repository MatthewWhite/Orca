#include <cstdio>
#include <string>
#include <cerrno>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "Renderer/ShaderProgram.h"
#include "Renderer/Texture.h"

void OnFramebufferResize(GLFWwindow* pWindow, int width, int height);
void ProcessInput(GLFWwindow* pWindow);

struct Vertex
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoord;
};

const Vertex vertices[] = {
    // position                 // normal               // texCoord
    {{0.0f, 0.5f, 0.0f},        {1.0f, 0.0f, 0.0f},     {0.5f, 1.0f}},     // top middle     red
    {{-0.5f, -0.5f, 0.0f},      {0.0f, 1.0f, 0.0f},     {0.0f, 0.0f}},     // lower left     green
    {{0.5f, -0.5f, 0.0f},       {0.0f, 0.0f, 1.0f},     {1.0f, 0.0f}},     // lower right    blue
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

    // initialize our texture loader
    Texture::InitTextureLoader();

    // compile and link the shader program
    // -----------------------------------
    ShaderProgram shaderProgram("assets/shaders/basic.vert", "assets/shaders/basic.frag");
    shaderProgram.SetUniform("texture1", 0);

    // set up vertex data and attributes
    // ---------------------------------
    GLuint vao, vbo, ebo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // NOTE: VAO stores any EBO bound, so don't unbind EBO until VAO is unbound
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
    glEnableVertexAttribArray(0);
    // color/normal
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(1);
    // uvs
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));
    glEnableVertexAttribArray(2);

    // texture
    // -------
    Texture texture1("assets/textures/wall.jpg");

    glClearColor(0.15f, 0.15f, 0.15f, 1.0f);

    while (!glfwWindowShouldClose(pWindow))
    {
        float time = glfwGetTime();

        // input
        // -----
        ProcessInput(pWindow);

        // render
        // ------
        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(vao);
        shaderProgram.Bind();
        glActiveTexture(GL_TEXTURE0);
        texture1.Bind();
        shaderProgram.SetUniform("time", time);
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