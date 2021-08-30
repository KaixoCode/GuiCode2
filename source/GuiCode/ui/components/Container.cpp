#include "GuiCode/ui/components/Container.hpp"

Container::Container(Vec2<int> size)
	: Component(size)
{
	// General event listener to forward the events to the appropriate components
	// inside this container.
	m_Listener += [this](Event& e)
	{
		Event _copy = e;

		if (e.type == Event::Type::KeyPressed)
		{
			for (auto& _c : m_Components)
				_c->AddEvent(_copy);

			return;
		}

		// If there was an unfocus event, unfocus the focussed component
		if (m_Focussed && e.type == Event::Type::Unfocused)
		{
			m_Focussed->AddEvent(_copy);
			return;
		}

		if (e.type == Event::Type::MouseEntered)
		{
			if (m_Hovering) 
				m_Hovering->AddEvent(_copy);
			
			return;
		}

		// If the mouse moved or pressed, update the hovering and focussed
		if (e.type == Event::Type::MouseMoved || e.type == Event::Type::MousePressed)
			this->Determine(e);

		// Send events to hovering and focussed if it's not the same component.
		if (m_Hovering) m_Hovering->AddEvent(_copy);
		//if (m_Hovering != m_Focussed && m_Focussed) m_Focussed->AddEvent(_copy);

		// If the mouse is released update the hovering and focussed after sending the events.
		if (e.type == Event::Type::MouseReleased)
			this->Determine(e);
	};
}

void Container::Update(const Vec4<int>& viewport)
{
	// This just sets the m_Viewport to viewport.
	Component::Update(viewport);
	
	// Only update if the viewport has actual size.
	if (m_Viewport.width == 0 || m_Viewport.height == 0)
		return;
	
	// Update all the components that lie within the viewport and are visible.
	for (auto& _c : m_Components)
		if (_c->Visible() &&
			_c->X() + _c->Width() >= m_Viewport.x && _c->Y() + _c->Height() >= m_Viewport.y &&
			_c->X() <= m_Viewport.x + m_Viewport.width && _c->Y() <= m_Viewport.y + m_Viewport.height)
			_c->Update(viewport);
}

void Container::Render(CommandCollection& d)
{
	// Only render if the viewport has actual size.
	if (m_Viewport.width == 0 || m_Viewport.height == 0)
		return;

	// Render all the components that lie within the viewport and are visible.
	for (auto& _c : m_Components)
		if (_c->Visible() &&
			_c->X() + _c->Width() >= m_Viewport.x && _c->Y() + _c->Height() >= m_Viewport.y &&
			_c->X() <= m_Viewport.x + m_Viewport.width && _c->Y() <= m_Viewport.y + m_Viewport.height)
			_c->Render(d);
}

void Container::Determine(Event& e)
{
	// Determine the next hovering
	Component* _nextHover = 0;
	for (auto& _c : m_Components)
	{
		if (_c->Visible())
			if (_c->WithinBounds({ e.x, e.y }))
				_nextHover = _c.get();
	}

	// If it is different, send MouseEntered and MouseExited events.
	if (_nextHover != m_Hovering)
	{
		if (m_Hovering)
		{
			Event _e{ Event::Type::MouseExited, e.x, e.y };
			m_Hovering->AddEvent(_e);
		}

		if (_nextHover != nullptr)
		{
			Event _e{ Event::Type::MouseEntered, e.x, e.y };
			_nextHover->AddEvent(_e);
		}
		m_Hovering = _nextHover;
	}

	// If MousePressed and Hovering is not focussed, send focus events.
	if (e.type == Event::Type::MousePressed && m_Hovering != m_Focussed)
	{
		if (m_Focussed != nullptr)
		{
			Event _e{ Event::Type::Unfocused };
			m_Focussed->AddEvent(_e);
		}

		m_Focussed = m_Hovering;
		if (m_Focussed != nullptr)
		{
			Event _e{ Event::Type::Focused };
			m_Focussed->AddEvent(_e);
		}
	}
}