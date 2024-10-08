#include "Camera.h"

#include <cstdio>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/norm.hpp>

#include "Core/InputManager.h"

namespace
{
	const glm::vec3 WORLD_ORIGIN(0.0f);
	const glm::vec3 WORLD_RIGHT(1.0f, 0.0f, 0.0f);
	const glm::vec3 WORLD_UP(0.0f, 1.0f, 0.0f);
	const glm::vec3 WORLD_FORWARD(0.0f, 0.0f, 1.0f);
}

Camera::Camera(int width, int height, float fov, float near, float far)
	: mProjectionMatrix(1.0f)
	, mViewMatrix(glm::lookAt(WORLD_ORIGIN, WORLD_FORWARD, WORLD_UP))
	, mPosition(WORLD_ORIGIN)
	, mRight(WORLD_RIGHT)
	, mUp(WORLD_UP)
	, mForward(WORLD_FORWARD)
	, mWidth(width)
	, mHeight(height)
	, mFov(fov)
	, mNearDistance(near)
	, mFarDistance(far)
	, mMovementSpeed(1.0f)
	, mbDirty(false)
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
	const InputManager* const pInputManager = InputManager::GetInstance();
	if (!pInputManager)
	{
		return;
	}

	glm::vec3 movementDirection(0.0f);

	if (pInputManager->GetKeyState(Key::KEY_A))
	{
		movementDirection -= mRight;
	}
	if (pInputManager->GetKeyState(Key::KEY_D))
	{
		movementDirection += mRight;
	}
	if (pInputManager->GetKeyState(Key::KEY_W))
	{
		movementDirection += mForward;
	}
	if (pInputManager->GetKeyState(Key::KEY_S))
	{
		movementDirection -= mForward;
	}

	if (glm::length2(movementDirection) > 0.0f)
	{
		Translate(deltaTime * mMovementSpeed * glm::normalize(movementDirection));
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