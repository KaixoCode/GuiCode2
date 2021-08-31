#include "GuiCode2/Component.hpp"
#include "GuiCode2/BasicEvents.hpp"

namespace GuiCode
{
	Component::Component()
		: listener{ m_Components }
	{
		State<Visible>(true);
	}

	void Component::CalculateOrder()
	{
		m_Components.sort([](Component* a, Component* b) { return a->zIndex > b->zIndex; });
	}

	void Component::RegisterComponent(Component& c)
	{
		m_Components.push_back(&c);
	}

	void Component::UnregisterComponent(Component& obj)
	{
		auto _it = std::find_if(m_Components.begin(), m_Components.end(), [&](auto& c) { return c == &obj; });
		if (_it != m_Components.end())
			m_Components.erase(_it);
	}

	void Component::ForwardRender(CommandCollection& d)
	{
		d.PushClip();
		d.PushMatrix();
		Render(d);
		if (State<UseDepth>())
			CalculateOrder();
		for (auto i = m_Components.rbegin(); i != m_Components.rend(); ++i)
			if ((*i)->State<Visible>())
				(*i)->ForwardRender(d);
		d.PopMatrix();
		d.PopClip();
	}

	void Component::ForwardUpdate()
	{
		Update();
		if (State<UseDepth>())
			CalculateOrder();
		for (auto i = m_Components.rbegin(); i != m_Components.rend(); ++i)
			if ((*i)->State<Visible>())
				(*i)->ForwardUpdate();
	}
}