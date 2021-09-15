#include "GuiCode2/Component.hpp"
#include "GuiCode2/BasicEvents.hpp"

namespace GuiCode
{
	Component::Component()
		: listener{ components }
	{
		State<Visible>(true);
		InitListeners();
		Init();
	}

	void Component::InitListeners()
	{
		// Hovering state has limit of 1. When MouseMoved event occurs it will check hovering.
		// if there is a change in state it will also send out the MouseEnter/MouseExit event
		// accordingly. MouseEnter/MouseExit also update the state, which is crucial for 
		// sub-components.
		(listener.State<Hovering>({ .limit = 1 })
			+= [this](const MouseMove& e, Component& c, int first) -> int
			{
				bool prev = c.State<Hovering>();
				bool curr = c.Hitbox(e.pos) && first; // first == 1 if no other component Hovering.

				if (!prev && curr) c.listener(MouseEnter{});
				else if (prev && !curr) c.listener(MouseExit{});

				return curr;
			})
			+= [](const MouseExit& e, Component& c, int) -> int { return false; };

		// The focused state also has a limit of 1, and is triggered by a MousePress event
		// If there is a state change it will also send out the Focus/Unfocus events accordingly.
		(listener.State<Focused>({ .limit = 1 }) 
			+= [](const MousePress& e, Component& c, int first) -> int
			{
				bool prev = c.State<Focused>();
				bool curr = c.State<Hovering>() && first;

				if (!prev && curr) c.listener(Focus{});
				else if (prev && !curr) c.listener(Unfocus{});

				return curr;
			})
			+= [](const Unfocus& e, Component& c, int) -> int { return false; };

		// Pressed also has limit of 1, and is handled simply by the MousePress and MouseRelease
		(listener.State<Pressed>({ .limit = 1 })
			+= [](const MousePress& e, Component& c, int) -> int { return c.State<Hovering>() ? c.State<Pressed>() | e.button : 0; })
			+= [](const MouseRelease& e, Component& c, int) -> int { return c.State<Pressed>() & ~e.button; };
	}

	void Component::CalculateOrder()
	{
		components.sort([](Component& a, Component& b) { return a.zIndex > b.zIndex; });
	}

	Component* Component::Get(int state)
	{
		for (auto& i : components)
			if (auto _c = i->Get(state))
				return _c;

		if (State(state))
			return this;

		return nullptr;
	}

	void Component::ForwardRender(CommandCollection& d)
	{
		d.PushClip();
		Render(d);
		CalculateOrder();
		for (auto i = components.rbegin(); i != components.rend(); ++i)
			if ((*i)->State<Visible>()) // If visible, and within bounding box, render
				if ((*i)->BoundingBox().Overlaps(BoundingBox()))
					(*i)->ForwardRender(d);
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