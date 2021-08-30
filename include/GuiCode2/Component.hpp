#pragma once
#include "GuiCode2/pch.hpp"
#include "GuiCode2/Event.hpp"
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

		Dimensions& operator=(Dimensions&& other)
		{
			dimensions = other.dimensions;
			return *this;
		}

		Dimensions& operator=(const Dimensions& other)
		{
			dimensions = other.dimensions;
			return *this;
		}
	};

	enum State
	{
		NoValue = 0, 
		Hovering, Focused, Pressed, 
	};

	class Component : public Dimensions
	{
	public:
		virtual bool Hitbox(const Vec2<double>& pos) const { return pos.Inside(dimensions); }

		template<int state>
		int State() const { return m_States.contains(state) ? m_States.at(state) : NoValue; }
		int State(int state) const { return m_States.contains(state) ? m_States.at(state) : NoValue; }

		template<int state>
		void State(int value) { m_States[state] = value; }
		void State(int state, int value) { m_States[state] = value; }

		EventListener listener;

	private:
		std::map<int, int> m_States;
	};
}