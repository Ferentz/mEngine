#include <iostream>


#include "GameComponent.h"
#include "GameObject.h"




namespace dae
{
	
	GameComponent::GameComponent(GameObject& parent)
		:m_pParent{&parent}
	{
	}

	void GameComponent::SetParent(GameObject& parent)
	{
		m_pParent = &parent;
	}
	

	GameObject* GameComponent::GetParent() const
	{
		return m_pParent;
	}

	derivedComponent::derivedComponent(GameObject& parent, float val)
		:GameComponent{ parent },
		m_val{val}
	{
	}

	RotatorComponent::RotatorComponent(GameObject& parent)
		:GameComponent{ parent }
	{
	}

	void RotatorComponent::Update(float deltaTime)
	{
		SmartTransform* transform{ GetParent()->GetTransform() };
		transform->SetRotation(transform->GetRotation() + m_rotatingSpeed * deltaTime);
		//transform->MakeDirty();
		GetParent()->MakeDirty();
	}

	void RotatorComponent::SetRotationSpeed(float speed)
	{
		m_rotatingSpeed = speed;
	}


	
	

}

