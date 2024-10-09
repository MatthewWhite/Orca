#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include <glm/vec2.hpp>

struct GLFWwindow;

enum Key
{
	KEY_SPACE,
	KEY_W,
	KEY_A,
	KEY_S,
	KEY_D,
	KEY_Q,
	KEY_Z,
	KEY_LSHIFT,
	KEY_LCTRL,
	KEY_LALT,
	// TODO: expand this as new keys are needed...
};

enum KeyState
{
	KS_UP,
	KS_DOWN,
};

enum MouseButton
{
	MOUSE_BUTTON_1,
	MOUSE_BUTTON_2,
	MOUSE_BUTTON_3,
	MOUSE_BUTTON_4,
	MOUSE_BUTTON_5,

	MOUSE_BUTTON_COUNT,
	MOUSE_BUTTON_LEFT = MOUSE_BUTTON_1,
	MOUSE_BUTTON_RIGHT = MOUSE_BUTTON_2,
	MOUSE_BUTTON_MIDDLE = MOUSE_BUTTON_3,
};

enum MouseButtonState
{
	MBS_UP,
	MBS_DOWN,
};

class InputManager
{

public:
	~InputManager();

	static InputManager* GetInstance();

	bool Init();

	KeyState GetKeyState(Key key) const;
	MouseButtonState GetMouseButtonState(MouseButton mb) const;
	glm::vec2 GetMousePosition() const;

	void EnableMouse();
	void DisableMouse();

	void SetContext(GLFWwindow* pWindow) { mWindow = pWindow; }
	bool HasContext() const { return mWindow; }

private:
	InputManager();

	GLFWwindow* mWindow;

	static InputManager* sInstance;
};

#endif