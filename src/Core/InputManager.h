#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

struct GLFWwindow;

enum Key
{
	KEY_W,
	KEY_A,
	KEY_S,
	KEY_D,
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