#include "GuiCode2/Components/Component.hpp"
#include "GuiCode2/Events/BasicEvents.hpp"

namespace GuiCode
{
	Component::Component()
	{
		State(Visible) = true;
		InitListeners();
	}

	Component::Component(const Component& other)
		: EventListener(other), cursor(other.cursor)
	{
		State(Visible) = true;
		InitListeners();
	}

	Component::Component(Component&& other)
		: EventListener(std::move(other)), cursor(std::move(other.cursor))
	{
		State(Visible) = true;
		InitListeners();
	}

	Component& Component::operator=(const Component& other)
	{
		components = other.components;
		cursor = other.cursor;
		return *this;
	}

	Component& Component::operator=(Component&& other)
	{
		components = std::move(other.components);
		cursor = std::move(other.cursor);
		return *this;
	}

	void Component::InitListeners()
	{
		// Hovering state has limit of 1. When MouseMoved event occurs it will check hovering.
		// if there is a change in state it will also send out the MouseEnter/MouseExit event
		// accordingly. MouseEnter/MouseExit also update the state, which is crucial for 
		// sub-components.
		StateHandler({ .state = Hovering, .limit = 1 })
			.Add([this](const MouseMove& e, Component& c, int first) -> int
			{
				bool prev = c.State(Hovering);
				bool curr = c.Hitbox(e.pos) && first; // first == 1 if no other component Hovering.

				if (!prev && curr) c.HandleEvent(MouseEnter{});
				else if (prev && !curr) c.HandleEvent(MouseExit{});

				return curr;
			})
			.Add([](const MouseExit& e, Component& c, int) -> int { return false; });

		// The focused state also has a limit of 1, and is triggered by a MousePress event
		// If there is a state change it will also send out the Focus/Unfocus events accordingly.
		StateHandler({.state = Focused, .limit = 1 })
			.Add([](const MousePress& e, Component& c, int first) -> int
			{
				bool prev = c.State(Focused);
				bool curr = c.State(Hovering) && first;

				if (!prev && curr) c.HandleEvent(Focus{});
				else if (prev && !curr) c.HandleEvent(Unfocus{});

				return curr;
			})
			.Add([](const Unfocus& e, Component& c, int) -> int { return false; });

		// Pressed also has limit of 1, and is handled simply by the MousePress and MouseRelease
		StateHandler({.state = Pressed, .limit = 1 })
			.Add([](const MousePress& e, Component& c, int) -> int { return c.State(Hovering) ? c.State(Pressed) | e.button : 0; })
			.Add([](const MouseRelease& e, Component& c, int) -> int { return c.State(Pressed) & ~e.button; });
	}

	Pointer<Component> Component::Get(GuiCode::State state)
	{
		for (auto& i : components)
		{
			if (auto _c = i->Get(state))
				return _c;
		
			if (i->State(state))
				return i;
		}

		return {};
	}

	void Component::ForwardRender(CommandCollection& d)
	{
		d.PushClip();
		Render(d);
		std::sort(components.begin(), components.end(), [](auto& a, auto& b) { return a->zIndex < b->zIndex; });

		for (auto _c : components)
			if (_c->State(Visible))
				_c->ForwardRender(d);
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
		for (auto& _c : components)
			if (_c->State(Visible))
				_c->ForwardUpdate();
	}
}