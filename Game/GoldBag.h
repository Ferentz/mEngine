#pragma once
#include <State.h>
#include <components/GameComponent.h>
#include <eventSystem/Listener.h>

namespace dae
{
	class TextureComponent;
	class GridMove;
	class Collider;
}

namespace digger
{
	class GoldBag : public dae::GameComponent , public dae::Listener
	{
		GoldBag(const GoldBag& other) = delete;
		GoldBag(GoldBag&& other) = delete;
		GoldBag& operator=(const GoldBag& other) = delete;
		GoldBag& operator=(GoldBag&& other) = delete;

		class BagState : public State<GoldBag, BagState>
		{
		public:
			BagState(GoldBag& bag, dae::GridMove* gridmove)
				:State(bag)
				, m_gridMove{ gridmove }
			{}

			bool CanFall();

			virtual ~BagState() = default;
			virtual void OnEnter() override {};
			virtual void OnExit() override {};

			virtual std::unique_ptr<BagState> ProcessState() override = 0;// { return nullptr; }
			virtual std::unique_ptr<BagState> OnPush(dae::Collider* subject) = 0;
			dae::GridMove* m_gridMove;
		};

		class StaticState : public BagState
		{
		public:
			StaticState(GoldBag & bag, dae::GridMove* gridmove);
			virtual ~StaticState() = default;
			virtual void OnEnter() override{}
			virtual void OnExit() override {}
			virtual std::unique_ptr<BagState> ProcessState() override;
			virtual std::unique_ptr<BagState> OnPush(dae::Collider* subject) override;
		};

		class PushState : public BagState
		{
		public:
			PushState(GoldBag& bag, dae::GridMove* gridmove);
			virtual ~PushState() = default;
			virtual void OnEnter() override {}
			virtual void OnExit() override {}
			virtual std::unique_ptr<BagState> ProcessState() override;
			virtual std::unique_ptr<BagState> OnPush(dae::Collider*) override { return nullptr; }
		};

		class FallState : public BagState
		{
		public:
			FallState(GoldBag& bag, dae::GridMove* gridmove);
			virtual ~FallState() = default;
			virtual std::unique_ptr<BagState> ProcessState() override;
			virtual std::unique_ptr<BagState> OnPush(dae::Collider* subject) override;
		};

		std::unique_ptr<BagState> m_pState;
		dae::TextureComponent* m_rTexture;
		
		bool isTreasure{false};

	public:
		GoldBag(dae::GameObject& parent, dae::TextureComponent& texture, dae::GridMove& gridmove, dae::Collider& collisionBox);
		virtual ~GoldBag() override;

		virtual void Update(float) override;

		virtual void TuneIn(dae::EventId message, dae::GameObject* subject) override;
	};
}