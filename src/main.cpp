#include <cstdio>
#include <string>
#include <cerrno>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

void OnFramebufferResize(GLFWwindow* pWindow, int width, int height);
void ProcessInput(GLFWwindow* pWindow);

std::string GetFileContents(const std::string& filename)
{
    std::string contents = "";

    std::FILE* file = std::fopen(filename.c_str(), "rb");
    if (file)
    {
        std::fseek(file, 0, SEEK_END);
        contents.resize(std::ftell(file));
        std::rewind(file);
        std::fread(&contents[0], 1, contents.size(), file);
        std::fclose(file);
    }
    else
    {
        printf("Failed to load contents of file \"%s\". %s\n", filename.c_str(), strerror(errno));
    }

    return contents;
}

const float vertices[] = {
    -0.5f, 0.5f, 0.0f,      // upper left
    -0.5f, -0.5f, 0.0f,     // lower left
    0.5f, -0.5f, 0.0f,      // lower right
    0.5f, 0.5f, 0.0f,       // upper right
};

const unsigned int indices[] = {
    0, 1, 2,
    2, 3, 0,
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

    auto CheckShaderCompileStatus = [](GLuint shader) -> void
    {
        GLint status;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
        if (!status)
        {
            char infoLog[512];
            glGetShaderInfoLog(shader, 512, NULL, infoLog);
            printf("Shader compilation failed. %s\n", infoLog);
        }
    };
    auto CheckProgramLinkStatus = [](GLuint program) -> void
    {
        GLint status;
        glGetProgramiv(program, GL_LINK_STATUS, &status);
        if (!status)
        {
            char infoLog[512];
            glGetProgramInfoLog(program, 512, NULL, infoLog);
            printf("Program linking failed. %s\n", infoLog);
        }
    };

    // compile and link the shader program
    // -----------------------------------
    // vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    std::string vertexShaderSource = GetFileContents("assets/shaders/basic.vert");
    const char* pVertexShaderSource = vertexShaderSource.c_str();
    glShaderSource(vertexShader, 1, &pVertexShaderSource, NULL);
    glCompileShader(vertexShader);
    CheckShaderCompileStatus(vertexShader);

    // fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    std::string fragmentShaderSource = GetFileContents("assets/shaders/basic.frag");
    const char* pFragmentShaderSource = fragmentShaderSource.c_str();
    glShaderSource(fragmentShader, 1, &pFragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    CheckShaderCompileStatus(fragmentShader);

    // link shaders together
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    CheckProgramLinkStatus(shaderProgram);

    // can safely delete the shaders now they've been linked
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

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

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

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
        glUseProgram(shaderProgram);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

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