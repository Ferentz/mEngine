#pragma once
#include <string>
#include <memory>

//#include "GameObject.h"


#include <SDL3/SDL_pixels.h>
#include <glm/glm.hpp>
#include <chrono>

namespace dae
{
	

	class GameObject;
	class Texture2D;
	class Font;
	
	class GameComponent
	{
	public:
		virtual ~GameComponent() = default;
		explicit GameComponent(GameObject& parent);

		void SetParent(GameObject& parent);
		virtual void Update(float) {};


	protected:
		explicit GameComponent(GameObject* parent)
			:m_pParent{parent}
		{}
		GameObject* GetParent() const;
	private:
		GameObject* m_pParent;
		
		
		GameComponent(const GameComponent& other) = delete;
		GameComponent(GameComponent&& other) = delete;
		GameComponent& operator=(const GameComponent& other) = delete;
		GameComponent& operator=(GameComponent&& other) = delete;
	};


	class derivedComponent final : public GameComponent
	{
	public:
		virtual ~derivedComponent() override = default;
		explicit derivedComponent(GameObject& parent, float val);


	protected: // i tried something, it didnt work. i need fps
		float m_val{};
		
		derivedComponent(const derivedComponent& other) = delete;
		derivedComponent(derivedComponent&& other) = delete;
		derivedComponent& operator=(const derivedComponent& other) = delete;
		derivedComponent& operator=(derivedComponent&& other) = delete;
	};

	class RotatorComponent final : public GameComponent
	{
	public:
		virtual ~RotatorComponent() override = default;
		explicit RotatorComponent(GameObject& parent);

		virtual void Update(float) override;

		void SetRotationSpeed(float speed);

	protected: // i tried something, it didnt work. i need fps
		float m_rotatingSpeed{ 1.f };

		RotatorComponent(const RotatorComponent& other) = delete;
		RotatorComponent(RotatorComponent&& other) = delete;
		RotatorComponent& operator=(const RotatorComponent& other) = delete;
		RotatorComponent& operator=(RotatorComponent&& other) = delete;
	};

	
}