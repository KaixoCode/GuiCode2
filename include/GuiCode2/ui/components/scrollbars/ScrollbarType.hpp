#pragma once
#include "GuiCode/ui/components/scrollbars/Scrollbar.hpp"
#include "GuiCode/ui/graphics/Graphics.hpp"

// --------------------------------------------------------------------------
// -------------------------- Scrollbar Type --------------------------------
// --------------------------------------------------------------------------

namespace ScrollbarType
{
	// --------------------------------------------------------------------------
	// ----------------------------- Vertical -----------------------------------
	// --------------------------------------------------------------------------

	class Vertical : public ScrollbarBase
	{
	public:
		Vertical();
	
	private:
		bool m_Dragging = false;
	};

	// --------------------------------------------------------------------------
	// ---------------------------- Horizontal ----------------------------------
	// --------------------------------------------------------------------------

	class Horizontal : public ScrollbarBase
	{
	public:
		Horizontal();

	private:
		bool m_Dragging = false;
	};
}