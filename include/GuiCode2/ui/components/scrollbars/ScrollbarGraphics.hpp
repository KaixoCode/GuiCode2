#pragma once
#include "GuiCode/ui/components/scrollbars/Scrollbar.hpp"
#include "GuiCode/ui/components/scrollbars/ScrollbarType.hpp"
#include "GuiCode/ui/graphics/Graphics.hpp"

// --------------------------------------------------------------------------
// ------------------------ Scrollbar Graphics ------------------------------
// --------------------------------------------------------------------------

namespace ScrollbarGraphics
{
	using namespace Graphics;

	// --------------------------------------------------------------------------
	// ------------------------------- Normal -----------------------------------
	// --------------------------------------------------------------------------

	struct Normal
	{
		template<typename ScrollbarType>
		static void Render(Scrollbar<Normal, ScrollbarType>& b, CommandCollection& d)
		{}
		
		template<>
		static void Render(Scrollbar<Normal, ScrollbarType::Horizontal>& b, CommandCollection& d)
		{
			int _p = 6;
			int _w = max((b.VisibleRange() - b.Range().start) / (float)(b.Range().end - b.Range().start) * b.Width(), (float)b.MinSize());
			int _x = (b.Value() - b.Range().start) / (float)(b.Range().end - b.Range().start - b.VisibleRange()) * (b.Width() - _w) + b.X();
			
			if (b.Hovering() && b.Mouse() >= _x && b.Mouse() <= _x + _w)
			{
				d.Command<Fill>(Color{ 50, 50, 50, 255 });
				d.Command<Quad>(b.Position().x, b.Position().y + b.Size().height / 2 - 1, b.Size().width, 2);
				d.Command<Fill>(Color{ 75, 75, 75, 255 });
				_p = 4;
			}
			else
				d.Command<Fill>(Color{ 55, 55, 55, 255 });

			d.Command<Quad>(_x + _p, b.Position().y + _p, _w - _p * 2, b.Size().height - _p * 2);
		}

		template<>
		static void Render(Scrollbar<Normal, ScrollbarType::Vertical>& b, CommandCollection& d)
		{
			int _p = 6;
			int _h = max((b.VisibleRange() - b.Range().start) / (float)(b.Range().end - b.Range().start) * b.Height(), (float)b.MinSize());
			int _y = (b.Value() - b.Range().start) / (float)(b.Range().end - b.Range().start - b.VisibleRange()) * -(b.Height()-_h) + b.Y() + b.Height() - _h;
			
			if (b.Hovering() && b.Mouse() >= _y && b.Mouse() <= _y + _h)
			{
				d.Command<Fill>(Color{ 50, 50, 50, 255 });
				d.Command<Quad>(b.Position().x + b.Size().width / 2 - 1, b.Position().y, 2, b.Size().height);
				d.Command<Fill>(Color{ 75, 75, 75, 255 });
				_p = 4;
			}
			else
				d.Command<Fill>(Color{ 55, 55, 55, 255 });

			d.Command<Quad>(b.Position().x + _p, _y + _p, b.Size().width - _p * 2, _h - _p * 2);
			
		}
	};
}