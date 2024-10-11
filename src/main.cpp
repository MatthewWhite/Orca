#include <cstdio>
#include <string>
#include <cerrno>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Core/InputManager.h"
#include "Renderer/Camera.h"
#include "Renderer/Mesh.h"
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
	Texture::InitTextureLoader();
	InputManager* pInputManager = InputManager::GetInstance();
	pInputManager->SetContext(pWindow);

	// compile and link shaders
	// --------------------------------------------------------------------------
	ShaderProgram standardShader("assets/shaders/standard.vert", "assets/shaders/standard.frag");
	ShaderProgram blendedShader("assets/shaders/blended_textures.vert", "assets/shaders/blended_textures.frag");
	ShaderProgram solidShader("assets/shaders/solid_color.vert", "assets/shaders/solid_color.frag");
	ShaderProgram phongShader("assets/shaders/phong_solid.vert", "assets/shaders/phong_solid.frag");

	// set up vertex data and attributes
	// --------------------------------------------------------------------------
	Mesh cubeMesh;
	cubeMesh.Load("assets/models/teapot.obj");

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

	// textures
	// --------------------------------------------------------------------------
	Texture texture1("assets/textures/container.jpg");
	Texture texture2("assets/textures/awesomeface.png");
	Texture diffuse("assets/textures/container2_d.png");
	Texture specular("assets/textures/container2_s.png");
	blendedShader.Bind();
	blendedShader.SetUniform("texture1", 0);
	blendedShader.SetUniform("texture2", 1);
	standardShader.Bind();
	standardShader.SetUniform("diffuseMap", 0);
	standardShader.SetUniform("specularMap", 1);

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
	camera.SetPosition(glm::vec3(0.0f, 0.0f, 3.0f));
	camera.LookAt(glm::vec3(0.0f));
	camera.SetMovementSpeed(2.0f);

	// cube transforms
	// --------------------------------------------------------------------------
	glm::mat4 containerTransform = glm::mat4(1.0f);
	containerTransform = glm::translate(containerTransform, glm::vec3(0.0f, 0.0f, 0.0f));

	glm::mat4 transform = glm::mat4(1.0f);
	transform = glm::translate(transform, glm::vec3(1.5f, 0.3f, -2.0f));
	transform = glm::rotate(transform, glm::radians(60.0f), glm::vec3(1.0f, 0.3f, 0.5f));

	//glm::mat4 solidBoxTransform = glm::mat4(1.0f);
	//solidBoxTransform = glm::translate(solidBoxTransform, glm::vec3(-1.3f, 0.0f, -1.5f));
	float rotationAngle = 0.0f;
	float degreesPerSecond = 45.0f;
	glm::vec3 offset(-1.7f, 0.0f, 0.0f);
	glm::vec3 solidBoxRotationPoint(-2.2f, -1.7f, -0.7f);

	glm::mat4 lightTransform = glm::mat4(1.0f);
	lightTransform = glm::translate(lightTransform, glm::vec3(-2.2f, 1.3f, -0.7f));
	lightTransform = glm::scale(lightTransform, glm::vec3(0.25f));

	// lighting data
	// --------------------------------------------------------------------------
	glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
	float ambientStrength = 0.025f;

	blendedShader.Bind();
	blendedShader.SetUniform("lightColor", lightColor);
	blendedShader.SetUniform("ambientStrength", ambientStrength);
	blendedShader.SetUniform("smoothness", 2.0f);
	blendedShader.SetUniform("specularIntensity", 0.0f);
	standardShader.Bind();
	standardShader.SetUniform("lightColor", lightColor);
	standardShader.SetUniform("ambientStrength", ambientStrength);
	standardShader.SetUniform("smoothness", 128.0f);
	phongShader.Bind();
	phongShader.SetUniform("lightColor", lightColor);
	phongShader.SetUniform("color", glm::vec3(1.0f, 0.36f, 0.22f));
	phongShader.SetUniform("ambientStrength", ambientStrength);
	phongShader.SetUniform("smoothness", 64.0f);
	phongShader.SetUniform("specularIntensity", 0.125f);
	solidShader.Bind();
	solidShader.SetUniform("color", lightColor);

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
		containerTransform = glm::rotate(containerTransform, glm::radians(rotationSpeed) * deltaTime, glm::vec3(1.0f, 1.0f, 1.0f));

		rotationAngle += degreesPerSecond * deltaTime;
		glm::mat4 solidBoxTransform(1.0f);
		solidBoxTransform = glm::translate(solidBoxTransform, glm::vec3(-1.0f, -1.0f, 1.5f));
		//solidBoxTransform = glm::translate(solidBoxTransform, solidBoxRotationPoint);
		//solidBoxTransform = glm::rotate(solidBoxTransform, glm::radians(rotationAngle), glm::vec3(0.0f, 1.0f, 0.0f));
		//solidBoxTransform = glm::translate(solidBoxTransform, offset);
		//solidBoxTransform = glm::rotate(solidBoxTransform, glm::radians(rotationAngle * 2), glm::vec3(0.0f, 1.0f, 0.0f));

		const glm::mat4& projectionMatrix = camera.GetProjectionMatrix();
		const glm::mat4& viewMatrix = camera.GetViewMatrix();
		const glm::vec3 lightPos(lightTransform[3]);

		// render
		// ----------------------------------------------------------------------
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// bind our textures
		glActiveTexture(GL_TEXTURE0);
		texture1.Bind();
		glActiveTexture(GL_TEXTURE1);
		texture2.Bind();

		// bind our matrices
		blendedShader.Bind();
		blendedShader.SetUniform("viewPos", camera.GetPosition());
		blendedShader.SetUniform("model", transform);
		blendedShader.SetUniform("view", viewMatrix);
		blendedShader.SetUniform("projection", projectionMatrix);
		blendedShader.SetUniform("lightPosition", lightPos);

		// do the actual rendering
		glBindVertexArray(vao);
		glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(indices[0]), GL_UNSIGNED_INT, 0);

		// repeat for container
		glActiveTexture(GL_TEXTURE0);
		diffuse.Bind();
		glActiveTexture(GL_TEXTURE1);
		specular.Bind();

		standardShader.Bind();
		standardShader.SetUniform("viewPos", camera.GetPosition());
		standardShader.SetUniform("model", containerTransform);
		standardShader.SetUniform("view", viewMatrix);
		standardShader.SetUniform("projection", projectionMatrix);
		standardShader.SetUniform("lightPosition", lightPos);
		glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(indices[0]), GL_UNSIGNED_INT, 0);

		// repeat for phong cube
		phongShader.Bind();
		phongShader.SetUniform("viewPos", camera.GetPosition());
		phongShader.SetUniform("model", solidBoxTransform);
		phongShader.SetUniform("view", viewMatrix);
		phongShader.SetUniform("projection", projectionMatrix);
		phongShader.SetUniform("lightPosition", lightPos);
		cubeMesh.Draw();
		//glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(indices[0]), GL_UNSIGNED_INT, 0);

		// repeat for light
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