#pragma once
#include "RenderComponent.h"


namespace dae
{
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

		//void RunTrash(std::vector<float>& toFill);
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
				size_t step{};
				for (size_t stepsize{ 1 }; stepsize <= 1024; stepsize *= 2)
				{
					for (size_t i{}; i < buffer.size(); i += stepsize)
					{
						if constexpr (requires{T::id; })
						{
							buffer[i].id *= 2;
						}
						else
						{
							buffer[i] *= 2;
						}


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
}