#include <cstdio>
#include <string>
#include <cerrno>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Core/InputManager.h"
#include "Renderer/Camera.h"
#include "Renderer/Mesh.h"
#include "Renderer/Model.h"
#include "Renderer/Shader.h"
#include "Renderer/Texture.h"
#include "Renderer/TextureManager.h"

void OnFramebufferResize(GLFWwindow* pWindow, int width, int height);
void ProcessInput(GLFWwindow* pWindow);

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoord;
};

const Vertex vertices[] = {
	// position					// normal				// texCoord
	{{-0.5f, 0.5f, 0.5f},		{0.0f, 0.0f, 1.0f},		{0.0f, 1.0f}},		// front upper left
	{{-0.5f, -0.5f, 0.5f},		{0.0f, 0.0f, 1.0f},		{0.0f, 0.0f}},		// front lower left
	{{0.5f, -0.5f, 0.5f},		{0.0f, 0.0f, 1.0f},		{1.0f, 0.0f}},		// front lower right
	{{0.5f, 0.5f, 0.5f},		{0.0f, 0.0f, 1.0f},		{1.0f, 1.0f}},		// front upper right

	{{0.5f, 0.5f, -0.5f},		{0.0f, 0.0f, -1.0f},	{0.0f, 1.0f}},		// back upper left
	{{0.5f, -0.5f, -0.5f},		{0.0f, 0.0f, -1.0f},	{0.0f, 0.0f}},		// back lower left
	{{-0.5f, -0.5f, -0.5f},		{0.0f, 0.0f, -1.0f},	{1.0f, 0.0f}},		// back lower right
	{{-0.5f, 0.5f, -0.5f},		{0.0f, 0.0f, -1.0f},	{1.0f, 1.0f}},		// back upper right

	{{-0.5f, 0.5f, -0.5f},		{-1.0f, 0.0f, 0.0f},	{0.0f, 1.0f}},		// left upper left
	{{-0.5f, -0.5f, -0.5f},		{-1.0f, 0.0f, 0.0f},	{0.0f, 0.0f}},		// left lower left
	{{-0.5f, -0.5f, 0.5f},		{-1.0f, 0.0f, 0.0f},	{1.0f, 0.0f}},		// left lower right
	{{-0.5f, 0.5f, 0.5f},		{-1.0f, 0.0f, 0.0f},	{1.0f, 1.0f}},		// left upper right

	{{0.5f, 0.5f, 0.5f},		{1.0f, 0.0f, 0.0f},		{0.0f, 1.0f}},		// right upper left
	{{0.5f, -0.5f, 0.5f},		{1.0f, 0.0f, 0.0f},		{0.0f, 0.0f}},		// right lower left
	{{0.5f, -0.5f, -0.5f},		{1.0f, 0.0f, 0.0f},		{1.0f, 0.0f}},		// right lower right
	{{0.5f, 0.5f, -0.5f},		{1.0f, 0.0f, 0.0f},		{1.0f, 1.0f}},		// right upper right

	{{-0.5f, 0.5f, -0.5f},		{0.0f, 1.0f, 0.0f},		{0.0f, 1.0f}},		// top upper left
	{{-0.5f, 0.5f, 0.5f},		{0.0f, 1.0f, 0.0f},		{0.0f, 0.0f}},		// top lower left
	{{0.5f, 0.5f, 0.5f},		{0.0f, 1.0f, 0.0f},		{1.0f, 0.0f}},		// top lower right
	{{0.5f, 0.5f, -0.5f},		{0.0f, 1.0f, 0.0f},		{1.0f, 1.0f}},		// top upper right

	{{-0.5f, -0.5f, 0.5f},		{0.0f, -1.0f, 0.0f},	{0.0f, 1.0f}},		// bottom upper left
	{{-0.5f, -0.5f, -0.5f},		{0.0f, -1.0f, 0.0f},	{0.0f, 0.0f}},		// bottom lower left
	{{0.5f, -0.5f, -0.5f},		{0.0f, -1.0f, 0.0f},	{1.0f, 0.0f}},		// bottom lower right
	{{0.5f, -0.5f, 0.5f},		{0.0f, -1.0f, 0.0f},	{1.0f, 1.0f}},		// bottom upper right
};

const unsigned int indices[] = {
	0, 1, 2,
	2, 3, 0,

	4, 5, 6,
	6, 7, 4,

	8, 9, 10,
	10, 11, 8,

	12, 13, 14,
	14, 15, 12,

	16, 17, 18,
	18, 19, 16,

	20, 21, 22,
	22, 23, 20,
};

int main(int argc, char** argv)
{
	// GLFW Initialization
	// --------------------------------------------------------------------------
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
	glfwSwapInterval(0);

	// initialize GLAD to get subsequent OpenGL functions
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		printf("Failed to initialize GLAD\n");
		glfwTerminate();
		return -1;
	}

	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	glfwSetFramebufferSizeCallback(pWindow, OnFramebufferResize);

	// subsystem initialization
	// --------------------------------------------------------------------------
	InputManager* pInputManager = InputManager::GetInstance();
	pInputManager->SetContext(pWindow);
	TextureManager* pTextureManager = TextureManager::GetInstance();

	// compile and link shaders
	// --------------------------------------------------------------------------
	Shader solidShader("assets/shaders/solid_color.vert", "assets/shaders/solid_color.frag");

	// set up vertex data and attributes
	// --------------------------------------------------------------------------
	Model model;
	{
		 auto start = glfwGetTime();
		 model.LoadModel("assets/models/sponza/sponza.obj");
		 auto end = glfwGetTime();
		 printf("Loading model (assimp) took %fms\n", (end - start) * 1000.0f);
	}

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
	// normals
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	glEnableVertexAttribArray(1);
	// uvs
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));
	glEnableVertexAttribArray(2);

	glEnable(GL_DEPTH_TEST);
	glClearColor(0.15f, 0.15f, 0.15f, 1.0f);

	float rotationSpeed = 45.0f;
	float translationLimit = 0.4f;
	float scaleSpeed = 2.0f;

	// camera
	// --------------------------------------------------------------------------
	const float fov = 90.0f;
	const float nearPlane = 0.1f;
	const float farPlane = 1000.0f;
	Camera camera(WINDOW_WIDTH, WINDOW_HEIGHT, fov, nearPlane, farPlane);
	camera.SetPosition(glm::vec3(7.0f, 1.0f, -1.85f));
	camera.LookAt(glm::vec3(0.0f, 0.8f, -1.85f));
	camera.SetMovementSpeed(2.0f);

	// transforms
	// --------------------------------------------------------------------------
	glm::mat4 modelTransform(1.0f);
	modelTransform = glm::translate(modelTransform, glm::vec3(-1.0f, -1.0f, -1.5f));
	modelTransform = glm::scale(modelTransform, glm::vec3(0.01f, 0.01f, 0.01f));

	glm::mat4 lightTransform = glm::mat4(1.0f);
	lightTransform = glm::translate(lightTransform, glm::vec3(-3.0f, 1.3f, -0.7f));
	lightTransform = glm::scale(lightTransform, glm::vec3(0.25f));

	// lighting data
	// --------------------------------------------------------------------------
	glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
	float ambientStrength = 0.001f;

	solidShader.Bind();
	solidShader.SetUniform("color", lightColor);

	// uniform buffer object for lights and matrices
	// --------------------------------------------------------------------------
	GLuint uboMatrices, uboLighting, uboCamera;

	glGenBuffers(1, &uboMatrices);
	glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
	glBufferData(GL_UNIFORM_BUFFER, 128, nullptr, GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, uboMatrices);
	
	glGenBuffers(1, &uboLighting);
	glBindBuffer(GL_UNIFORM_BUFFER, uboLighting);
	glBufferData(GL_UNIFORM_BUFFER, 32, nullptr, GL_STATIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, 1, uboLighting);

	// set lighting data now
	glBufferSubData(GL_UNIFORM_BUFFER, 0, 12, glm::value_ptr(lightTransform[3]));
	glBufferSubData(GL_UNIFORM_BUFFER, 16, 12, glm::value_ptr(lightColor));
	glBufferSubData(GL_UNIFORM_BUFFER, 28, 4, &ambientStrength);

	glGenBuffers(1, &uboCamera);
	glBindBuffer(GL_UNIFORM_BUFFER, uboCamera);
	glBufferData(GL_UNIFORM_BUFFER, 16, nullptr, GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, 2, uboCamera);

	// set model matrix
	model.SetTransform(modelTransform);

	// start currentTime 1 frame back so we don't get weird timing issues on the first frame
	float deltaTime = 1.0f / 60.0f;
	float currentTime = glfwGetTime() - deltaTime;
	float previousTime = currentTime;

	while (!glfwWindowShouldClose(pWindow))
	{
		previousTime = currentTime;
		currentTime = glfwGetTime();
		deltaTime = currentTime - previousTime;
		printf("Frame time: %2.2fms (%.1f fps)\r", deltaTime * 1000.0f, 1.0f / deltaTime);

		// input
		// ----------------------------------------------------------------------
		ProcessInput(pWindow);

		// update
		// ----------------------------------------------------------------------
		camera.Update(deltaTime);

		const glm::mat4& projectionMatrix = camera.GetProjectionMatrix();
		const glm::mat4& viewMatrix = camera.GetViewMatrix();
		const glm::vec3 lightPos(lightTransform[3]);

		// set matrix uniform buffer data
		glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, 64, glm::value_ptr(projectionMatrix));
		glBufferSubData(GL_UNIFORM_BUFFER, 64, 64, glm::value_ptr(viewMatrix));

		// same for camera
		glBindBuffer(GL_UNIFORM_BUFFER, uboCamera);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, 12, glm::value_ptr(camera.GetPosition()));

		// render
		// ----------------------------------------------------------------------
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		model.Draw();

		glBindVertexArray(vao);
		solidShader.Bind();
		solidShader.SetUniform("model", lightTransform);
		solidShader.SetUniform("view", viewMatrix);
		solidShader.SetUniform("projection", projectionMatrix);
		glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(indices[0]), GL_UNSIGNED_INT, 0);

		// swap buffers and poll IO events
		// ----------------------------------------------------------------------
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