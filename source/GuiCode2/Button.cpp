#include "GuiCode2/Button.hpp"

namespace GuiCode
{
	void Button::GroupBase::Unselect()
	{
		for (auto& i : group)
		{
			if (i->State<Selected>())
			{
				i->State<Selected>(false);
				i->settings.callback(false);
			}
		}
	}

	Button::Button(const Settings& settings)
		: settings(settings)
	{
		this->settings.group->group.push_back(this);
		Init();
	}

	Button::Button(const Button& other)
		: settings(other.settings)
	{
		settings.group->group.push_back(this);
		Init();
	}

	Button::Button(Button&& other)
		: settings(other.settings)
	{
		settings.group->group.push_back(this);
		Init();
	}

	Button::~Button()
	{
		std::remove(settings.group->group.begin(), settings.group->group.end(), this);
	}

	void Button::Init()
	{
		listener += [this](const MouseEnter& e)
		{
			if (!settings.callback || State<Disabled>())
				return;

			std::cout << "enter" << std::endl;

			if (settings.type == Hover)
			{
				State<Selected>(true);
				settings.callback(true);
			}
		};

		listener += [this](const MouseExit& e)
		{
			if (!settings.callback || State<Disabled>())
				return;

			std::cout << "exit" << std::endl;

			if (settings.type == Hover)
			{
				State<Selected>(false);
				settings.callback(false);
			}
		};

		listener += [this](const MouseRelease& e)
		{
			if (!settings.callback || !(State<Focused>() && State<Hovering>() && Hitbox(e.pos)) || State<Disabled>())
				return;

			if (settings.type == Click)
				settings.callback(true);

			if (settings.type == Radio)
			{
				settings.group->Unselect();
				State<Selected>(true);
				settings.callback(true);
			}

			if (settings.type == Toggle)
			{
				bool _selected = State<Selected>() ^ true;
				settings.group->Unselect();
				if (_selected)
				{
					State<Selected>(_selected);
					settings.callback(_selected);
				}
			}
		};

		listener += [this](const KeyPress& e)
		{
			if (!settings.callback || State<Disabled>())
				return;

			if (!e.repeat && (State<Hovering>() && e.keycode == Key::Enter || e == settings.combo))
			{
				if (settings.type == Click)
					settings.callback(true);

				if (settings.type == Radio)
				{
					settings.group->Unselect();
					State<Selected>(true);
					settings.callback(true);
				}

				if (settings.type == Toggle)
				{
					bool _selected = State<Selected>() ^ true;
					settings.group->Unselect();
					if (_selected)
					{
						State<Selected>(_selected);
						settings.callback(_selected);
					}
				}
				e.Handle();
			}
		};
	}
}