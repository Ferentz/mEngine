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
		auto tile = gridmove.GetGrid()->GetTile(gridmove.GetPoint())->GetComponent<GameTile>();
		tile->TakeOccupancy(GetGameObject());
		dae::EventStack::GetEventStack().Register(*this, dae::make_sdbm_hash("ground dug"));
		collisionBox.m_signal.Register(*this);
	}

	GoldBag::~GoldBag()
	{
		dae::EventStack::GetEventStack().Unregister(this);
	}

	void GoldBag::Update(float)
	{
		if (auto newState = m_pState->ProcessState();
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
				dae::EventStack::GetEventStack().PushEvent(dae::Event(dae::make_sdbm_hash("gold_collected"), collider->GetCollisions()[0]));
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
		if (auto tileObj = m_gridMove->GetGrid()->GetTile(m_gridMove->GetPoint() - glm::ivec2(0, -1)))
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
	std::unique_ptr<GoldBag::BagState> GoldBag::StaticState::ProcessState()
	{
		return nullptr;
	}
	std::unique_ptr<GoldBag::BagState> GoldBag::StaticState::OnPush(dae::Collider* other)
	{
		// the goldbag can only be pushed from side to side;
		
		float direction_x = m_subject->GetGameObject()->m_transform.GetLocalTransform()->GetPosition().x - other->GetCollisions()[0]->m_transform.GetLocalTransform()->GetPosition().x;
		if (direction_x != 0)
		{
			direction_x /= fabs(direction_x);
		}
		glm::ivec2 direction{ direction_x , 0 };

		m_gridMove->MoveTo(direction);

		// the gold bags can always push eachother

		//auto newPoint = m_gridMove->GetPoint() + direction;
		//if (!m_gridMove->GetGrid()->IsPointValid(newPoint)) return;
		//auto tile = m_gridMove->GetGrid()->GetTile(newPoint)->GetComponent<GameTile>();
		//if ((m_moveDirection == dae::Direction::left || m_moveDirection == dae::Direction::right) || tile->CanMoveToTile())
		//{
		//	//release poit
		//	m_gridMove->GetGrid()->GetTile(m_gridMove->GetPoint())->GetComponent<GameTile>()->ReleaseOccupancy(m_gridMove->GetGameObject());
		//	m_gridMove->SetTarget(newPoint);
		//	//aquire point
		//	tile->TakeOccupancy(GetGameObject());
		//}


		return std::make_unique<PushState>(*m_subject, m_gridMove);
	}

#pragma endregion static_state
#pragma region push_state
	GoldBag::PushState::PushState(GoldBag& bag, dae::GridMove* gridmove)
		:BagState(bag, gridmove)
	{
		
	}

	std::unique_ptr<GoldBag::BagState> GoldBag::PushState::ProcessState()
	{
		if (m_gridMove->IsAtPoint())
			return nullptr;
		else
		{
			if (CanFall())
				return std::make_unique<FallState>(*m_subject, m_gridMove);
		} 
		return std::make_unique<StaticState>(*m_subject, m_gridMove);
	}
#pragma endregion push_state
#pragma region fall_state
	GoldBag::FallState::FallState(GoldBag& bag, dae::GridMove* gridmove)
		:BagState(bag, gridmove)
	{
	}
	std::unique_ptr<GoldBag::BagState> GoldBag::FallState::ProcessState()
	{
		if (!m_gridMove->IsAtPoint()) return nullptr;
		if (CanFall())
		{
			m_gridMove->MoveTo(glm::ivec2(0, 1));

			return nullptr;
		}

		m_subject->isTreasure = true;
		return std::make_unique<StaticState>(*m_subject, m_gridMove);
	}
	std::unique_ptr<GoldBag::BagState> GoldBag::FallState::OnPush(dae::Collider*)
	{
		// kill colided thing and self.
		return std::unique_ptr<GoldBag::BagState>();
	}
#pragma endregion fall_state


	

}
