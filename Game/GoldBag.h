#pragma once
#include <State.h>
#include <components/GameComponent.h>
#include <eventSystem/Listener.h>

namespace dae
{
	class TextureComponent;
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
			BagState(GoldBag& bag)
				:State(bag)
			{}
			virtual ~BagState() = default;
			virtual void OnEnter() override  = 0;
			virtual void OnExit() override  = 0;

			virtual BagState* ProcessState() override = 0;// { return nullptr; }
		};
		class StaticState : public BagState
		{
		public:
			StaticState(GoldBag &);
			virtual void OnEnter() override;
			virtual void OnExit() override;
			virtual BagState* ProcessState() override;
		};
		class FallState : public BagState
		{
		public:
			FallState(GoldBag&);
			virtual void OnEnter() override;
			virtual void OnExit() override;
			virtual BagState* ProcessState() override;
		};

		std::unique_ptr<BagState> m_pState;
		dae::TextureComponent* m_rTexture;

	public:
		GoldBag(dae::GameObject& parent, dae::TextureComponent& texture);
		virtual ~GoldBag() override = default;

		virtual void Update(float) override;

		virtual void TuneIn(dae::EventId message, dae::GameObject* subject) override;
	};
}