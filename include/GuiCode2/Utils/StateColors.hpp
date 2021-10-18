#pragma once
#include "GuiCode2/pch.hpp"
#include "GuiCode2/Components/Component.hpp"

namespace GuiCode
{
	/**
	 * Link this object to a Component, and then assign color
	 * to states. The order in which you assign color will 
	 * determine which state has priority, first added is
	 * higher priority. The Current method will then return
	 * the color by looking at the state of the Component.
	 */
	class StateColors
	{
	public:
		struct Settings
		{
			Component* link = nullptr;
			Color base{ 0, 0, 0, 0 };
			std::map<GuiCode::State, Color> colors;
			float transition = 100;
		};

		StateColors(const Settings& settings = {});
		StateColors(const StateColors& other) { *this = other; }
		StateColors(StateColors&& other) { *this = other; }
		StateColors& operator=(const StateColors& other);
		StateColors& operator=(StateColors&& other);

		void Link(Component* link) { m_Link = link; }
		void Clear();

		Color& State(GuiCode::State state);
		void Remove(GuiCode::State state);

		Color Current() const;

		Color base{ 0, 0, 0, 0 }; // Base color, when no state is present
		float transition = 100;   // Transition time in milliseconds

	private:
		Component* m_Link;
		std::list<GuiCode::State> m_States;
		std::map<GuiCode::State, Color> m_ColorMap;
		mutable Color m_Current = base;
		mutable GuiCode::State m_Goal = NoValue;
		mutable std::chrono::steady_clock::time_point m_ChangeTime;
	};
}