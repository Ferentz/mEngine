#include <iostream>


#include "GameComponent.h"
#include "GameObject.h"




namespace dae
{
	
	GameComponent::GameComponent(GameObject& parent)
		:m_pGameObject{&parent}
	{
	}

	void GameComponent::SetParent(GameObject& parent)
	{
		m_pGameObject = &parent;
	}
	

	GameObject* GameComponent::GetGameObject()
	{
		return m_pGameObject;
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
		SmartTransform* transform{ GetGameObject()->GetTransform() };
		transform->SetLocalRotation(transform->GetRotation() + m_rotatingSpeed * deltaTime);
		//transform->MakeDirty();
		GetGameObject()->MakeDirty();
	}

	void RotatorComponent::SetRotationSpeed(float speed)
	{
		m_rotatingSpeed = speed;
	}


	
	

}

