#pragma once
#include "GuiCode2/pch.hpp"
#include "GuiCode2/Event.hpp"
#include "GuiCode2/Component.hpp"
namespace GuiCode
{
	struct EventMods
	{
		enum {
			Control, 
			Shift, 
			Middle, 
			Left, 
			Right, 
			Alt, 
			NumLock, 
			CapsLock
		};
	};

	struct MouseButton
	{
		enum {
			Left, Right, Middle
		};
	};

	struct MouseMove : public Event
	{
		MouseMove(Vec2<double> pos)
			: pos(pos)
		{}

		bool Forward(const Component& c) const override { return c.State<Hovering>(); };

		Vec2<double> pos;
	};

	struct MouseDrag : public Event
	{
		MouseDrag(Vec2<double> pos)
			: pos(pos)
		{}

		bool Forward(const Component& c) const override { return c.State<Pressed>(); };

		Vec2<double> pos;
	};

	struct MousePress : public Event
	{
		MousePress(Vec2<double> pos)
			: pos(pos)
		{}

		bool Forward(const Component& c) const override { return c.State<Hovering>(); };

		Vec2<double> pos;
	};

	struct MouseWheel : public Event
	{
		MouseWheel(Vec2<double> pos)
			: pos(pos)
		{}

		bool Forward(const Component& c) const override { return c.State<Hovering>(); };

		Vec2<double> pos;
	};

	struct MouseRelease : public Event
	{
		MouseRelease(Vec2<double> pos)
			: pos(pos)
		{}

		bool Forward(const Component& c) const override { return c.State<Focused>(); };

		Vec2<double> pos;
	};

	struct MouseExit : public Event
	{
		bool Forward(const Component& c) const override { return true; };
	};

	struct MouseEnter : public Event
	{
		bool Forward(const Component& c) const override { return false; };
	};

	struct Focus : public Event
	{
		bool Forward(const Component& c) const override { return false; };
	};

	struct Unfocus : public Event
	{
		bool Forward(const Component& c) const override { return true; };
	};
}