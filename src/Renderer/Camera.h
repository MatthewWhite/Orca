#ifndef CAMERA_H
#define CAMERA_H

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

class Camera
{
public:
	Camera(int width, int height, float fov, float near, float far);
	~Camera();

	void Translate(const glm::vec3& direction);
	void LookAt(const glm::vec3& target);

	const glm::mat4& GetProjectionMatrix() const { return mProjectionMatrix; }
	const glm::mat4& GetViewMatrix();
	const glm::vec3& GetPosition() const { return mPosition; }
	void SetPosition(const glm::vec3& pos);
	const glm::vec3& GetRight() const { return mRight; }
	const glm::vec3& GetUp() const { return mUp; }
	const glm::vec3& GetForward() const { return mForward; }
	int GetWidth() const { return mWidth; }
	int GetHeight() const { return mHeight; }
	float GetFov() const { return mFov; }
	float GetNearPlaneDistance() const { return mNearDistance; }
	float GetFarPlaneDistance() const { return mFarDistance; }

private:
	glm::mat4 mProjectionMatrix;
	glm::mat4 mViewMatrix;

	glm::vec3 mPosition;
	glm::vec3 mRight;
	glm::vec3 mUp;
	glm::vec3 mForward;

	int mWidth;
	int mHeight;
	float mFov;
	float mNearDistance;
	float mFarDistance;

	bool mbDirty;
};

#endif