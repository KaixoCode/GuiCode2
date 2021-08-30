#include "GuiCode/ui/components/panels/ScrollPanel.hpp"

// --------------------------------------------------------------------------
// ---------------------------- Scroll Panel --------------------------------
// --------------------------------------------------------------------------

ScrollPanel::ScrollPanel()
	: Panel()
{
	m_Listener += [this](Event::MouseWheel& e)
	{
		if (!e.wheelmod && m_ScrollbarY->Visible())
			m_ScrollbarY->Scroll(-e.amount / 4);
	};

	m_Listener += [this](Event::MousePressed& e)
	{
		m_PMouseX = e.x;
		m_PMouseY = e.y;
		m_PValX = m_ScrollbarX->Value();
		m_PValY = m_ScrollbarY->Value();
	};

	m_Listener += [this](Event::MouseDragged& e)
	{
		if (e.button == Event::MouseButton::MIDDLE)
		{
			if (m_ScrollbarY->Visible())
				m_ScrollbarY->Value(m_PValY + e.y - m_PMouseY);
			if (m_ScrollbarX->Visible())
				m_ScrollbarX->Value(m_PValX - e.x + m_PMouseX);
		}
	};
}

void ScrollPanel::Update(const Vec4<int>& viewport) 
{
	if (m_Panel)
	{
		int yw = m_ScrollbarY->Width();
		int xh = m_ScrollbarX->Height();

		int yoff = m_ScrollbarX->NotNecessary() ? 0 : xh;
		m_ScrollbarY->Position({ Width() - yw, yoff });
		m_ScrollbarY->Height(Height() - yoff);
		m_ScrollbarY->Range({ 0, m_Panel->Height() + yoff });
		m_ScrollbarY->VisibleRange(Height());
		m_ScrollbarY->Visible(!m_ScrollbarY->NotNecessary() && m_EnableY);

		int xoff = m_ScrollbarY->NotNecessary() ? 0 : yw;
		m_ScrollbarX->Position({ 0, 0 });
		m_ScrollbarX->Width(Width() - xoff);
		m_ScrollbarX->Range({ 0, m_Panel->Width() + xoff });
		m_ScrollbarX->VisibleRange(Width());
		m_ScrollbarX->Visible(!m_ScrollbarX->NotNecessary() && m_EnableX);

		int _x = min(-m_ScrollbarX->Value(), 0);
		int _y = Height() - m_Panel->Height() + m_ScrollbarY->Value();
		if (m_ScrollbarY->NotNecessary())
			_y = Height() - m_Panel->Height();
		m_Panel->Position({ _x, _y });

		// If the panel is not being autoresized, set the width/height
		// to the size of this scroll panel.
		if (!m_Panel->AutoResize().x)
			m_Panel->Width(Width() - xoff);

		if (!m_Panel->AutoResize().y)
			m_Panel->Height(Height());
		
		Panel::Update(viewport);
	}
}

void ScrollPanel::Render(CommandCollection& d) 
{
	using namespace Graphics;
	d.Command<Clip>(X(), Y(), Width(), Height());
	Background(d);
	d.Command<PushMatrix>();
	d.Command<Translate>(Vec2<int>{ X(), Y() });
	if (m_Panel)
	{
		m_Panel->Render(d);
		if (m_ScrollbarX->Visible())
			m_ScrollbarX->Render(d);
		if (m_ScrollbarY->Visible())
			m_ScrollbarY->Render(d);

		if (m_ScrollbarX->Visible() && m_ScrollbarY->Visible())
		{
			int yw = m_ScrollbarY->Width();
			int xh = m_ScrollbarX->Height();
			d.Command<Fill>(Color{ 55, 55, 55, 0 });
			d.Command<Quad>(Width() - yw, 0, yw, xh);
		}
	}
	d.Command<PopMatrix>();
	d.Command<ClearClip>();
}