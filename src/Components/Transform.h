#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "Component.h"

class Transform : public Component
{
public:
	Transform();
	~Transform();

	Transform* GetParent() const { return m_parent; }
	void SetParent(Transform* parent) { m_parent = parent; }

#pragma region local_transforms
	glm::vec3 GetLocalPosition() const { return m_localPosition; }
	void SetLocalPosition(const glm::vec3& pos) { m_localPosition = pos; m_dirty = true; }
	void SetLocalPosition(float x, float y, float z) { SetLocalPosition(glm::vec3(x, y, z)); }

	glm::vec3 GetLocalScale() const { return m_localScale; }
	void SetLocalScale(const glm::vec3& scale) { m_localScale = scale; m_dirty = true; }
	void SetLocalScale(float scale) { SetLocalScale(glm::vec3(scale)); }
	void SetLocalScale(float x, float y, float z) { SetLocalScale(glm::vec3(x, y, z)); }

	glm::quat GetLocalRotation() const { return m_localRotation; }
	// Returns the euler angle representation of the local rotation, in degrees
	glm::vec3 GetLocalRotationAsEulerAngles() const;
	// Sets the local rotation to the provided euler angles, in degrees
	void SetLocalRotationFromEulerAngles(const glm::vec3& eulerAngles);
	// Sets the local rotation to the provided euler angles, in degrees
	void SetLocalRotationFromEulerAngles(float x, float y, float z) { SetLocalRotationFromEulerAngles(glm::vec3(x, y, z)); }
#pragma endregion

#pragma region world_transforms
	glm::vec3 GetPosition() const;
	void SetPosition(const glm::vec3& pos);
	void SetPosition(float x, float y, float z);

	glm::vec3 GetScale() const;
	void SetScale(const glm::vec3& scale);
	void SetScale(float x, float y, float z);
	void SetScale(float scale);

	glm::quat GetRotation() const;
	glm::vec3 GetRotationAsEulerAngles() const;
	void SetRotationFromEulerAngles(const glm::vec3& eulerAngles);
	void SetRotationFromEulerAngles(float x, float y, float z);
#pragma endregion

	glm::mat4 GetWorldMatrix();

	DECLARE_COMPONENT_TYPE_ID(Transform);

private:
	Transform* m_parent = nullptr;

	glm::mat4 m_worldMatrix = glm::mat4(1.0);

	glm::quat m_localRotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
	glm::vec3 m_localPosition = glm::vec3(0.0f);
	glm::vec3 m_localScale = glm::vec3(1.0f);

	bool m_dirty = false;
};

#endif