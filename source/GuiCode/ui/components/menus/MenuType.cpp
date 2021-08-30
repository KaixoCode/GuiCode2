#include "GuiCode/ui/components/menus/MenuType.hpp"

// --------------------------------------------------------------------------
// ---------------------------- Menu Type -----------------------------------
// --------------------------------------------------------------------------

namespace MenuType
{
	// --------------------------------------------------------------------------
	// ---------------------------- Toggle Hover --------------------------------
	// --------------------------------------------------------------------------
	// -- Activates the button you hover over if one of the buttons is active ---
	// --------------------------------------------------------------------------

	void ToggleHover::Update(const Vec4<int>& viewport)
	{
		// Check if a button is active
		ButtonBase* _ed = { 0 };
		for (auto& _i : Get())
			if ((_ed = dynamic_cast<ButtonBase*>(_i.get())) != nullptr && _ed->Active())
				break;

		// Switch deactivate the active button and activate the button under the mouse
		ButtonBase* _ed2;
		if (_ed != nullptr && _ed->Active() && (_ed2 = dynamic_cast<ButtonBase*>(m_Hovering)) != nullptr && _ed != _ed2)
		{
			_ed->Active(false);
			m_Focussed = _ed2;
			_ed2->Active(true);
		}

		MenuBase::Update(viewport);
	}
}