#include "GuiCode/ui/components/scrollbars/Scrollbar.hpp"

// --------------------------------------------------------------------------
// -------------------------- Scrollbar Base --------------------------------
// --------------------------------------------------------------------------

ScrollbarBase::ScrollbarBase()
	: Component()
{
	m_Listener += [this](Event::MouseEntered& e)
	{
		m_Hovering = true;
	};

	m_Listener += [this](Event::MouseExited& e)
	{
		m_Hovering = false;
	};
}

void ScrollbarBase::Update(const Vec4<int>& viewport)
{
	m_Value = constrain(m_Value, m_Range.start, m_Range.end - m_VisibleRange);
}