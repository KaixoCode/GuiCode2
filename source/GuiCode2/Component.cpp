#include "GuiCode2/Component.hpp"
#include "GuiCode2/BasicEvents.hpp"

namespace GuiCode
{
	Dimensions& Dimensions::operator=(Dimensions&& other)
	{
		dimensions = other.dimensions;
		zIndex = other.zIndex;
		return *this;
	}

	Dimensions& Dimensions::operator=(const Dimensions& other)
	{
		dimensions = other.dimensions;
		zIndex = other.zIndex;
		return *this;
	}

	Component::Component()
		: listener{ components }
	{
		State<Visible>(true);
	}

	Component::Component(const Component& c)
		: components(c.components), listener(components),
		m_States(c.m_States)
	{}

	Component& Component::operator=(const Component& c)
	{
		components = c.components;
		m_States = c.m_States;
		return *this;
	}

	void Component::CalculateOrder()
	{
		components.sort([](Component* a, Component* b) { return a->zIndex > b->zIndex; });
	}

	void Component::ForwardRender(CommandCollection& d)
	{
		d.PushClip();
		d.PushMatrix();
		Render(d);
		CalculateOrder();
		for (auto i = components.rbegin(); i != components.rend(); ++i)
			if ((*i)->State<Visible>())
				(*i)->ForwardRender(d);
		d.PopMatrix();
		d.PopClip();
	}

	void Component::ConstrainSize()
	{
		if (min.width != -1 && width < min.width)
			width = min.width;
		if (max.width != -1 && width > max.width)
			width = max.width;
		if (min.height != -1 && height < min.height)
			height = min.height;
		if (max.height != -1 && height > max.height)
			height = max.height;
	}

	void Component::ForwardUpdate()
	{
		Update();
		ConstrainSize();
		for (auto i = components.rbegin(); i != components.rend(); ++i)
			if ((*i)->State<Visible>())
				(*i)->ForwardUpdate();
	}
}