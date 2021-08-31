#include "GuiCode2/Panel.hpp"

namespace GuiCode
{
	Panel::Panel()
	{
		listener.State<Hovering>({ .limit = 1 }) += [](const MouseMove& e, const Component& c) -> int
		{
			int prev = c.State<Hovering>();
			int curr = c.Hitbox(e.pos);

			if (!prev && curr)
				c.listener(MouseEnter{});

			else if (prev && !curr)
				c.listener(MouseExit{});

			return curr;
		};
		
		listener.State<Focused>({ .limit = 1 }) += [](const MousePress& e, const Component& c) -> int
		{
			int prev = c.State<Focused>();
			int curr = c.State<Hovering>();

			if (!prev && curr)
				c.listener(Focus{});

			else if (prev && !curr)
				c.listener(Unfocus{});

			return curr;
		};

		(listener.State<Pressed>({ .limit = 1 }) 
			+= [](const MousePress& e, const Component& c) -> int { return c.State<Hovering>(); })
			+= [](const MouseRelease& e, const Component& c) -> int { return false; };
	}
}