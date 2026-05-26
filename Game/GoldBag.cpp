#include <GoldBag.h>

#include <GameObject.h>
#include <eventSystem/EventHash.h>
#include <eventSystem/EventStack.h>

namespace digger
{
	GoldBag::GoldBag(dae::GameObject& parent, dae::TextureComponent& texture)
		:dae::GameComponent(parent)
		, m_rTexture{&texture}
	{
		m_pState = std::make_unique<GoldBag::StaticState>(*this);
		dae::EventStack::GetEventStack().Register(*this, dae::make_sdbm_hash("dug ground"));
	}
	void GoldBag::Update(float)
	{
		if (auto newState = m_pState->ProcessState();
			newState != nullptr)
		{
			m_pState.reset(newState);
		}
	}

	void GoldBag::TuneIn(dae::EventId, dae::GameObject* )
	{
		//is only registered with ground was dug, we know that's what hapened.
		// we can chack if it was the ground below the bag
	}

#pragma region static_state
	GoldBag::StaticState::StaticState(GoldBag& bag)
		:BagState(bag)
	{
	}
	void GoldBag::StaticState::OnEnter()
	{
	}
	void GoldBag::StaticState::OnExit()
	{
	}

	GoldBag::BagState* GoldBag::StaticState::ProcessState()
	{
		return nullptr;
	}
#pragma endregion static_state
#pragma region fall_state
	GoldBag::FallState::FallState(GoldBag& bag)
		:BagState(bag)
	{
	}
	void GoldBag::FallState::OnEnter()
	{
	}
	void GoldBag::FallState::OnExit()
	{
	}
	GoldBag::BagState* GoldBag::FallState::ProcessState()
	{
		return nullptr;
	}
}
#pragma endregion fall_state