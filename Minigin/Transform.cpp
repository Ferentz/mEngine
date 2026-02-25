#include "Transform.h"

void dae::Transform::SetPosition(const float x, const float y, const float z)
{
	m_position.x = x;
	m_position.y = y;
	m_position.z = z;
}

void dae::Transform::SetPosition(const glm::vec3& position) 
{ 
	m_position = position; 
}

void dae::Transform::SetRotation(float x)
{
	m_rotation_radians = x;
}


void dae::SmartTransform::SetTransform(Transform& newTransform)
{
	m_dirty = true;
	m_local = newTransform;
}

void dae::SmartTransform::SetPosition(float x, float y, float z)
{
	m_dirty = true;
	m_local.SetPosition(x, y, z);
}

void dae::SmartTransform::SetRotation(float x)
{
	m_dirty = true;
	m_local.SetRotation(x);
}


dae::Transform const * dae::SmartTransform::GetLocalTransform() const
{
	return &m_local;
}

dae::Transform const* dae::SmartTransform::QueryWorldTransform(Transform const * const parentWorldTransform)
{
	if (parentWorldTransform == nullptr)
	{
		if (m_dirty)
		{
			m_dirty = false;
			m_global = m_local;
		}
		return GetLocalTransform();
	}

	//Transform const * world{ GetWorldTransform() };
	if (m_dirty)
	{
		m_global = m_local + *parentWorldTransform;
		m_dirty = false;
	}
	return &m_global;
}

dae::Transform const * dae::SmartTransform::GetWorldTransform() const
{
	//if (m_dirty) return nullptr;
	return &m_global;
}

void dae::SmartTransform::Rebase(Transform const * newBase)
{
	float rotation = m_local.GetRotation();
	m_local = m_global - *newBase;
	m_local.SetRotation(rotation);
}

void dae::SmartTransform::MakeDirty()
{
	m_dirty = true;
}
