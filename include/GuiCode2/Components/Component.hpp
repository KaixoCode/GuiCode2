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

	class Component : public Dimensions
	{
	public:
		Component();
		Component(Component&& other);
		Component(const Component&) = delete;
		Component& operator=(Component&&);
		Component& operator=(const Component&) = delete;
		virtual ~Component() {};

		std::vector<Pointer<Component>> components;
		EventListener listener;
		Cursor cursor = Cursor::Arrow;

		int& State(GuiCode::State state) { return m_States[state]; }
		int const& State(GuiCode::State state) const { return m_States[state]; }

		virtual Pointer<Component> Get(GuiCode::State state);

		virtual void ConstrainSize();

		virtual bool Hitbox(const Vec2<float>& pos) const { return pos.Inside(dimensions); }
		virtual Vec4<float> BoundingBox() const { return dimensions; }

		virtual void Render(CommandCollection& d) const {}
		virtual void Update() {}

		virtual void ForwardRender(CommandCollection& d);
		virtual void ForwardUpdate();

	private:
		mutable std::unordered_map<GuiCode::State, int> m_States;
		void InitListeners();
	};
}