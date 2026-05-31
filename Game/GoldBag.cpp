#include "GoldBag.h"
#include "GameTile.h"

#include <tilegrid/GridMove.h>

#include <GameObject.h>
#include <eventSystem/EventHash.h>
#include <eventSystem/EventStack.h>
#include <collision/collider.h>

namespace digger
{
	
	GoldBag::GoldBag(dae::GameObject& parent, dae::TextureComponent& texture, dae::GridMove& gridmove, dae::Collider& collisionBox)
		:dae::GameComponent(parent)
		, m_rTexture{&texture}
		, m_pState{ std::make_unique<StaticState>(*this, &gridmove) }
	{
		auto tile = gridmove.GetGrid()->GetTile(gridmove.GetClosestPoint())->GetComponent<GameTile>();
		tile->TakeOccupancy(GetGameObject());
		dae::EventStack::GetEventStack().Register(*this, dae::make_sdbm_hash("ground dug"));
		collisionBox.m_signal.Register(*this);
	}

	GoldBag::~GoldBag()
	{
		dae::EventStack::GetEventStack().Unregister(this);
	}

	void GoldBag::Update(float delta)
	{
		if (auto newState = m_pState->ProcessState(delta);
			newState.get() != nullptr)
		{
			m_pState.swap(newState);
		}
	}

	void GoldBag::TuneIn(dae::EventId id, dae::GameObject* subject)
	{
		//is only registered with ground was dug, we know that's what hapened.
		// we can chack if it was the ground below the bag

		if (id == dae::make_sdbm_hash("ground dug"))
		{
			if (m_pState->CanFall())
				m_pState = std::make_unique<FallState>(*this, m_pState->m_gridMove);
		}
		else if (id == dae::make_sdbm_hash("collide"))
		{
			auto collider = subject->GetComponent<dae::Collider>();
			if (isTreasure)
			{
				dae::EventStack::GetEventStack().PushEvent(dae::Event(dae::make_sdbm_hash("gold_collected"), collider->GetCollisions()[0]->GetGameObject()));
				GetGameObject()->MarkForDelete();
				// if player collect coins
			}
			else
			{
				
				if (collider == nullptr) return;

				if (auto newState = m_pState->OnPush(collider);
					newState.get() != nullptr)
				{
					m_pState.swap(newState);
				}
			}
			
		}
	}
	
	bool GoldBag::BagState::CanFall()
	{
		if (auto tileObj = m_gridMove->GetGrid()->GetTile(m_gridMove->GetClosestPoint() - glm::ivec2(0, -1)))
		{
			if (auto tile = tileObj->GetComponent<GameTile>())
			{
				if (!tile->IsTraversed()) return false;

				return true;
			}
		}
		return false;
	}

#pragma region static_state
	GoldBag::StaticState::StaticState(GoldBag& bag, dae::GridMove* gridmove)
		:BagState(bag, gridmove)
	{
	}
	std::unique_ptr<GoldBag::BagState> GoldBag::StaticState::ProcessState(float)
	{
		return nullptr;
	}
	std::unique_ptr<GoldBag::BagState> GoldBag::StaticState::OnPush(dae::Collider*)
	{

		if (CanFall() && m_gridMove->Move(false, 1))
		{
			return std::make_unique<FallState>(*m_subject, m_gridMove);
		}
		return nullptr;

	}

#pragma endregion static_state

#pragma region fall_state
	GoldBag::FallState::FallState(GoldBag& bag, dae::GridMove* gridmove)
		:BagState(bag, gridmove)
	{
	}
	std::unique_ptr<GoldBag::BagState> GoldBag::FallState::ProcessState(float delta)
	{
		if(CanFall() && 
			m_gridMove->Move(false, 1))
		{
			fallTIme += delta;
			return nullptr;
		}
		else if(fallTIme > 1.f)
			m_subject->isTreasure = true;
		return std::make_unique<StaticState>(*m_subject, m_gridMove);
	}
	std::unique_ptr<GoldBag::BagState> GoldBag::FallState::OnPush(dae::Collider*)
	{
		if (fallTIme <= 1.f)
			return nullptr;
		// kill colided thing and self.
		return std::unique_ptr<GoldBag::StaticState>();
	}
#pragma endregion fall_state


	

}
