#include "GuiCode/ui/components/menus/MenuAccessories.hpp"

// --------------------------------------------------------------------------
// ------------------------- Menu accessories -------------------------------
// --------------------------------------------------------------------------

namespace MenuAccessories
{
	using namespace Graphics;
	
	// --------------------------------------------------------------------------
	// ------------------------------ Divider -----------------------------------
	// --------------------------------------------------------------------------
	
	void Divider::Render(CommandCollection& d)
	{
		
		d.Command<Fill>(Color{ 51, 51, 51, 255 });
		d.Command<Quad>(X() + m_PaddingX, Y() + m_PaddingY, Width() - m_PaddingX * 2, m_Stroke);
	}
}