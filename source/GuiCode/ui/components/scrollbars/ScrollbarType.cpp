#include "GuiCode/ui/components/scrollbars/ScrollbarType.hpp"

// --------------------------------------------------------------------------
// -------------------------- Scrollbar Type --------------------------------
// --------------------------------------------------------------------------

namespace ScrollbarType
{
	// --------------------------------------------------------------------------
	// ----------------------------- Vertical -----------------------------------
	// --------------------------------------------------------------------------

	Vertical::Vertical()
	{
		m_Listener += [this](Event::MousePressed& e)
		{
			m_PressMouse = e.y;
			m_PressValue = m_Value;

			int _h = max((VisibleRange() - Range().start) / (float)(Range().end - Range().start) * Height(), (float)MinSize());
			int _y = (Value() - Range().start) / (float)(Range().end - Range().start - VisibleRange()) * -(Height() - _h) + Y() + Height() - _h;
			if (m_PressMouse > _y && m_PressMouse < _y + _h)
				m_Dragging = true;
		};

		m_Listener += [this](Event::MouseReleased& e)
		{
			m_Dragging = false;
		};

		m_Listener += [this](Event::MouseMoved& e)
		{
			m_Mouse = e.y;
		};

		m_Listener += [this](Event::MouseDragged& e)
		{
			if (e.button != Event::MouseButton::LEFT || !m_Dragging)
				return;

			m_Mouse = constrain(e.y, Y(), Height());
			int _h = max((VisibleRange() - Range().start) / (float)(Range().end - Range().start) * Height(), (float)MinSize());
			m_Value = m_PressValue - ((e.y - m_PressMouse) / (float)(Height() - _h)) * (m_Range.end - m_Range.start - m_VisibleRange) + m_Range.start;
			m_Value = constrain(m_Value, m_Range.start, m_Range.end - m_VisibleRange);
		};

		Width(16);
	}

	// --------------------------------------------------------------------------
	// ---------------------------- Horizontal ----------------------------------
	// --------------------------------------------------------------------------

	Horizontal::Horizontal()
	{
		m_Listener += [this](Event::MousePressed& e)
		{
			m_PressMouse = e.x;
			m_PressValue = m_Value;

			int _w = max((VisibleRange() - Range().start) / (float)(Range().end - Range().start) * Width(), (float)MinSize());
			int _x = (Value() - Range().start) / (float)(Range().end - Range().start - VisibleRange()) * (Width() - _w) + X();
			if (m_PressMouse > _x && m_PressMouse < _x + _w)
				m_Dragging = true;
		};

		m_Listener += [this](Event::MouseMoved& e)
		{
			m_Mouse = e.x;
		};

		m_Listener += [this](Event::MouseDragged& e)
		{	
			if (e.button != Event::MouseButton::LEFT || !m_Dragging)
				return;

			m_Mouse = constrain(e.x, X(), Width());
			int _w = max((VisibleRange() - Range().start) / (float)(Range().end - Range().start) * Width(), (float)MinSize());
			m_Value = m_PressValue + ((e.x - m_PressMouse) / (float)(Width() - _w)) * (m_Range.end - m_Range.start - m_VisibleRange) + m_Range.start;
			m_Value = constrain(m_Value, m_Range.start, m_Range.end - m_VisibleRange);
		};

		Height(16);
	}
}