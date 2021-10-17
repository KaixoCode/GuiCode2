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

	Color& StateColors::State(GuiCode::State state)
	{
		if (!m_ColorMap.contains(state))
		{
			m_States.push_back(state);
			m_ColorMap.emplace(state, Color{});
		}

		return m_ColorMap[state];
	}

	void StateColors::Remove(GuiCode::State state)
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

		GuiCode::State _state = NoValue;
		for (auto& i : m_States)
			if (m_Link->State(i))
			{
				_state = i;
				break;
			}

		auto _now = std::chrono::steady_clock::now();
		auto _duration = std::chrono::duration_cast<std::chrono::milliseconds>(_now - m_ChangeTime).count();
		float _percent = constrain(_duration / transition, 0, 1);

		const Color& c = m_Goal == NoValue ? base : m_ColorMap.at(m_Goal);

		if (_state != m_Goal)
		{
			m_Goal = _state;
			m_Current = m_Current.Lerp(c, _percent);
			m_ChangeTime = std::chrono::steady_clock::now();
			return m_Current;
		}

		return m_Current.Lerp(c, _percent);
	}
}