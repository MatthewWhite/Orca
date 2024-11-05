#include "Transform.h"

#include <glm/gtx/quaternion.hpp>

Transform::Transform()
{
}

Transform::~Transform()
{
}

glm::vec3 Transform::GetLocalRotationAsEulerAngles() const
{
	return glm::degrees(glm::eulerAngles(m_localRotation));
}

void Transform::SetLocalRotationFromEulerAngles(const glm::vec3& eulerAngles)
{
	m_localRotation = glm::quat(glm::radians(eulerAngles));
	m_dirty = true;
}

glm::mat4 Transform::GetWorldMatrix()
{
	if (m_dirty)
	{
		glm::mat4 scale = glm::scale(glm::mat4(1.0f), m_localScale);
		glm::mat4 rotate = glm::mat4_cast(m_localRotation);
		glm::mat4 translate = glm::translate(glm::mat4(1.0f), m_localTranslation);

		m_worldMatrix = translate * rotate * scale;

		m_dirty = false;
	}

	return m_worldMatrix;
}