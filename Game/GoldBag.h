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
	public:
		GoldBag(dae::GameObject& parent, dae::TextureComponent& texture, dae::GridMove& gridmove, dae::Collider& collisionBox, std::string const& goldTex);
		virtual ~GoldBag() override;

		virtual void Update(float) override;

		virtual void TuneIn(dae::EventId message, dae::GameObject* subject) override;

	private:
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

			virtual std::unique_ptr<BagState> ProcessState() override { return nullptr; }
			virtual std::unique_ptr<BagState> Update(float) = 0;
			virtual std::unique_ptr<BagState> OnPush(dae::Collider* subject) = 0;
			virtual std::unique_ptr<BagState> OnDug() { return nullptr; };
			dae::GridMove* m_gridMove;
		};

		class StaticState : public BagState
		{
		public:
			StaticState(GoldBag & bag, dae::GridMove* gridmove);
			virtual ~StaticState() = default;
			virtual void OnEnter() override{}
			virtual void OnExit() override {}
			virtual std::unique_ptr<BagState> Update(float) override;
			virtual std::unique_ptr<BagState> OnPush(dae::Collider* subject) override;
			virtual std::unique_ptr<BagState> OnDug() override;
		};

		class FallState : public BagState
		{
		public:
			FallState(GoldBag& bag, dae::GridMove* gridmove);
			virtual ~FallState() = default;
			virtual std::unique_ptr<BagState> Update(float) override;
			virtual std::unique_ptr<BagState> OnPush(dae::Collider* subject) override;
			glm::ivec2 m_originPoint{};
		};

		class GoingToFallState : public BagState
		{
			float timer{};
			float const maxTime{ 2.f };
		public:
			GoingToFallState(GoldBag& bag, dae::GridMove* gridmove);
			virtual ~GoingToFallState() = default;
			virtual std::unique_ptr<BagState> Update(float) override;
			virtual std::unique_ptr<BagState> OnPush(dae::Collider* subject) override;
		};

		std::unique_ptr<BagState> m_pState;
		dae::TextureComponent* m_rTexture;
		std::string const goldTexture;
		bool isTreasure{false};

	};
}