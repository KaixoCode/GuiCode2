#pragma once
#include "GuiCode/ui/components/panels/Panel.hpp"

// --------------------------------------------------------------------------
// ----------------------------- Tree Panel ---------------------------------
// --------------------------------------------------------------------------

class TreePanel : public Panel 
{
public:
	TreePanel();

	void SortItems(bool reverse);
};