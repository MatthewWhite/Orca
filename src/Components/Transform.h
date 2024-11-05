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

	glm::vec3 GetLocalPosition() const { return m_localTranslation; }
	void SetLocalPosition(const glm::vec3& pos) { m_localTranslation = pos; m_dirty = true; }
	void SetLocalPosition(float x, float y, float z) { SetLocalPosition(glm::vec3(x, y, z)); }

	glm::vec3 GetLocalScale() const { return m_localScale; }
	void SetLocalScale(const glm::vec3& scale) { m_localScale = scale; m_dirty = true; }
	void SetLocalScale(float scale) { SetLocalScale(glm::vec3(scale)); }
	void SetLocalScale(float x, float y, float z) { SetLocalScale(glm::vec3(x, y, z)); }

	// Returns the euler angle representation of the local rotation, in degrees
	glm::vec3 GetLocalRotationAsEulerAngles() const;
	// Sets the local rotation to the provided euler angles, in degrees
	void SetLocalRotationFromEulerAngles(const glm::vec3& eulerAngles);
	// Sets the local rotation to the provided euler angles, in degrees
	void SetLocalRotationFromEulerAngles(float x, float y, float z) { SetLocalRotationFromEulerAngles(glm::vec3(x, y, z)); }

	glm::mat4 GetWorldMatrix();

	DECLARE_COMPONENT_TYPE_ID(Transform);

private:
	glm::mat4 m_worldMatrix = glm::mat4(1.0);

	glm::quat m_localRotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
	glm::vec3 m_localTranslation = glm::vec3(0.0f);
	glm::vec3 m_localScale = glm::vec3(1.0f);

	bool m_dirty = false;
};

#endif