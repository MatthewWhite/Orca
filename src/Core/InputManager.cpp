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
	GLFW_KEY_LEFT_SHIFT,
	GLFW_KEY_LEFT_CONTROL,
	GLFW_KEY_LEFT_ALT,
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

KeyState InputManager::GetKeyState(Key key) const
{
	return (KeyState)glfwGetKey(mWindow, KEY_TO_GLFW_MAP[key]);
}