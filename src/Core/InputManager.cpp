#include "InputManager.h"

#include <GLFW/glfw3.h>

static int KEY_TO_GLFW_MAP[] = 
{
	GLFW_KEY_SPACE,
	GLFW_KEY_W,
	GLFW_KEY_A,
	GLFW_KEY_S,
	GLFW_KEY_D,
	GLFW_KEY_Q,
	GLFW_KEY_Z,
	GLFW_KEY_ENTER,
	GLFW_KEY_LEFT_SHIFT,
	GLFW_KEY_LEFT_CONTROL,
	GLFW_KEY_LEFT_ALT,
};

static int MOUSE_TO_GLFW_MAP[] =
{
	GLFW_MOUSE_BUTTON_1,
	GLFW_MOUSE_BUTTON_2,
	GLFW_MOUSE_BUTTON_3,
	GLFW_MOUSE_BUTTON_4,
	GLFW_MOUSE_BUTTON_5,

	GLFW_MOUSE_BUTTON_LAST,
	GLFW_MOUSE_BUTTON_LEFT,
	GLFW_MOUSE_BUTTON_RIGHT,
	GLFW_MOUSE_BUTTON_MIDDLE,
};

InputManager* InputManager::sInstance = nullptr;

InputManager::InputManager()
	: mWindow(nullptr)
{
}

InputManager::~InputManager()
{
}

InputManager* InputManager::GetInstance()
{
	if (!sInstance)
	{
		sInstance = new InputManager();
	}

	return sInstance;
}

bool InputManager::Init()
{
	EnableMouse();
	return true;
}

KeyState InputManager::GetKeyState(Key key) const
{
	return (KeyState)glfwGetKey(mWindow, KEY_TO_GLFW_MAP[key]);
}

MouseButtonState InputManager::GetMouseButtonState(MouseButton mb) const
{
	return (MouseButtonState)glfwGetMouseButton(mWindow, MOUSE_TO_GLFW_MAP[mb]);
}

void InputManager::EnableMouse()
{
	glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void InputManager::DisableMouse()
{
	glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

glm::vec2 InputManager::GetMousePosition() const
{
	double x, y;
	glfwGetCursorPos(mWindow, &x, &y);
	return glm::vec2((float)x, (float)y);
}