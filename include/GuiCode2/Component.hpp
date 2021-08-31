#pragma once
#include "GuiCode2/pch.hpp"
#include "GuiCode2/Event.hpp"
#include "GuiCode2/Graphics.hpp"

namespace GuiCode
{
	struct Dimensions
	{
		Dimensions()
			: dimensions{ 0, 0, 0, 0 }
		{}

		Dimensions(double a, double b, double c, double d)
			: dimensions{ a, b, c, d }
		{}

		Dimensions(Dimensions&& other) { *this = other; }
		Dimensions(const Dimensions& other) { *this = other; }

		union
		{
			struct { double x, y, width, height; };
			struct { Vec2<double> position, size; };
			Vec4<double> dimensions;
		};
		double zIndex = 0;

		Dimensions& operator=(Dimensions&& other)
		{
			dimensions = other.dimensions;
			zIndex = other.zIndex;
			return *this;
		}

		Dimensions& operator=(const Dimensions& other)
		{
			dimensions = other.dimensions;
			zIndex = other.zIndex;
			return *this;
		}
	};

	enum State
	{
		NoValue = 0, 
		Hovering, Focused, Pressed, UseDepth,
	};

	class Component : public Dimensions
	{
	public:
		Component();

	private:
		std::list<Component*> m_Components;

	public:
		EventListener listener;

		virtual bool Hitbox(const Vec2<double>& pos) const { return pos.Inside(dimensions); }

		void CalculateOrder();
		void RegisterComponent(Component& c);
		void UnregisterComponent(Component& obj);

		virtual void Render(CommandCollection& d) const {};

		template<int state>
		int State() const { return m_States.contains(state) ? m_States.at(state) : NoValue; }
		int State(int state) const { return m_States.contains(state) ? m_States.at(state) : NoValue; }

		template<int state>
		void State(int value) { m_States[state] = value; }
		void State(int state, int value) { m_States[state] = value; }

	protected:
		virtual void ForwardRender(CommandCollection& d) final;

	private:
		std::map<int, int> m_States;
	};
}