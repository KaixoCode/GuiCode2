#include "GuiCode/ui/components/menus/Menu.hpp"

// --------------------------------------------------------------------------
// ---------------------------- Menu Base -----------------------------------
// --------------------------------------------------------------------------

bool MenuBase::WithinBounds(const Vec2<int>& pos) const
{
	bool _within = Component::WithinBounds(pos);
	for (auto& _i : m_Components)
		_within |= _i->WithinBounds(pos);
	return _within;
}