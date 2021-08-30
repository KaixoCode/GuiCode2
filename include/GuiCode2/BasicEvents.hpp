#pragma once
#include "GuiCode2/pch.hpp"
#include "GuiCode2/Event.hpp"
#include "GuiCode2/Component.hpp"
namespace GuiCode
{
	struct EventMods
	{
		enum {
			Control      = 0x0001,
			RightControl = 0x0002,
			LeftControl  = 0x0004,
			Shift        = 0x0008, 
			RightShift   = 0x0010, 
			LeftShift    = 0x0020, 
			Alt          = 0x0100, 
			LeftAlt      = 0x0200, 
			RightAlt     = 0x0400, 
			NumLock      = 0x0800, 
			CapsLock     = 0x1000
		};
	};

	struct MouseButton
	{
		enum {
			None   = 0x0000,
			Left   = 0x0001, 
			Right  = 0x0002, 
			Middle = 0x0004
		};
	};

	struct Render : public Event
	{
		Render(CommandCollection& c)
			: collection(c)
		{}

		bool Forward(const Component& c) const override { return true; };

		CommandCollection& collection;
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
		MouseDrag(Vec2<double> pos, int buttons, int mod)
			: pos(pos), buttons(buttons), mod(mod)
		{}

		bool Forward(const Component& c) const override { return c.State<Pressed>(); };

		Vec2<double> pos; // Position of cursor
		int buttons;      // Buttons held down
		int mod;          // Key mods
	};

	struct MousePress : public Event
	{
		MousePress(Vec2<double> pos, int button, int mod)
			: pos(pos), button(button), mod(mod)
		{}

		bool Forward(const Component& c) const override { return c.State<Hovering>(); };

		Vec2<double> pos;
		int button;
		int mod;
	};

	struct MouseClick : public Event
	{
		MouseClick(Vec2<double> pos, int button, int mod)
			: pos(pos), button(button), mod(mod)
		{}

		bool Forward(const Component& c) const override { return c.State<Hovering>(); };

		Vec2<double> pos;
		int button;
		int mod;
	};

	struct MouseWheel : public Event
	{
		MouseWheel(Vec2<double> pos, int amount, int mod)
			: pos(pos), amount(amount), mod(mod)
		{}

		bool Forward(const Component& c) const override { return c.State<Hovering>(); };

		Vec2<double> pos;
		int amount;
		int mod;
	};

	struct MouseRelease : public Event
	{
		MouseRelease(Vec2<double> pos, int button, int mod)
			: pos(pos), button(button), mod(mod)
		{}

		bool Forward(const Component& c) const override { return c.State<Focused>(); };

		Vec2<double> pos;
		int button;
		int mod;
	};

	struct KeyPress : public Event
	{
		KeyPress(int code, int mod, bool repeat)
			: keycode(code), mod(mod), repeat(repeat)
		{}

		bool Forward(const Component& c) const override { return true; };

		int keycode;
		int mod;
		bool repeat;
	};

	struct KeyType : public Event
	{
		KeyType(char key, int mod, int coded)
			: key(key), mod(mod), coded(coded)
		{}

		bool Forward(const Component& c) const override { return true; };

		char key;
		int mod;
		int coded;
	};

	struct KeyRelease : public Event
	{
		KeyRelease(int code, int mod)
			: keycode(code), mod(mod)
		{}

		bool Forward(const Component& c) const override { return true; };

		int keycode;
		int mod;
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