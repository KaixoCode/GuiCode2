#include "GuiCode2/Components/Menu.hpp"

namespace GuiCode
{
	Menu::Menu(const Settings& s)
		: settings(s)
	{
		InitListeners();
	}

	Menu::Menu(const Menu& other)
		: Component(other), settings(other.settings)
	{
		InitListeners();
	}

	Menu::Menu(Menu&& other) 
		: Component(std::move(other)), settings(std::move(other.settings))
	{
		InitListeners();
	}

	void Menu::InitListeners()
	{
		*this += [&](const KeyPress& e)
		{
			if (e.Handled() || !State(Focused))
				return;

			Component* _c = Get(Hovering);
			if ((settings.vertical && e.keycode == Key::Down) || (!settings.vertical && e.keycode == Key::Right))
			{
				if (_c && _c != this)
				{
					bool _found = false;
					for (auto& i : components)
					{
						if (_found && !i->State(Disabled))
						{
							_c->State(Hovering) = false;
							_c->HandleEvent(MouseExit{});
							i->State(Hovering) = true;
							i->HandleEvent(MouseEnter{});
							e.Handle();
							break;
						}

						if (i == _c)
							_found = true;
					}
				}
				else
				{
					(*components.begin())->State(Hovering) = true;
					(*components.begin())->HandleEvent(MouseEnter{});
					e.Handle();
				}
			}
			else if ((settings.vertical && e.keycode == Key::Up) || (!settings.vertical && e.keycode == Key::Left))
			{
				if (_c && _c != this)
				{
					Component* _prev = nullptr;
					for (auto& i : components)
					{
						if (i == _c)
						{
							if (_prev)
							{
								_c->State(Hovering) = false;
								_c->HandleEvent(MouseExit{});
								_prev->State(Hovering) = true;
								_prev->HandleEvent(MouseEnter{});
								e.Handle();
							}
							break;
						}

						if (!i->State(Disabled))
							_prev = i;
					}
				}
				else
				{
					(*(--components.end()))->State(Hovering) = true;
					(*(--components.end()))->HandleEvent(MouseEnter{});
					e.Handle();
				}
			}

			if (!e.Handled() && !e.repeat && e.keycode == Key::Left)
			{
				ContextMenu::Close(*this);
			}
		};

		*this += [&](const Unfocus& e)
		{
			Component* _c = Get(Hovering);
			if (_c)
			{
				_c->State(Hovering) = false;
				_c->HandleEvent(MouseExit{});
			}
		};
	}

	void Menu::ForwardUpdate() 
	{
		float _x = x + settings.margin.left;
		float _y = y + settings.margin.top;
		float _w = width - settings.margin.left - settings.margin.right;
		float _h = height - settings.margin.top - settings.margin.bottom;
		float _b = 0;
		if (settings.vertical)
		{
			for (auto& i : components)
			{
				i->dimensions = { _x, _y, _w, i->height };
				_y += i->height + settings.padding * 2;

				if (i->width > _b)
					_b = i->width;
			}

			height = _y + settings.margin.bottom;
			width = _b;
		}
		else 
		{
			for (auto& i : components)
			{
				i->dimensions = { _x, _y, i->width, _h };
				_x += i->width + settings.padding * 2;
				if (i->height > _b)
					_b = i->height;
			}

			width = _x + settings.margin.right;
			height = _b;
		}

		Component::ForwardUpdate();
	}

	bool Menu::Hitbox(const Vec2<float>& pos) const
	{
		if (Component::Hitbox(pos))
			return true;

		// Make sure to include menu component hitboxes
		for (auto& i : components)
			if (i->Hitbox(pos))
				return true;

		return false;
	}

	SubMenuButton::SubMenuButton(const Settings& settings)
		: Button(Button::Settings{
			.group = settings.group,
			.type = Button::Hover,
			.callback = [this](bool v) {
				if (v) ContextMenu::Open(menu, { x + width, y });
				else ContextMenu::Close(menu);
			},
			.combo = settings.combo,
			.graphics = settings.graphics,
		})
	{}

	bool SubMenuButton::Hitbox(const Vec2<float>& pos) const
	{
		return Button::Hitbox(pos) || menu.Hitbox(pos - Vec2<float>{ x + width, y });
	}

	Divider::Divider(const Settings& settings)
		: settings(settings)
	{
		State(Disabled) = true;
	}

	void Divider::Update()
	{
		height = settings.padding.top + settings.padding.bottom + settings.stroke;
		width = settings.padding.left + settings.padding.right + settings.stroke;
	}

	void Divider::Render(CommandCollection& d) const
	{
		d.Fill(settings.color);
		d.Quad({ x + settings.padding.left, y + settings.padding.top, width - settings.padding.left - settings.padding.right, settings.stroke });
	}
}