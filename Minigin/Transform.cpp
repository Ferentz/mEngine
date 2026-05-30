#include "Transform.h"
#include "Transform.h"
#include "Transform.h"
#include "Transform.h"
#include "Transform.h"
#include "Transform.h"
#include "Transform.h"

#include "GameObject.h"
#include "components/RenderComponent.h"

void dae::Transform::SetPosition(const float x, const float y)
{
	m_position.x = x;
	m_position.y = y;
}

void dae::Transform::SetPosition(const glm::vec2& position) 
{ 
	m_position = position; 
}

void dae::Transform::Translate(float x, float y)
{
	m_position.x += x;
	m_position.y += y;
}

void dae::Transform::SetRotation(float x)
{
	m_rotation_radians = x;
}

void dae::Transform::Rotate(float degrees)
{
	m_rotation_radians += glm::radians(degrees);
}


dae::SmartTransform::SmartTransform(GameObject* owner)
	: m_pGameObject{owner}
{
}

void dae::SmartTransform::SetLocalTransform(Transform& newTransform)
{
	MakeDirty();
	m_local = newTransform;
}

void dae::SmartTransform::SetLocalPosition(float x, float y)
{
	MakeDirty();
	m_local.SetPosition(x, y);
}

void dae::SmartTransform::SetLocalRotation(float x)
{
	MakeDirty();
	m_local.SetRotation(x);
}

//void dae::SmartTransform::SetGlobalTransform(Transform& )
//{
//}
//
//void dae::SmartTransform::SetGlobalPosition(float , float , float )
//{
//}
//
//void dae::SmartTransform::SetGlobalRotation(float )
//{
//}

void dae::SmartTransform::Translate(float x, float y)
{
	MakeDirty();
	m_local.Translate(x, y);
}


dae::Transform const * dae::SmartTransform::GetLocalTransform() const
{
	return &m_local;
}

dae::Transform const* dae::SmartTransform::GetWorldTransform()
{
	

	//Transform const * world{ GetWorldTransform() };
	if (m_dirty)
	{
		if (m_pGameObject->GetParent() == nullptr)
		{
			m_global = m_local;
		}
		else
		{
			m_global = *m_pGameObject->GetParent()->GetWorldTransform() + m_local; 
		}
		m_dirty = false;
	}
	return &m_global;
}

void dae::SmartTransform::ReconstructLocal(GameObject * newBase)
{
	m_local = (newBase == nullptr)? m_global : m_global.MissingLinkWith( *newBase->GetWorldTransform());
}

void dae::SmartTransform::MakeDirty()
{
	m_dirty = true;
}
