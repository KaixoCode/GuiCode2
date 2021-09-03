#pragma once
#include "GuiCode2/pch.hpp"
#include "GuiCode2/Component.hpp"

namespace GuiCode
{
	/**
	 * Link this object to a Component, and then assign colors
	 * to states. The order in which you assign colors will 
	 * determine which state has priority, first added is
	 * higher priority. The Current method will then return
	 * the color by looking at the state of the Component.
	 */
	class StateColors
	{
	public:
		StateColors(Component& link);
		StateColors& operator=(const StateColors& other);

		/**
		 * Clears all colors apart from the base color.
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
		const Color& Current() const;

		Color base{ 0, 0, 0 }; // Base color, when no state is present

	private:
		Component& m_Link;
		std::list<int> m_States;
		std::map<int, Color> m_ColorMap;
	};
}