#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

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

class InputManager
{

public:
	~InputManager();

	static InputManager* GetInstance();

	KeyState GetKeyState(Key key) const;

	void SetContext(GLFWwindow* pWindow) { mWindow = pWindow; }
	bool HasContext() const { return mWindow; }

private:
	InputManager();

	GLFWwindow* mWindow;

	static InputManager* sInstance;
};

#endif