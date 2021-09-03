#pragma once
#include "GuiCode2/pch.hpp"
#include "GuiCode2/Event.hpp"
#include "GuiCode2/Graphics.hpp"

namespace GuiCode
{
	/**
	 * Dimensions object contains a union for easy access to all
	 * aspects of dimensions, like position/size etc. Also contains
	 * a z-index
	 */
	struct Dimensions
	{
		Dimensions() : dimensions{ 0, 0, 0, 0 } {}
		Dimensions(float a, float b, float c, float d) : dimensions{ a, b, c, d } {}
		Dimensions(Dimensions&& other) { *this = other; }
		Dimensions(const Dimensions& other) { *this = other; }

		Vec2<float> min{ -1, -1 };
		Vec2<float> max{ -1, -1 };

		union
		{
			struct { float x, y, width, height; };
			struct { Vec2<float> position, size; };
			Vec4<float> dimensions;
		};
		float zIndex = 0;

		Dimensions& operator=(Dimensions&& other);
		Dimensions& operator=(const Dimensions& other);
	};

	/**
	 * Standard states
	 */
	enum State
	{
		NoValue = 0, 
		Hovering, Focused, Pressed, Visible
	};

	/**
	 * Base class for objects that receive events and are drawable.
	 */
	class Component : public Dimensions
	{
	public:
		Component();

		virtual ~Component() {};

		std::list<Component*> components;
		EventListener listener;

		virtual bool Hitbox(const Vec2<float>& pos) const { return pos.Inside(dimensions); }

		virtual void CalculateOrder();
		virtual void ConstrainSize();

		virtual void Render(CommandCollection& d) const {};
		virtual void Update() {};

		template<int state>
		int State() const { return m_States[state]; }
		int State(int state) const { return m_States[state]; }

		template<int state>
		void State(int value) { m_States[state] = value; }
		void State(int state, int value) { m_States[state] = value; }

		virtual void ForwardRender(CommandCollection& d);
		virtual void ForwardUpdate();

	private:
		mutable std::unordered_map<int, int> m_States;
	
		void InitListeners();
	};

	class ColorState
	{
	public:
		ColorState(Component& link)
			: m_Link(link)
		{}

		ColorState& operator=(const ColorState& other)
		{
			base = other.base;
			m_States = other.m_States;
			m_ColorMap = other.m_ColorMap;
		}

		void Reset()
		{
			m_States.clear();
			m_ColorMap.clear();
		}

		template<int state>
		inline void State(Color color) { State(state, color); }
		inline void State(int state, Color color)
		{
			m_States.push_back(state);
			m_ColorMap.emplace(state, color);
		}

		Color Current() const
		{
			for (auto& i : m_States)
				if (m_Link.State(i))
					return m_ColorMap.at(i);
			return base;
		}

		Color base{ 0, 0, 0 };

	private:
		Component& m_Link;
		std::list<int> m_States;
		std::map<int, Color> m_ColorMap;
	};
}