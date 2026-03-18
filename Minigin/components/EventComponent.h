#pragma once

//#include <type_traits>

#include "components/RenderComponent.h"
#include "eventSystem/BroadCaster.h"
#include "eventSystem/Listener.h"

#include <string>

namespace dae
{
	class IntTracker final : public GameComponent
	{
	public:
		IntTracker(GameObject& parent,int startValue, int maxValue = 3, int minValue = 0)
			:GameComponent(parent),
			m_value{ startValue },
			m_minValue{ minValue },
			m_maxValue{maxValue}
		{
		}

		int GetValue() const { return m_value; }
		int SubtractValue(int val);
		int AddValue(int val);

		BroadCaster m_signal{};

	private:

		int m_value;
		int m_minValue;
		int m_maxValue;
	};

	class HealthDisplay final : public TextComponent
	{
	public:
		HealthDisplay(GameObject& parent, const std::string& text,  std::shared_ptr<Font> font, IntTracker& healthComponent);
		void SetLife();
		Listener m_listener;
	private:
		const std::string m_base;
		IntTracker* m_healthComponent;
	};



	/*template<
		typename T,
		typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type
	>
	class NumberTrackerComponent final  : public GameComponent
	{
	public:
		virtual ~NumberTrackerComponent() override = default;
		explicit NumberTrackerComponent(GameObject& parent, T startValue, T minValue = 0, T maxValue = startValue);

		T GetValue() const;
		T GetMinValue() const;
		T GetMaxValue() const;
		void SetValue();
		void SetMaxValue();
		void SetMinValue();

	private:
		BroadCaster m_signal{};
		T m_value;
		T m_minValue;
		T m_maxValue;
		NumberTrackerComponent(const NumberTrackerComponent& other) = delete;
		NumberTrackerComponent(NumberTrackerComponent&& other) = delete;
		NumberTrackerComponent& operator=(const NumberTrackerComponent& other) = delete;
		NumberTrackerComponent& operator=(NumberTrackerComponent&& other) = delete;
	};*/
}