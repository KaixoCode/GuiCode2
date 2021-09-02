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
		Hovering, Focused, Pressed, Visible
	};

	class Component : public Dimensions
	{
	public:
		Component();

		Component(const Component& c) 
			: m_Components(c.m_Components), listener(m_Components), 
			m_States(c.m_States)
		{}

		virtual ~Component() {};

	private:
		std::list<Component*> m_Components;

	public:
		EventListener listener;

		virtual bool Hitbox(const Vec2<float>& pos) const { return pos.Inside(dimensions); }

		virtual void CalculateOrder();
		virtual void ConstrainSize();
		virtual void RegisterComponent(Component& c);
		virtual void UnregisterComponent(Component& obj);

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


		Component* Get(int state)
		{
			Component* _c;
			for (auto& i : m_Components)
			{
				_c = i->Get(state);
				if (_c)
					return _c;
			}

			if (State(state))
				return this;

			return nullptr;
		}


	private:
		mutable std::unordered_map<int, int> m_States;
	};
}