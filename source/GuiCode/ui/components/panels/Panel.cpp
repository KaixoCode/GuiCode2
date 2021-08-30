#include "GuiCode/ui/components/panels/Panel.hpp"

// --------------------------------------------------------------------------
// -------------------------------- Panel -----------------------------------
// --------------------------------------------------------------------------

Panel::Panel(Vec2<int> size)
	: Container(size)
{
	m_Listener += [this](Event& e)
	{
		m_LayoutManager.AddEvent(e);

		// Make event coords relative to this panel
		if (e.type != Event::Type::KeyPressed && e.type != Event::Type::KeyReleased)
			e.x -= X(), e.y -= Y();
	};

	// TODO: Find a better fix than just updating when mousepressed....
	// Hacky fix to the TreePanel Folder expanding bug...
	m_Listener += [this](Event::MousePressed& e)
	{
		Container::Update(m_Viewport);
	};
}

void Panel::Background(CommandCollection& d)
{
	using namespace Graphics;
	d.Command<Fill>(m_Background);
	d.Command<Quad>(X(), Y(), Width(), Height());
};

void Panel::Update(const Vec4<int>& viewport)
{
	// Step 1 is updating the layout to make sure the positions of all components are correct.
	m_LayoutManager.Update({ 0, 0, Width(), Height() }, m_Components); // Also set the cursor
	m_Cursor = m_LayoutManager.Cursor() == -1 ? GLFW_CURSOR_NORMAL : m_LayoutManager.Cursor();
	// Next: calculate the highest coords and auto resize to those coords if that is enabled.
	if (m_AutoResizeX)
		Width(m_LayoutManager.BiggestCoords().x);

	if (m_AutoResizeY)
		Height(m_LayoutManager.BiggestCoords().y);

	// Then update all the components in this container with the right viewport
	// make sure to overlap the viewport with the coords of this panel, and then
	// since all components in a panel are drawn relative from it, translate.
	Container::Update(viewport.Overlap({ X(), Y(), Width(), Height() }).Translate({ X(), Y() }));
}

void Panel::Render(CommandCollection& d)
{
	using namespace Graphics;
	Background(d);
	d.Command<PushMatrix>();
	d.Command<Translate>(Vec2<int>{ X(), Y() });
	Container::Render(d);
	d.Command<PopMatrix>();
}