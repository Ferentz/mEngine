#include <GoldBag.h>

#include <GameObject.h>

namespace digger
{
	GoldBag::GoldBag(dae::GameObject& parent, dae::TextureComponent& texture)
		:dae::GameComponent(parent)
		, m_rTexture{&texture}
	{
		m_pState = std::make_unique<GoldBag::BagState>(*this);
	}
	void GoldBag::Update(float)
	{
		if (auto newState = m_pState->ProcessState();
			newState != nullptr)
		{
			m_pState.reset(newState);
		}
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