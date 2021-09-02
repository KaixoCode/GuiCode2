#include "GuiCode2/Panel.hpp"

namespace GuiCode
{
	Panel::Panel()
	{
		// Hovering state has limit of 1. When MouseMoved event occurs it will check hovering.
		// if there is a change in state it will also send out the MouseEnter/MouseExit event
		// accordingly. MouseEnter/MouseExit also update the state, which is crucial for 
		// sub-components.
		((listener.State<Hovering>({ .limit = 1 }) 
			+= [](const MouseMove& e, const Component& c) -> int
			{
				int prev = c.State<Hovering>();
				int curr = c.Hitbox(e.pos);

				if (!prev && curr) c.listener(MouseEnter{});
				else if (prev && !curr) c.listener(MouseExit{});

				return curr;
			})
			+= [](const MouseEnter& e, const Component& c) -> int { return true; })
			+= [](const MouseExit& e, const Component& c) -> int { return false; };
		
		// The focused state also has a limit of 1, and is triggered by a MousePress event
		// If there is a state change it will also send out the Focus/Unfocus events accordingly.
		listener.State<Focused>({ .limit = 1 }) += [](const MousePress& e, const Component& c) -> int
		{
			int prev = c.State<Focused>();
			int curr = c.State<Hovering>();

			if (!prev && curr) c.listener(Focus{});
			else if (prev && !curr) c.listener(Unfocus{});

			return curr;
		};

		// Pressed also has limit of 1, and is handled simply by the MousePress and MouseRelease
		(listener.State<Pressed>({ .limit = 1 }) 
			+= [](const MousePress& e, const Component& c) -> int { return c.State<Hovering>(); })
			+= [](const MouseRelease& e, const Component& c) -> int { return false; };

		components.push_back(&span);
	}

	void Panel::ForwardUpdate()
	{
		// Set dimensions of the span, then forward normally
		span.settings.size = size; 
		span.SetDimensions(dimensions);
		span.zIndex = std::numeric_limits<int>::max();
		Component::ForwardUpdate();
	}

	void Panel::ForwardRender(CommandCollection& d)
	{
		// If span contains anything, use span
		if (span.component || span.spans.size() > 0)
			span.ForwardRender(d);

		// Otherwise use normal rendering
		else
			Component::ForwardRender(d);
	};
}