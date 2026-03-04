#pragma once
#include <string>
#include <memory>

//#include "GameObject.h"
#include "Transform.h"

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

	class RenderComponent : public GameComponent
	{
	public:
		virtual ~RenderComponent() = default;
		explicit RenderComponent(GameObject& parent)
			:GameComponent{ parent },
			m_transform{}
		{
		}

		RenderComponent(const RenderComponent& other) = delete;
		RenderComponent(RenderComponent&& other) = delete;
		RenderComponent& operator=(const RenderComponent& other) = delete;
		RenderComponent& operator=(RenderComponent&& other) = delete;
		
		virtual void Render() = 0;
		virtual void Update(float) override;
		void MakeDirty();

		void SetPosition(float x, float y);
		Transform const* GetLocalTransform() const;
		Transform const * QueryWorldTransform();
		Transform const* GetWorldTransform() const;

	private:
		SmartTransform m_transform;

	};

	class UIComponent : public RenderComponent
	{
	public:
		virtual ~UIComponent() override = default;
		explicit UIComponent(GameObject& parent, char const* name);

		UIComponent(const UIComponent& other) = delete;
		UIComponent(UIComponent&& other) = delete;
		UIComponent& operator=(const UIComponent& other) = delete;
		UIComponent& operator=(UIComponent&& other) = delete;

		virtual void Update(float) override;
		virtual void Render() override;

	protected:
		char const* m_name;
		bool m_run_trash1{ false };
		bool m_drawPlot1{ false };

		int m_bufferSize{ 10000 };
		int m_tries{ 5 };
		static int const m_timingsAmount{ 10 };
		std::vector<float> m_timings{ };

		void RunTrash(std::vector<float>& toFill);
		/*void RunTrash1();
		void RunTrash2();*/

		template<class T>
		void RunTrash(std::vector<float>& toFill)
		{
			std::vector<T> buffer(m_bufferSize);
			auto start = std::chrono::high_resolution_clock::now();

			toFill.resize(m_timingsAmount);
			std::fill(toFill.begin(), toFill.end(), 0.f);

			for (int trie{}; trie < m_tries; trie++)
			{
				int step{};
				for (int stepsize{ 1 }; stepsize <= 1024; stepsize *= 2)
				{
					for (int i{}; i < buffer.size(); i += stepsize)
					{
						buffer[i].id *= 2;
					}
					const auto current_time{ std::chrono::high_resolution_clock::now() };
					float m_deltaTime = std::chrono::duration<float>(current_time - start).count();
					//std::cout << m_deltaTime << "\n";

					toFill[step] += m_deltaTime;
					step++;
					if (step >= toFill.size())
					{
						break;
					}
					start = current_time;
				}
			}

			for (float& val : toFill)
			{
				val /= m_tries;
			}
		}
	};

	class UIComponent_2 final : public UIComponent
	{
	public:
		virtual ~UIComponent_2() override = default;
		explicit UIComponent_2(GameObject& parent, char const* name);

		UIComponent_2(const UIComponent_2& other) = delete;
		UIComponent_2(UIComponent_2&& other) = delete;
		UIComponent_2& operator=(const UIComponent_2& other) = delete;
		UIComponent_2& operator=(UIComponent_2&& other) = delete;
		
		virtual void Update(float) override;
		virtual void Render() override;

	private:
		bool m_run_trash2{ false };
		bool m_drawPlot2{ false };

		std::vector<float> m_timings2{ };
	};

	class TextureComponent : public RenderComponent
	{
	public:
		virtual ~TextureComponent() override = default;
		explicit TextureComponent(GameObject& parent);

		TextureComponent(const TextureComponent& other) = delete;
		TextureComponent(TextureComponent&& other) = delete;
		TextureComponent& operator=(const TextureComponent& other) = delete;
		TextureComponent& operator=(TextureComponent&& other) = delete;

		virtual void Render() override;
		//virtual void Render()const override {}
		void SetTexture(const std::string& filename);

	protected:
		std::shared_ptr<Texture2D> m_texture{};
	private:
		
	};

	class TextComponent : public TextureComponent
	{
	public:
		virtual ~TextComponent() override = default;

		explicit TextComponent(GameObject& parent,
			const std::string& text, std::shared_ptr<Font> font, const SDL_Color& color = { 255, 255, 255, 255 });

		virtual void Update(float) override;

		void SetText(const std::string& text);
		void SetColor(const SDL_Color& color);

	protected:

	private:
		bool m_needsUpdate{};
		std::string m_text{};
		SDL_Color m_color{ 255, 255, 255, 255 };
		std::shared_ptr<Font> m_font{};
	};

	class FPSComponent final : public TextComponent
	{
	public:
		virtual ~FPSComponent() override = default;

		explicit FPSComponent(GameObject& parent,
			const std::string& text, std::shared_ptr<Font> font, const SDL_Color& color = { 255, 255, 255, 255 });

		virtual void Update(float) override;

	protected:

	private:
		float m_accTime{};
		int m_frameCount{};
		float m_prevDelta{};
	};

}