#pragma once
#include "GuiCode2/pch.hpp"
#include "GuiCode2/Component.hpp"
namespace GuiCode
{
	struct MouseButton
	{
		enum {
			None   = 0x0000,
			Left   = 0x0001, 
			Right  = 0x0002, 
			Middle = 0x0004
		};
	};

	struct MouseMove : public Event
	{
		MouseMove(Vec2<float> pos)
			: pos(pos), Event(false)
		{}

		bool Forward(const Component& c) const override { return c.State(Hovering); };

		Vec2<float> pos; // New mouse position
	};

	struct MouseDrag : public Event
	{
		MouseDrag(Vec2<float> source, Vec2<float> pos, int buttons, int mod)
			: source(source), pos(pos), buttons(buttons), mod(mod), Event(false)
		{}

		bool Forward(const Component& c) const override { return c.State(Pressed); };

		Vec2<float> source; // Press position of cursor
		Vec2<float> pos;    // Position of cursor
		int buttons;        // Buttons held down
		int mod;            // Key mods
	};

	struct MousePress : public Event
	{
		MousePress(Vec2<float> pos, int button, int mod)
			: pos(pos), button(button), mod(mod), Event(false)
		{}

		bool Forward(const Component& c) const override { return c.State(Hovering); };

		Vec2<float> pos; // Position of press
		int button;      // Button held down
		int mod;         // Key mods
	};

	struct MouseClick : public Event
	{
		MouseClick(Vec2<float> pos, int button, int mod)
			: pos(pos), button(button), mod(mod), Event(false)
		{}

		bool Forward(const Component& c) const override { return c.State(Hovering); };

		Vec2<float> pos; // Position of click
		int button;      // Button used to click
		int mod;         // Key mods
	};

	struct MouseWheel : public Event
	{
		MouseWheel(Vec2<float> pos, int amount, int mod)
			: pos(pos), amount(amount), mod(mod)
		{}

		bool Forward(const Component& c) const override { return c.State(Hovering); };

		Vec2<float> pos; // Cursor position when mousewheel was used
		int amount;      // Amount the mousewheel was used
		int mod;         // Key mods
	};

	struct MouseRelease : public Event
	{
		MouseRelease(Vec2<float> pos, int button, int mod)
			: pos(pos), button(button), mod(mod), Event(false)
		{}

		bool Forward(const Component& c) const override { return c.State(Focused); };

		Vec2<float> pos; // Position of the cursor
		int button;      // Button that was released
		int mod;         // Key mods
	};

	struct KeyPress : public Event
	{
		KeyPress(int code, int mod, bool repeat)
			: keycode(code), mod(mod), repeat(repeat), Event(false)
		{}

		bool Forward(const Component& c) const override { return true; };

		int keycode; // Key code
		int mod;     // Key mods
		bool repeat; // Is repeat key (fired when key held down)
	};

	struct KeyType : public Event
	{
		KeyType(char key, int mod, bool coded)
			: key(key), mod(mod), coded(coded), Event(false)
		{}

		bool Forward(const Component& c) const override { return true; };

		char key;   // Character typed
		int mod;    // Key mods
		bool coded; // Is it a coded character
	};

	struct KeyRelease : public Event
	{
		KeyRelease(int code, int mod)
			: keycode(code), mod(mod)
		{}

		bool Forward(const Component& c) const override { return true; };

		int keycode; // Key code of released key
		int mod;     // Key mods
	};

	struct MouseExit : public Event
	{
		bool Forward(const Component& c) const override { return c.State(Hovering); };
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
		bool Forward(const Component& c) const override { return c.State(Focused); };
	};
}