#include "GuiCode2/Panel.hpp"

namespace GuiCode
{
	Panel::Panel()
	{
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
		{
			span.State<Visible>(true);
			span.ForwardRender(d);
		}

		// Otherwise use normal rendering
		else
		{
			span.State<Visible>(false);
			Component::ForwardRender(d);
		}
	};
}