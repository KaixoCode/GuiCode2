#pragma once
#include "GuiCode2/pch.hpp"
#include "GuiCode2/Component.hpp"

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
		StateColors(Component* link = nullptr);
		StateColors& operator=(const StateColors& other);
		StateColors& operator=(StateColors&& other);

		/**
		 * Link it to a component.
		 * @param link component
		 */
		void Link(Component* link) { m_Link = link; }

		/**
		 * Clears all color apart from the base color.
		 */
		void Clear();

		template<int state>
		void State(Color color) { State(state, color); }
		void State(int state, Color color);

		/**
		 * Remove a state.
		 * @param state state
		 */
		template<int state>
		void Remove() { Remove(state); }
		void Remove(int state);

		/**
		 * Get the current color based on state of the Component.
		 * @return color
		 */
		Color Current() const;

		Color base{ 0, 0, 0, 0 }; // Base color, when no state is present
		float transition = 100; // Transition time in milliseconds

	private:
		Component* m_Link;
		std::list<int> m_States;
		std::map<int, Color> m_ColorMap;
		mutable Color m_Current = base;
		mutable int m_Goal = NoValue;
		mutable std::chrono::steady_clock::time_point m_ChangeTime;
	};
}