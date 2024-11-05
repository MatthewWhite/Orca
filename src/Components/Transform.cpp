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

glm::vec3 Transform::GetPosition() const
{
	glm::vec3 pos = m_localPosition;
	if (m_parent)
	{
		pos += m_parent->GetPosition();
	}

	return pos;
}

void Transform::SetPosition(const glm::vec3& pos)
{
	m_localPosition = pos;
	if (m_parent)
	{
		m_localPosition -= m_parent->GetPosition();
	}

	m_dirty = true;
}

void Transform::SetPosition(float x, float y, float z)
{
	SetPosition(glm::vec3(x, y, z));
}

glm::vec3 Transform::GetScale() const
{
	glm::vec3 scale = m_localScale;
	if (m_parent)
	{
		scale *= m_parent->GetScale();
	}

	return scale;
}

void Transform::SetScale(const glm::vec3& scale)
{
	m_localScale = scale;
	if (m_parent)
	{
		m_localScale /= m_parent->GetScale();
	}

	m_dirty = true;
}

void Transform::SetScale(float x, float y, float z)
{
	SetScale(glm::vec3(x, y, z));
}

void Transform::SetScale(float scale)
{
	SetScale(glm::vec3(scale));
}

glm::quat Transform::GetRotation() const
{
	glm::quat rotation = m_localRotation;
	if (m_parent)
	{
		rotation *= m_parent->GetRotation();
	}

	return rotation;
}

glm::vec3 Transform::GetRotationAsEulerAngles() const
{
	return glm::degrees(glm::eulerAngles(GetRotation()));
}

void Transform::SetRotationFromEulerAngles(const glm::vec3& eulerAngles)
{
	m_localRotation = glm::quat(glm::radians(eulerAngles));
	if (m_parent)
	{
		m_localRotation = m_localRotation * glm::inverse(m_parent->GetRotation());
	}

	m_dirty = true;
}

void Transform::SetRotationFromEulerAngles(float x, float y, float z)
{
	SetRotationFromEulerAngles(glm::vec3(x, y, z));
}

glm::mat4 Transform::GetWorldMatrix()
{
	if (m_dirty)
	{
		glm::mat4 scale = glm::scale(glm::mat4(1.0f), m_localScale);
		glm::mat4 rotate = glm::mat4_cast(m_localRotation);
		glm::mat4 translate = glm::translate(glm::mat4(1.0f), m_localPosition);

		m_worldMatrix = translate * rotate * scale;

		m_dirty = false;
	}

	if (m_parent)
	{
		return m_parent->GetWorldMatrix() * m_worldMatrix;
	}

	return m_worldMatrix;
}