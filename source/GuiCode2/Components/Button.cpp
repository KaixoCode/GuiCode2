#include "GuiCode2/Components/Button.hpp"

namespace GuiCode
{
	void Button::GroupBase::Unselect() const
	{
		// Unselect all buttons in the group, if they are selected.
		for (auto& i : group)
			if (i->State(Selected))
			{
				i->State(Selected) = false;
				i->settings.callback(false); // Also call the callback!
			}
	}

	Button::Group::Group()
		: Pointer<GroupBase>(new GroupBase{})
	{}

	Button::Group::Group(const Group& other)
		: Pointer<GroupBase>(other)
	{
		Link(other.me);
	}

	Button::Group::Group(Group&& other)
		: Pointer<GroupBase>(std::move(other))
	{
		Link(other.me);
	}

	Button::Group& Button::Group::operator=(const Group& other)
	{
		Unlink(me); // When assigning we need to make sure we erase from previous group
		Pointer::operator=(other); 
		Link(me); // And then put it back into the new group.
		return *this;
	}

	Button::Group& Button::Group::operator=(Group&& other)
	{
		Unlink(me); // When assigning we need to make sure we erase from previous group
		Pointer::operator=(std::move(other));
		Link(me); // And then put it back into the new group.
		return *this;
	}

	void Button::Group::Link(Button* button)
	{
		me = button;
		auto _base = Get<GroupBase>();
		if (_base) 
			_base->group.push_back(button);
	}

	void Button::Group::Unlink(Button* button)
	{
		auto _base = Get<GroupBase>();
		if (_base)
		{
			auto _it = std::remove(_base->group.begin(), _base->group.end(), button);
			if (_it != _base->group.end())
				_base->group.erase(_it);
		}
	}

	Button::Button(const Settings& s)
		: settings(s)
	{
		InitListeners();
	}

	Button::Button(const Button& other) 
		: Component(other), settings(other.settings)
	{
		InitListeners();
	}

	Button::Button(Button&& other) 
		: Component(std::move(other)), settings(std::move(other.settings))
	{
		InitListeners();
	}

	Button::~Button()
	{
		settings.group.Unlink(this);
	}

	Button& Button::operator=(const Button& other)
	{
		settings = other.settings;
		return *this;
	}

	Button& Button::operator=(Button&& other) 
	{
		settings = other.settings;
		return *this;
	}

	void Button::Render(CommandCollection& d) const 
	{ 
		if (settings.graphics) 
			settings.graphics->Render(d); 
	}

	void Button::Update()
	{ 
		if (settings.graphics) 
			settings.graphics->Update();
	}

	void Button::InitListeners() 
	{
		if (settings.graphics)
		{
			settings.graphics->button = this;
			settings.graphics->Link(this);
		}
		settings.group.Link(this);

		*this += [this](const MouseEnter& e)
		{
			if (!settings.callback  // Ignore when no callback
				|| State(Disabled)) // Ignore when disabled
				return;

			if (settings.type == Hover
				&& !State(Hovering))    // To filter out Hovering state set by arrow keys.
			{                           // The Hovering State only gets set after this
				State(Selected) = true; // event normally, but not when done with arrow keys.
				settings.callback(true);
			}
		};

		*this += [this](const MouseExit& e)
		{
			if (!settings.callback  // Ignore when no callback
				|| State(Disabled)) // Ignore when disabled
				return;

			if (settings.type == Hover)   // Mouse Exit event is only handled
			{                             // by Hover Behaviour.
				State(Selected) = false;  // Set Selected to false
				settings.callback(false); // and call the callback
			}
		};

		*this += [this](const MouseRelease& e)
		{
			if (!settings.callback               // Ignore when no callback
				|| ~e.button & MouseButton::Left // Only when Left click
				|| !(State(Focused)              // Only if focused, 
					&& State(Hovering)		     // hovering, 
					&& Hitbox(e.pos))			 // and mouse is over the button
				|| State(Disabled))              // Ignore when disabled
				return;

			if (settings.type == Click)  // Simple click behaviour will call the
				settings.callback(true); // callback once, only when mouse released.

			if (settings.type == Radio
				&& !State(Selected))        // Radio button behavious will
			{                               // First unselect all buttons in the group
				settings.group->Unselect(); // And then selected this button
				State(Selected) = true;
				settings.callback(true);
			}

			if (settings.type == Toggle)
			{
				bool _selected = State(Selected) ^ true; // Toggle the current state
				settings.group->Unselect();              // Unselect any in the group
				if (_selected)                           // If the new state is 'on'
				{                                        // We will set the state
					State(Selected) = _selected;         // Otherwise the group Unselect
					settings.callback(_selected);        // will have turned it off.
				}
			}
		};

		*this += [this](const KeyPress& e)
		{
			if (!settings.callback  // Ignore if no callback is set
				|| State(Disabled)) // and if disabled
				return;

			if (!e.repeat                       // Don't react to repeated key press events
				&& (State(Hovering)             // Either trigger when hovering state and
					&& e.keycode == Key::Enter  // key is Enter
					|| e == settings.combo))    // Or when the assigned key combo was pressed.
			{
				if (settings.type == Click)
				{
					settings.callback(true);    // Click behaviour will simply call the callback.
					e.Handle();                 // And make sure to handle the event.
				}
				else if (settings.type == Radio // Radio behaviour only has change 
					&& !State(Selected))        // states if the current state is not Selected
				{
					settings.group->Unselect(); // Unselect the group
					State(Selected) = true;     // and then select this button
					settings.callback(true);
					e.Handle();                 // make sure to handle the event
				}
				else if (settings.type == Hover // Hover behaviour also can only
					&& !State(Selected))        // change the state to 'on' when Enter is pressed
				{
					State(Selected) = true;     // So set the Selected state to true
					settings.callback(true);    // and call the callback
					e.Handle();                 // Also handle the event
				}
				else if (settings.type == Toggle)
				{
					bool _selected = State(Selected) ^ true; // Toggle the current state
					settings.group->Unselect();              // Unselect group
					if (_selected)                           // Only change this button's
					{                                        // state if it's turning 'on'
						State(Selected) = _selected;         // because group unselect
						settings.callback(_selected);        // would have turned it off.
					}
					e.Handle();                              // Also handle the event.
				}
			}

			if (!e.repeat                 // Ignore repeated key press events
				&& settings.type == Hover // If type is hover, we can also react to
				&& State(Hovering))       // left and right arrow presses.
			{
				if (e.keycode == Key::Right // When right is pressed
					&& !State(Selected))    // and not selected
				{
					State(Selected) = true;  // Set Selected to true
					settings.callback(true); // call the callback
					e.Handle();              // And handle the event
				}
				else if (e.keycode == Key::Left // When left is pressed
					&& State(Selected))         // and selected
				{
					State(Selected) = false;    // Set Selected state to false
					settings.callback(false);   // call the callback
					e.Handle();                 // And handle the event
				}
			}
		};
	}
}