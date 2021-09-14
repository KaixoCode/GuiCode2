#include "GuiCode2/StateColors.hpp"

namespace GuiCode
{
	StateColors::StateColors(const Settings& settings)
		: m_Link(settings.link), transition(settings.transition), base(settings.base)
	{
		for (auto& [state, color] : settings.colors)
		{
			m_States.push_front(state);
			m_ColorMap[state] = color;
		}
	}

	StateColors& StateColors::operator=(const StateColors& other)
	{
		base = other.base;
		m_States = other.m_States;
		m_ColorMap = other.m_ColorMap;
		return *this;
	}

	StateColors& StateColors::operator=(StateColors&& other)
	{
		base = other.base;
		m_States = other.m_States;
		m_ColorMap = other.m_ColorMap;
		return *this;
	}

	void StateColors::Clear()
	{
		m_States.clear();
		m_ColorMap.clear();
	}

	void StateColors::State(int state, Color color)
	{
		if (!m_ColorMap.contains(state))
		{
			m_States.push_back(state);
			m_ColorMap.emplace(state, color);
		}
		else
			m_ColorMap[state] = color;
	}

	void StateColors::Remove(int state)
	{
		if (m_ColorMap.contains(state))
		{
			if (m_Goal == state)
				m_Goal = NoValue;

			m_States.remove(state);
			m_ColorMap.erase(state);
		}
	}

	Color StateColors::Current() const
	{
		if (!m_Link)
			return base;

		// Get current state
		int _state = NoValue;
		for (auto& i : m_States)
			if (m_Link->State(i))
			{
				_state = i;
				break;
			}

		// Calculate lerp percent
		auto _now = std::chrono::steady_clock::now();
		auto _duration = std::chrono::duration_cast<std::chrono::milliseconds>(_now - m_ChangeTime).count();
		float _percent = constrain(_duration / transition, 0, 1);

		// Get the color
		const Color& c = m_Goal == NoValue ? base : m_ColorMap.at(m_Goal);

		// If change
		if (_state != m_Goal)
		{
			m_Goal = _state; // set new goal
			m_Current = m_Current.Lerp(c, _percent); // set current to current lerp percentage
			m_ChangeTime = std::chrono::steady_clock::now(); // Get changetime
			return m_Current;
		}

		return m_Current.Lerp(c, _percent);
	}
}