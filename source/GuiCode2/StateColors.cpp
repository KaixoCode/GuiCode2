#include "GuiCode2/StateColors.hpp"

namespace GuiCode
{
	StateColors::StateColors(Component& link)
		: m_Link(link)
	{}

	StateColors& StateColors::operator=(const StateColors& other)
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
		m_States.push_back(state);
		m_ColorMap.emplace(state, color);
	}

	void StateColors::Remove(int state)
	{
		if (m_ColorMap.contains(state))
		{
			m_States.remove(state);
			m_ColorMap.erase(state);
		}
	}

	const Color& StateColors::Current() const
	{
		for (auto& i : m_States)
			if (m_Link.State(i))
				return m_ColorMap.at(i);
		return base;
	}
}