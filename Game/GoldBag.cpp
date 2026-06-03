#include "GoldBag.h"
#include "GameTile.h"
//#include "Digger.h"
#include "Entity.h"
#include <tileGrid/GridMove.h>

#include <GameObject.h>
#include <eventSystem/EventHash.h>
#include <eventSystem/EventStack.h>
#include <collision/collider.h>
#include <components/RenderComponent.h>

namespace digger
{
	
	GoldBag::GoldBag(dae::GameObject& parent, dae::TextureComponent& texture, dae::GridMove& gridmove, dae::Collider& collisionBox, std::string const & goldTex)
		:dae::GameComponent(parent)
		, m_rTexture{&texture}
		, m_pState{ std::make_unique<StaticState>(*this, &gridmove) }
		, goldTexture{goldTex}
	{
		dae::EventStack::GetEventStack().Register(*this, dae::make_sdbm_hash("ground dug"));
		collisionBox.m_signal.Register(*this);
	}

	GoldBag::~GoldBag()
	{
		dae::EventStack::GetEventStack().Unregister(this);
	}

	void GoldBag::Update(float delta)
	{
		if (auto newState = m_pState->Update(delta);
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
			if (auto newState = m_pState->OnDug();
				newState.get() != nullptr)
			{
				m_pState.swap(newState);
			}
		}
		else 
			if (id == dae::make_sdbm_hash("collide"))
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
		auto closest{ m_gridMove->GetGrid()->GetGridLocationOfPoint(m_gridMove->GetClosestPoint())};
		auto pos{ m_gridMove->GetGameObject()->GetLocalTransform()->GetPosition() };
		glm::ivec2 point{closest.y > pos.y ? m_gridMove->GetClosestPoint() : m_gridMove->GetClosestPoint() - glm::ivec2(0, -1) };
		if (auto tileObj = m_gridMove->GetGrid()->GetTile(point))
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
	std::unique_ptr<GoldBag::BagState> GoldBag::StaticState::Update(float)
	{
		//if (CanFall()) // only check can fall if he entered from block
		//{
		//	return std::make_unique<GoingToFallState>(*m_subject, m_gridMove);
		//}
		return nullptr;
	}
	std::unique_ptr<GoldBag::BagState> GoldBag::StaticState::OnPush(dae::Collider*)
	{

		if (CanFall() && 
			m_gridMove->Move(false, 1))
		{
			return std::make_unique<GoingToFallState>(*m_subject, m_gridMove);
		}
		return nullptr;

	}

	std::unique_ptr<GoldBag::BagState> GoldBag::StaticState::OnDug()
	{
		if (CanFall())
			return std::make_unique<GoingToFallState>(*m_subject, m_gridMove);
		return nullptr;
	}

#pragma endregion static_state

#pragma region fall_state
	GoldBag::FallState::FallState(GoldBag& bag, dae::GridMove* gridmove)
		:BagState(bag, gridmove)
	{
		m_originPoint = gridmove->GetClosestPoint();
	}
	std::unique_ptr<GoldBag::BagState> GoldBag::FallState::Update(float)
	{
		if (CanFall())
		{
			if (m_gridMove->Move(false, 1))
			{
				return nullptr;
			}
			else
			{
				m_gridMove->Move(false, -1);
				return nullptr;
			}
		}

		if ((m_gridMove->GetClosestPoint().y - m_originPoint.y) >= 2)
		{
			m_subject->m_rTexture->SetTexture(m_subject->goldTexture);
			m_subject->isTreasure = true;
		}
		return std::make_unique<StaticState>(*m_subject, m_gridMove);
	}
	std::unique_ptr<GoldBag::BagState> GoldBag::FallState::OnPush(dae::Collider *collider)
	{
		//kill
		for (auto collision : collider->GetCollisions())
		{
			if (auto entity = collision->GetGameObject()->GetComponent<Entity>())
			{
				entity->Die();
			}
		}
		return nullptr;
	}
#pragma endregion fall_state


	

	GoldBag::GoingToFallState::GoingToFallState(GoldBag& bag, dae::GridMove* gridmove)
		:BagState(bag, gridmove)
	{
	}

	std::unique_ptr<GoldBag::BagState> GoldBag::GoingToFallState::Update(float delta)
	{
		timer += delta;
		if (timer >= maxTime)
		{
			return std::make_unique<GoldBag::FallState>(*m_subject, m_gridMove);
		}
		return nullptr;
	}

	std::unique_ptr<GoldBag::BagState> GoldBag::GoingToFallState::OnPush(dae::Collider* collider)
	{
		auto & collisions{ collider->GetCollisions() };
		for (auto collision : collisions)
		{
			if (collision->GetGameObject()->GetComponent<digger::Entity>())
			{
				timer = 0;
				return nullptr;
			}
		}
		return nullptr;
	}

}
