#include "GuiCode2/Layout.hpp"

namespace GuiCode
{
	Span* Span::Find(int id)
	{
		if (settings.id == id)
			return this;

		for (auto& i : divs)
		{
			Span* _div = i.Find(id);
			if (_div)
				return _div;
		}

		return nullptr;
	}

	void Span::Layout() 
	{
		if (component)
		{
			// Sizing
			switch(settings.layout.x)
			{
			case Fit: settings.dimensions.width = component->dimensions.width; break;
			case Fill: component->dimensions.width = settings.dimensions.width; break;
			}

			switch (settings.layout.y)
			{
			case Fit: settings.dimensions.height = component->dimensions.height; break;
			case Fill: component->dimensions.height = settings.dimensions.height; break;
			}

			// Positioning
			if (settings.align & Align::Right) component->dimensions.x = settings.dimensions.x + settings.dimensions.width - component->dimensions.width;
			else if (settings.align & Align::XCenter) component->dimensions.x = settings.dimensions.x + settings.dimensions.width / 2 - component->dimensions.width / 2;
			else component->dimensions.x = settings.dimensions.x;

			if (settings.align & Align::Bottom) component->dimensions.y = settings.dimensions.y + settings.dimensions.height - component->dimensions.height;
			else if (settings.align & Align::YCenter) component->dimensions.y = settings.dimensions.y + settings.dimensions.height / 2 - component->dimensions.height / 2;
			else component->dimensions.y = settings.dimensions.y;
		}
		else
		{
			for (auto& _div : divs)
				_div.Layout();
		}
	}
}