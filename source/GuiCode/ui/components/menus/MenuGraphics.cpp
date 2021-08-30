#include "GuiCode/ui/components/menus/MenuGraphics.hpp"
#include "GuiCode/ui/graphics/Theme.hpp"
#include "GuiCode/ui/graphics/Graphics.hpp"

// --------------------------------------------------------------------------
// --------------------------- MenuGraphics ---------------------------------
// --------------------------------------------------------------------------

namespace MenuGraphics
{
	using namespace Graphics;

	// --------------------------------------------------------------------------
	// ------------------------------ Vertical ----------------------------------
	// --------------------------------------------------------------------------

	void Horizontal::Render(MenuBase& b, CommandCollection& d)
	{
		auto& _menuItems = b.Get();

		int _x = b.X();
		int _y = b.Y();
		int _ix = b.X();
		int _mh = 0;

		for (auto& _item : _menuItems)
		{
			_item->Position({ _x, _y });
			_x += _item->Width();
			if (_item->Height() > _mh)
				_mh = _item->Height();
		}

		b.Size({ _x - _ix, _mh });
	}

	// --------------------------------------------------------------------------
	// ----------------------------- Horizontal ---------------------------------
	// --------------------------------------------------------------------------

	void MenuGraphics::Vertical::Render(MenuBase& b, CommandCollection& d)
	{
		auto& _menuItems = b.Get();

		int _padding = 2;
		int _x = b.X();
		int _y = b.Y() + b.Height();
		int _iy = _y;
		int _mw = 0;

		d.Command<Fill>(Theme::Menu::border);
		d.Command<Quad>(b.X() - 1, b.Y() - 1, b.Width() + 2, b.Height() + 2);

		d.Command<Fill>(Theme::Menu::background);
		d.Command<Quad>(b.X(), b.Y(), b.Width(), b.Height());

		for (auto& _item : _menuItems)
		{
			_y -= _item->Height();
			_item->Position({ _x + _padding, _y - _padding });
			if (_item->Width() > _mw)
				_mw = _item->Width();
		}

		b.Size({ _mw + _padding * 2, -_y + _iy + _padding * 2 });
	}
}