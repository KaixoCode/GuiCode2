#pragma once
#include "GuiCode2/pch.hpp"
#include "GuiCode2/Events/Event.hpp"
#include "GuiCode2/Graphics/Graphics.hpp"
#include "GuiCode2/Utils/Dimensions.hpp"
#include "GuiCode2/Utils/Pointer.hpp"

namespace GuiCode
{
	enum class Cursor
	{
		None, Arrow, IBeam, Crosshair, Hand, HResize, VResize
	};

	class Component : public Dimensions, public EventListener
	{
	public:
		Cursor cursor = Cursor::Arrow;

		Component();
		Component(const Component&);
		Component(Component&&);
		Component& operator=(const Component&);
		Component& operator=(Component&&);

		virtual ~Component() {};

		/**
		 * Get the value of a state
		 * @param state the state
		 * @return reference to the value of the state
		 */
		int& State(GuiCode::State state) { return m_States[state]; }
		int const& State(GuiCode::State state) const { return m_States[state]; }

		/**
		 * Get the first component found with the given state, depth first
		 * search, in the order of the zIndex.
		 * @param state state
		 * @return component or nullptr
		 */
		virtual Pointer<Component> Get(GuiCode::State state);

		/**
		 * Constrain the size of this component to its min/max
		 * usually gets called with every Update.
		 */
		virtual void ConstrainSize();

		/**
		 * Returns true when the position is inside
		 * the hitbox of this component.
		 * @param pos
		 * @return true when inside the hitbox
		 */
		virtual bool Hitbox(const Vec2<float>& pos) const { return pos.Inside(dimensions); }

		/**
		 * Return's the bounding box of this component, aka the smallest
		 * rectangle that encases the entire component.
		 * @return bounding box
		 */
		virtual Vec4<float> BoundingBox() const { return dimensions; }

		/**
		 * Default render method.
		 * @param d collection of commands to append to
		 */
		virtual void Render(CommandCollection& d) const {}

		/**
		 * Default update method. Gets called each frame before Render.
		 */
		virtual void Update() {}

		/**
		 * Used to forward the Render calls, separated so
		 * components that don't have to forward Render calls
		 * don't have to worry about calling Base::Render.
		 * @param d collection of commands
		 */
		virtual void ForwardRender(CommandCollection& d);

		/**
		 * Same as ForwardRender, but for Update.
		 */
		virtual void ForwardUpdate();

		template<std::derived_from<Component> Type, typename ...Args> requires std::constructible_from<Type, Args...>
		Type& emplace_back(Args&&...args) { return components.emplace_back(new Type{ std::forward<Args>(args)... }); }

		template<std::derived_from<Component> Type>
		Type& emplace_back(Type::Settings settings) { return components.emplace_back(new Type{ settings }); }

		template<std::derived_from<Component> Type>
		Type& push_back(Type::Settings settings) { return components.emplace_back(new Type{ settings }); }

		template<std::derived_from<Component> Type>
		Type& push_back(const Pointer<Type>& ptr) { return components.push_back(ptr); }

	private:
		mutable std::unordered_map<GuiCode::State, int> m_States;
		void InitListeners();
	};
}