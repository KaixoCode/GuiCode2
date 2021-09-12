#pragma once
#include "GuiCode2/pch.hpp"
#include "GuiCode2/Event.hpp"
#include "GuiCode2/Graphics.hpp"
#include "GuiCode2/Dimensions.hpp"

namespace GuiCode
{
	/**
	 * Standard states.
	 */
	enum State
	{
		NoValue = 0, 
		Hovering, Focused, Pressed, Visible
	};

	enum Cursor
	{
		Arrow, IBeam, Crosshair, Hand, HResize, VResize
	};

	/**
	 * Base class for objects that receive events, have state, and are drawable.
	 */
	class Component : public Dimensions
	{
	public:
		Component();
		virtual ~Component() {};

		/**
		 * List of pointers to components that are registered to 
		 * have events/render calls/update calls forwarded to them.
		 * Make sure to remove component from the list whenever its 
		 * lifetime expires!!!
		 */
		std::list<Wrapper<Component>> components;

		/**
		 * Listener of this component, will handle forwarding of events
		 * to components, and contains a StateHandler to handle the 
		 * states of all components. Register event listeners and
		 * state handlers through this object.
		 */
		EventListener listener;

		uint8_t cursor = Cursor::Arrow;

		/**
		 * Return's true when the position in within this 
		 * component's hitbox. Used by mouse events.
		 * @param pos position
		 * @return true when within hitbox
		 */
		virtual bool Hitbox(const Vec2<float>& pos) const { return pos.Inside(dimensions); }
		virtual Vec4<float> BoundingBox() const { return dimensions; }

		/**
		 * Calculates the z ordering of the components using their z-index.
		 */
		virtual void CalculateOrder();

		/**
		 * Constrains the size to the component's min and max size.
		 */
		virtual void ConstrainSize();

		virtual void Render(CommandCollection& d) const {};
		virtual void Update() {};

		/**
		 * Get the value of a state.
		 * @param state state
		 * @return value of state.
		 */
		template<int state>
		int State() const { return m_States[state]; }
		int State(int state) const { return m_States[state]; }

		/**
		 * Set the value of a state.
		 * @param state state
		 * @param value value
		 */
		template<int state>
		void State(int value) { m_States[state] = value; }
		void State(int state, int value) { m_States[state] = value; }

		virtual Component* Get(int state);

		virtual void ForwardRender(CommandCollection& d);
		virtual void ForwardUpdate();

	private:
		mutable std::unordered_map<int, int> m_States;
		void InitListeners();
	};
}