#include "GuiCode2/Button.hpp"

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

	Button::Group& Button::Group::operator=(const Group& other)
	{
		// When assigning we need to make sure we erase from previous group
		if (me)
			me->settings.group->group.erase(std::remove(me->settings.group->group.begin(), me->settings.group->group.end(), me));
		Pointer::operator=(other);

		// And then put it back into the new group.
		if (me)
			me->settings.group->group.push_back(me);
		return *this;
	}

	Button::Button(const Settings& settings)
		: settings(settings)
	{
		// Add this button to its group.
		this->settings.group.me = this;
		this->settings.group->group.push_back(this);
		Init();
	}

	Button::Button(Button&& other)
		: settings(other.settings)
	{
		// Add this button to its group.
		settings.group.me = this;
		settings.group->group.push_back(this);
		Init();
	}

	Button& Button::operator=(Button&& other) 
	{
		// Remove this from current group, and assign new group
		settings.group->group.erase(std::remove(settings.group->group.begin(), settings.group->group.end(), this));
		settings = std::move(other.settings); 

		// Remove moved one from their group and add this
		settings.group->group.erase(std::remove(settings.group->group.begin(), settings.group->group.end(), &other));
		settings.group->group.push_back(this);
		Component::operator=(std::move(other));
		return *this; 
	}

	Button::~Button()
	{
		// Remove this button from its group.
		if (settings.group)
			settings.group->group.erase(std::remove(settings.group->group.begin(), settings.group->group.end(), this));
	}

	void Button::Init()
	{
		listener += [this](const MouseEnter& e)
		{
			if (!settings.callback    // Ignore when no callback
				|| State(Disabled)) // Ignore when disabled
				return;

			if (settings.type == Hover
				&& !State(Hovering)) // To filter out Hovering state set by arrow keys.
			{                          // The Hovering State only gets set after this
				State(Selected) = true; // event normally, but not when done with arrow keys.
				settings.callback(true);
			}
		};

		listener += [this](const MouseExit& e)
		{
			if (!settings.callback    // Ignore when no callback
				|| State(Disabled)) // Ignore when disabled
				return;

			if (settings.type == Hover)   // Mouse Exit event is only handled
			{                             // by Hover Behaviour.
				State(Selected) = false;   // Set Selected to false
				settings.callback(false); // and call the callback
			}
		};

		listener += [this](const MouseRelease& e)
		{
			if (!settings.callback                // Ignore when no callback
				|| ~e.button & MouseButton::Left  // Only when Left click
				|| !(State(Focused)             // Only if focused, 
					&& State(Hovering)		  // hovering, 
					&& Hitbox(e.pos))			  // and mouse is over the button
				|| State(Disabled))             // Ignore when disabled
				return;

			if (settings.type == Click)  // Simple click behaviour will call the
				settings.callback(true); // callback once, only when mouse released.

			if (settings.type == Radio
				&& !State(Selected))      // Radio button behavious will
			{                               // First unselect all buttons in the group
				settings.group->Unselect(); // And then selected this button
				State(Selected) = true;
				settings.callback(true);
			}

			if (settings.type == Toggle) 
			{
				bool _selected = State(Selected) ^ true; // Toggle the current state
				settings.group->Unselect();                // Unselect any in the group
				if (_selected)                             // If the new state is 'on'
				{                                          // We will set the state
					State(Selected) = _selected;            // Otherwise the group Unselect
					settings.callback(_selected);          // will have turned it off.
				}
			}
		};

		listener += [this](const KeyPress& e)
		{
			if (!settings.callback    // Ignore if no callback is set
				|| State(Disabled)) // and if disabled
				return;

			if (!e.repeat                       // Don't react to repeated key press events
				&& (State(Hovering)           // Either trigger when hovering state and
					&& e.keycode == Key::Enter  // key is Enter
					|| e == settings.combo))    // Or when the assigned key combo was pressed.
			{
				if (settings.type == Click)
				{
					settings.callback(true);    // Click behaviour will simply call the callback.
					e.Handle();                 // And make sure to handle the event.
				}
				else if (settings.type == Radio // Radio behaviour only has change 
					&& !State(Selected))      // states if the current state is not Selected
				{
					settings.group->Unselect(); // Unselect the group
					State(Selected) = true;      // and then select this button
					settings.callback(true);
					e.Handle();                 // make sure to handle the event
				}
				else if (settings.type == Hover // Hover behaviour also can only
					&& !State(Selected))      // change the state to 'on' when Enter is pressed
				{
					State(Selected) = true;      // So set the Selected state to true
					settings.callback(true);    // and call the callback
					e.Handle();                 // Also handle the event
				}
				else if (settings.type == Toggle)
				{
					bool _selected = State(Selected) ^ true; // Toggle the current state
					settings.group->Unselect();                // Unselect group
					if (_selected)                             // Only change this button's
					{                                          // state if it's turning 'on'
 						State(Selected) = _selected;            // because group unselect
						settings.callback(_selected);          // would have turned it off.
					}
					e.Handle();                                // Also handle the event.
				}
			}

			if (!e.repeat                 // Ignore repeated key press events
				&& settings.type == Hover // If type is hover, we can also react to
				&& State(Hovering))     // left and right arrow presses.
			{
				if (e.keycode == Key::Right // When right is pressed
					&& !State(Selected))  // and not selected
				{
					State(Selected) = true;   // Set Selected to true
					settings.callback(true); // call the callback
					e.Handle();              // And handle the event
				}
				else if (e.keycode == Key::Left // When left is pressed
					&& State(Selected))       // and selected
				{
					State(Selected) = false;     // Set Selected state to false
					settings.callback(false);   // call the callback
					e.Handle();                 // And handle the event
				}
			}
		};
	}

	/**
	 * Parsers
	 */

	template<>
	Button::Type Parsers<Button::Type>::Parse(std::string_view& c)
	{
		return (Button::Type)Parsers<int>::Parse(c);
	};

	template<>
	Key Parsers<Key>::Parse(std::string_view& c)
	{
		return (Key::Combo)Parsers<int>::Parse(c);
	};

	template<>
	Button::Group Parsers<Button::Group>::Parse(std::string_view& c)
	{
		// Remove spaces
		c = c.substr(c.find_first_not_of(" "));
		std::string _res = std::string{ c.substr(0, c.find_last_not_of(" ") + 1) };
		
		if (ButtonParser::buttonGroupMap.contains(_res))
			return ButtonParser::buttonGroupMap.at(_res);

		else
			return ButtonParser::buttonGroupMap.emplace(_res, Button::Group{}).first->second;
	};

	NormalButton::NormalButton(const Settings& settings)
		: Button({ settings.group, settings.type, settings.callback, settings.combo })
	{
		this->settings = settings;
		Init();
	}

	NormalButton::NormalButton(NormalButton&& other)
		: Button({ other.settings.group, other.settings.type, other.settings.callback, other.settings.combo })
	{
		settings = other.settings;
		Init();
	}

	NormalButton::NormalButton(const NormalButton& other)
		: Button({ other.settings.group, other.settings.type, other.settings.callback, other.settings.combo })
	{
		settings = other.settings;
		Init();
	}

	NormalButton& NormalButton::operator=(NormalButton&& other)
	{
		settings = std::move(other.settings);
		Button::operator=(std::move(other));
		return *this;
	}

	void NormalButton::Init()
	{
		height = 20;
		settings.Link(this);
	}

	void NormalButton::Update()
	{
		// Set the minimum width of this menu button to fit all text.
		float _minWidth = 0;
		_minWidth += GraphicsBase::StringWidth(settings.name, settings.font, settings.text.size);
		_minWidth += GraphicsBase::StringWidth(Button::settings.combo.ToString(), settings.font, settings.text.size);
		_minWidth += height + 36;
		min.width = _minWidth;
	}

	void NormalButton::Render(CommandCollection& d) const
	{
		d.Fill(settings.border.color.Current());
		d.Quad(dimensions);

		d.Fill(settings.color.Current());
		d.Quad({ x + settings.border.width, y + settings.border.width, width - 2 * settings.border.width, height - 2 * settings.border.width });

		d.Fill(settings.text.color.Current());
		d.TextAlign(Align::Center);
		d.FontSize(settings.text.size);
		d.Font(settings.font);
		d.Text(settings.name, { x + width / 2, y + height / 2 });
	}

	ButtonParser::ButtonParser()
	{
		settings.name = "";
		Attribute("type", &Button::m_Type);
		Attribute("callback", &Button::m_Callback);
		Attribute("group", &Button::m_Group);
		Attribute("combo", &Button::m_Combo);

		enumMap["Click"] = Button::Click;
		enumMap["Toggle"] = Button::Toggle;
		enumMap["Hover"] = Button::Hover;
		enumMap["Radio"] = Button::Radio;

		enumMap["CTRL_A"] = Key::CTRL_A; enumMap["CTRL_SHIFT_A"] = Key::CTRL_SHIFT_A; enumMap["CTRL_ALT_A"] = Key::CTRL_ALT_A;
		enumMap["CTRL_B"] = Key::CTRL_B; enumMap["CTRL_SHIFT_B"] = Key::CTRL_SHIFT_B; enumMap["CTRL_ALT_B"] = Key::CTRL_ALT_B;
		enumMap["CTRL_C"] = Key::CTRL_C; enumMap["CTRL_SHIFT_C"] = Key::CTRL_SHIFT_C; enumMap["CTRL_ALT_C"] = Key::CTRL_ALT_C;
		enumMap["CTRL_D"] = Key::CTRL_D; enumMap["CTRL_SHIFT_D"] = Key::CTRL_SHIFT_D; enumMap["CTRL_ALT_D"] = Key::CTRL_ALT_D;
		enumMap["CTRL_E"] = Key::CTRL_E; enumMap["CTRL_SHIFT_E"] = Key::CTRL_SHIFT_E; enumMap["CTRL_ALT_E"] = Key::CTRL_ALT_E;
		enumMap["CTRL_F"] = Key::CTRL_F; enumMap["CTRL_SHIFT_F"] = Key::CTRL_SHIFT_F; enumMap["CTRL_ALT_F"] = Key::CTRL_ALT_F;
		enumMap["CTRL_G"] = Key::CTRL_G; enumMap["CTRL_SHIFT_G"] = Key::CTRL_SHIFT_G; enumMap["CTRL_ALT_G"] = Key::CTRL_ALT_G;
		enumMap["CTRL_H"] = Key::CTRL_H; enumMap["CTRL_SHIFT_H"] = Key::CTRL_SHIFT_H; enumMap["CTRL_ALT_H"] = Key::CTRL_ALT_H;
		enumMap["CTRL_I"] = Key::CTRL_I; enumMap["CTRL_SHIFT_I"] = Key::CTRL_SHIFT_I; enumMap["CTRL_ALT_I"] = Key::CTRL_ALT_I;
		enumMap["CTRL_J"] = Key::CTRL_J; enumMap["CTRL_SHIFT_J"] = Key::CTRL_SHIFT_J; enumMap["CTRL_ALT_J"] = Key::CTRL_ALT_J;
		enumMap["CTRL_K"] = Key::CTRL_K; enumMap["CTRL_SHIFT_K"] = Key::CTRL_SHIFT_K; enumMap["CTRL_ALT_K"] = Key::CTRL_ALT_K;
		enumMap["CTRL_L"] = Key::CTRL_L; enumMap["CTRL_SHIFT_L"] = Key::CTRL_SHIFT_L; enumMap["CTRL_ALT_L"] = Key::CTRL_ALT_L;
		enumMap["CTRL_M"] = Key::CTRL_M; enumMap["CTRL_SHIFT_M"] = Key::CTRL_SHIFT_M; enumMap["CTRL_ALT_M"] = Key::CTRL_ALT_M;
		enumMap["CTRL_N"] = Key::CTRL_N; enumMap["CTRL_SHIFT_N"] = Key::CTRL_SHIFT_N; enumMap["CTRL_ALT_N"] = Key::CTRL_ALT_N;
		enumMap["CTRL_O"] = Key::CTRL_O; enumMap["CTRL_SHIFT_O"] = Key::CTRL_SHIFT_O; enumMap["CTRL_ALT_O"] = Key::CTRL_ALT_O;
		enumMap["CTRL_P"] = Key::CTRL_P; enumMap["CTRL_SHIFT_P"] = Key::CTRL_SHIFT_P; enumMap["CTRL_ALT_P"] = Key::CTRL_ALT_P;
		enumMap["CTRL_Q"] = Key::CTRL_Q; enumMap["CTRL_SHIFT_Q"] = Key::CTRL_SHIFT_Q; enumMap["CTRL_ALT_Q"] = Key::CTRL_ALT_Q;
		enumMap["CTRL_R"] = Key::CTRL_R; enumMap["CTRL_SHIFT_R"] = Key::CTRL_SHIFT_R; enumMap["CTRL_ALT_R"] = Key::CTRL_ALT_R;
		enumMap["CTRL_S"] = Key::CTRL_S; enumMap["CTRL_SHIFT_S"] = Key::CTRL_SHIFT_S; enumMap["CTRL_ALT_S"] = Key::CTRL_ALT_S;
		enumMap["CTRL_T"] = Key::CTRL_T; enumMap["CTRL_SHIFT_T"] = Key::CTRL_SHIFT_T; enumMap["CTRL_ALT_T"] = Key::CTRL_ALT_T;
		enumMap["CTRL_U"] = Key::CTRL_U; enumMap["CTRL_SHIFT_U"] = Key::CTRL_SHIFT_U; enumMap["CTRL_ALT_U"] = Key::CTRL_ALT_U;
		enumMap["CTRL_V"] = Key::CTRL_V; enumMap["CTRL_SHIFT_V"] = Key::CTRL_SHIFT_V; enumMap["CTRL_ALT_V"] = Key::CTRL_ALT_V;
		enumMap["CTRL_W"] = Key::CTRL_W; enumMap["CTRL_SHIFT_W"] = Key::CTRL_SHIFT_W; enumMap["CTRL_ALT_W"] = Key::CTRL_ALT_W;
		enumMap["CTRL_X"] = Key::CTRL_X; enumMap["CTRL_SHIFT_X"] = Key::CTRL_SHIFT_X; enumMap["CTRL_ALT_X"] = Key::CTRL_ALT_X;
		enumMap["CTRL_Y"] = Key::CTRL_Y; enumMap["CTRL_SHIFT_Y"] = Key::CTRL_SHIFT_Y; enumMap["CTRL_ALT_Y"] = Key::CTRL_ALT_Y;
		enumMap["CTRL_Z"] = Key::CTRL_Z; enumMap["CTRL_SHIFT_Z"] = Key::CTRL_SHIFT_Z; enumMap["CTRL_ALT_Z"] = Key::CTRL_ALT_Z;
		enumMap["CTRL_1"] = Key::CTRL_1; enumMap["CTRL_SHIFT_1"] = Key::CTRL_SHIFT_1; enumMap["CTRL_ALT_1"] = Key::CTRL_ALT_1;
		enumMap["CTRL_2"] = Key::CTRL_2; enumMap["CTRL_SHIFT_2"] = Key::CTRL_SHIFT_2; enumMap["CTRL_ALT_2"] = Key::CTRL_ALT_2;
		enumMap["CTRL_3"] = Key::CTRL_3; enumMap["CTRL_SHIFT_3"] = Key::CTRL_SHIFT_3; enumMap["CTRL_ALT_3"] = Key::CTRL_ALT_3;
		enumMap["CTRL_4"] = Key::CTRL_4; enumMap["CTRL_SHIFT_4"] = Key::CTRL_SHIFT_4; enumMap["CTRL_ALT_4"] = Key::CTRL_ALT_4;
		enumMap["CTRL_5"] = Key::CTRL_5; enumMap["CTRL_SHIFT_5"] = Key::CTRL_SHIFT_5; enumMap["CTRL_ALT_5"] = Key::CTRL_ALT_5;
		enumMap["CTRL_6"] = Key::CTRL_6; enumMap["CTRL_SHIFT_6"] = Key::CTRL_SHIFT_6; enumMap["CTRL_ALT_6"] = Key::CTRL_ALT_6;
		enumMap["CTRL_7"] = Key::CTRL_7; enumMap["CTRL_SHIFT_7"] = Key::CTRL_SHIFT_7; enumMap["CTRL_ALT_7"] = Key::CTRL_ALT_7;
		enumMap["CTRL_8"] = Key::CTRL_8; enumMap["CTRL_SHIFT_8"] = Key::CTRL_SHIFT_8; enumMap["CTRL_ALT_8"] = Key::CTRL_ALT_8;
		enumMap["CTRL_9"] = Key::CTRL_9; enumMap["CTRL_SHIFT_9"] = Key::CTRL_SHIFT_9; enumMap["CTRL_ALT_9"] = Key::CTRL_ALT_9;
		enumMap["CTRL_0"] = Key::CTRL_0; enumMap["CTRL_SHIFT_0"] = Key::CTRL_SHIFT_0; enumMap["CTRL_ALT_0"] = Key::CTRL_ALT_0;

		enumMap["CTRL_SPACE"] = Key::CTRL_SPACE;
		enumMap["CTRL_COMMA"] = Key::CTRL_COMMA;
		enumMap["ALT_F4"] = Key::ALT_F4;
		enumMap["CTRL_F4"] = Key::CTRL_F4;
		enumMap["SHIFT_TAB"] = Key::SHIFT_TAB;
		enumMap["F11"] = Key::F11;
	}

	NormalButtonParser::NormalButtonParser()
	{
		Parser::Link<ButtonParser>();
		settings.name = "button";
		Attribute("name", &NormalButton::m_Name);
		Attribute("color", &NormalButton::m_Color);
		Attribute("color.transition", &StateColors::transition);
		Attribute("border-color", &NormalButton::m_BorderColor);
		Attribute("border-color.transition", &StateColors::transition);
		Attribute("border-width", &NormalButton::m_BorderWidth);
		Attribute("font-size", &NormalButton::m_TextSize);
		Attribute("text-color", &NormalButton::m_TextColor);
		Attribute("font", &NormalButton::m_Font);
	}

	Pointer<Component> ButtonParser::Create() 
	{
		return new Button{};
	}

	Pointer<Component> NormalButtonParser::Create()
	{
		return new NormalButton{};
	}
}