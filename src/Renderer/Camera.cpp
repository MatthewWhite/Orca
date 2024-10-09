#include "Camera.h"

#include <cstdio>

#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/norm.hpp>

#include "Core/InputManager.h"

namespace
{
	const glm::vec3 WORLD_ORIGIN(0.0f);
	const glm::vec3 WORLD_RIGHT(1.0f, 0.0f, 0.0f);
	const glm::vec3 WORLD_UP(0.0f, 1.0f, 0.0f);
	const glm::vec3 WORLD_FORWARD(0.0f, 0.0f, 1.0f);

	const float MAX_PITCH = glm::radians(89.0f);
	const float MIN_PITCH = glm::radians(-89.0f);
}

Camera::Camera(int width, int height, float fov, float near, float far)
	: mProjectionMatrix(1.0f)
	, mViewMatrix(glm::lookAt(WORLD_ORIGIN, WORLD_FORWARD, WORLD_UP))
	, mPosition(WORLD_ORIGIN)
	, mRight(WORLD_RIGHT)
	, mUp(WORLD_UP)
	, mForward(WORLD_FORWARD)
	, mLastFrameMousePos(0.0f, 0.0f)
	, mWidth(width)
	, mHeight(height)
	, mFov(fov)
	, mNearDistance(near)
	, mFarDistance(far)
	, mYaw(0.0f)
	, mPitch(0.0f)
	, mMovementSpeed(1.0f)
	, mLookSensitivity(0.005f)
	, mbDirty(false)
	, mbFreeLookEnabled(false)
{
	// glm calculates the projection matrix using vertical field of view, but I find horizontal more intuitive from a player's perspective
	float aspectRatio = (float)mWidth / (float)mHeight;
	float fovy = 2 * glm::degrees(atanf(tanf(glm::radians(fov) / 2) / aspectRatio));

	mProjectionMatrix = glm::perspective(glm::radians(fovy), aspectRatio, mNearDistance, mFarDistance);
}

Camera::~Camera()
{
}

void Camera::Update(float deltaTime)
{
	InputManager* pInputManager = InputManager::GetInstance();
	if (!pInputManager)
	{
		return;
	}

	// translate
	// --------------------------------------------------------------------------
	glm::vec3 movementDirection(0.0f);
	glm::vec3 planeLockedForward = glm::cross(WORLD_UP, mRight);
	float speed = mMovementSpeed;

	if (pInputManager->GetKeyState(Key::KEY_A) == KeyState::KS_DOWN)
	{
		movementDirection -= mRight;
	}
	if (pInputManager->GetKeyState(Key::KEY_D) == KeyState::KS_DOWN)
	{
		movementDirection += mRight;
	}
	if (pInputManager->GetKeyState(Key::KEY_W) == KeyState::KS_DOWN)
	{
		movementDirection += planeLockedForward;
	}
	if (pInputManager->GetKeyState(Key::KEY_S) == KeyState::KS_DOWN)
	{
		movementDirection -= planeLockedForward;
	}
	if (pInputManager->GetKeyState(Key::KEY_SPACE) == KeyState::KS_DOWN)
	{
		movementDirection += WORLD_UP;
	}
	if (pInputManager->GetKeyState(Key::KEY_LALT) == KeyState::KS_DOWN)
	{
		movementDirection -= WORLD_UP;
	}

	if (pInputManager->GetKeyState(Key::KEY_LSHIFT) == KeyState::KS_DOWN)
	{
		speed += 2.0f;
	}
	if (pInputManager->GetKeyState(Key::KEY_LCTRL) == KeyState::KS_DOWN)
	{
		speed -= 0.9f;
	}

	if (glm::length2(movementDirection) > 0.0f)
	{
		Translate(deltaTime * speed * glm::normalize(movementDirection));
	}

	// free look
	// --------------------------------------------------------------------------
	if (!mbFreeLookEnabled && pInputManager->GetMouseButtonState(MouseButton::MOUSE_BUTTON_MIDDLE) == MouseButtonState::MBS_DOWN)
	{
		pInputManager->DisableMouse();
		mbFreeLookEnabled = true;
		mLastFrameMousePos = pInputManager->GetMousePosition();
	}
	else if (mbFreeLookEnabled && pInputManager->GetMouseButtonState(MouseButton::MOUSE_BUTTON_MIDDLE) == MouseButtonState::MBS_UP)
	{
		pInputManager->EnableMouse();
		mbFreeLookEnabled = false;
	}

	if (mbFreeLookEnabled)
	{
		glm::vec2 mousePos = pInputManager->GetMousePosition();
		glm::vec2 delta = mousePos - mLastFrameMousePos;
		delta *= mLookSensitivity;

		mYaw += delta.x;
		mPitch -= delta.y;

		if (mPitch > MAX_PITCH)
			mPitch = MAX_PITCH;
		else if (mPitch < MIN_PITCH)
			mPitch = MIN_PITCH;

		glm::vec3 dir(0.0f);
		dir.x = glm::cos(mYaw) * glm::cos(mPitch);
		dir.y = glm::sin(mPitch);
		dir.z = glm::sin(mYaw) * glm::cos(mPitch);
		LookAt(mPosition + dir);

		mLastFrameMousePos = mousePos;
	}
}

void Camera::Translate(const glm::vec3& direction)
{
	SetPosition(mPosition + direction);
}

void Camera::LookAt(const glm::vec3& target)
{
	mForward = glm::normalize(target - mPosition);
	mRight = glm::cross(mForward, WORLD_UP);
	mUp = glm::cross(mRight, mForward);

	// recalculate yaw/pitch in case this is being called from outside of the normal freelook controls
	mYaw = glm::atan(mForward.z, mForward.x);
	mPitch = glm::asin(mForward.y);

	mbDirty = true;
}

void Camera::SetPosition(const glm::vec3& position)
{
	mPosition = position;
	mbDirty = true;
}

const glm::mat4& Camera::GetViewMatrix()
{
	if (mbDirty)
	{
		mViewMatrix = glm::lookAt(mPosition, mPosition + mForward, mUp);
		mbDirty = false;
	}

	return mViewMatrix;
}