#include "GuiCode2/Menu.hpp"

namespace GuiCode
{
	Menu::Menu(bool vertical)
		: vertical(vertical),
		border(settings.border),
		background(settings.background),
		padding(settings.padding)
	{
		Init();
		background = { 26, 26, 26, 255 };
		padding = { 4, 4, 4, 4 };
		border.width = 1;
		border.color = { 64, 64, 64, 255 };
	}

	Menu::Menu(Menu&& other)
		: vertical(other.vertical),
		border(settings.border),
		background(settings.background),
		padding(settings.padding)
	{
		Init();
		background = other.background;
		padding = other.padding;
		border = other.border;
	}

	Menu::Menu(const Menu& other)
		: vertical(other.vertical),
		border(settings.border),
		background(settings.background),
		padding(settings.padding)
	{
		Init();
		background = other.background;
		padding = other.padding;
		border = other.border;
	}

	void Menu::Clear()
	{
		panels.Clear();
	}

	void Menu::Update()
	{
		float _min = 136;
		for (auto& c : components)
		{
			if (c->min.width > _min)
				_min = c->min.width;
		}

		if (vertical)
		{
			min.width = _min + (scrollbar.y.Necessary() ? scrollbar.y.width : 0) + padding.left + padding.right;
			min.height = 39 + padding.top + padding.bottom + (scrollbar.x.Necessary() ? scrollbar.x.height : 0);
		}
		ConstrainSize();
	}

	bool Menu::Hitbox(const Vec2<float>& pos) const
	{
		if (Panel::Hitbox(pos))
			return true;

		// Make sure to include menu component hitboxes
		for (auto& i : panels)
			if (i.component->Hitbox(pos))
				return true;

		return false;
	}

	void Menu::Init()
	{
		settings.max.height = 500;
		settings.max.width = 500;
		settings.ratio = 1;
		settings.layout = vertical ? Layout::Column : Layout::Row;
		settings.overflow = Overflow::Scroll;
		settings.size.width = vertical ? Inherit : Auto;
		settings.size.height = vertical ? Auto : Inherit;
		listener += [&](const KeyPress& e)
		{
			if (e.Handled() || !State<Focused>())
				return;

			Component* _c = Get(Hovering);
			if ((vertical && e.keycode == Key::Down) || (!vertical && e.keycode == Key::Right))
			{
				if (_c && _c != this)
				{
					bool _found = false;
					for (auto& i : panels)
					{
						if (_found && !i.component->State<Disabled>())
						{
							_c->State<Hovering>(false);
							_c->listener(MouseExit{});
							i.component->State<Hovering>(true);
							i.component->listener(MouseEnter{});
							e.Handle();
							break;
						}

						if (i.component == _c)
							_found = true;
					}
				}
				else
				{
					panels.begin()->component->State<Hovering>(true);
					panels.begin()->component->listener(MouseEnter{});
					e.Handle();
				}
			}
			else if ((vertical && e.keycode == Key::Up) || (!vertical && e.keycode == Key::Left))
			{
				if (_c && _c != this)
				{
					Component* _prev = nullptr;
					for (auto& i : panels)
					{
						if (i.component == _c)
						{
							if (_prev)
							{
								_c->State<Hovering>(false);
								_c->listener(MouseExit{});
								_prev->State<Hovering>(true);
								_prev->listener(MouseEnter{});
								e.Handle();
							}
							break;
						}

						if (!i.component->State<Disabled>())
							_prev = i.component;
					}
				}
				else
				{
					(--panels.end())->component->State<Hovering>(true);
					(--panels.end())->component->listener(MouseEnter{});
					e.Handle();
				}
			}

			if (!e.Handled() && !e.repeat && e.keycode == Key::Left)
			{
				ContextMenu::Close(*this);
			}
		};

		listener += [&](const Unfocus& e)
		{
			Component* _c = Get(Hovering);
			if (_c)
			{
				_c->State<Hovering>(false);
				_c->listener(MouseExit{});
			}
		};
	}

	MenuButton::MenuButton(const Settings& settings)
		: Button({ settings.group, settings.type, settings.callback, settings.combo })
	{
		this->settings = settings;
		Init();
	}

	MenuButton::MenuButton(MenuButton&& other)
		: Button({ other.settings.group, other.settings.type, other.settings.callback, other.settings.combo })
	{
		settings = other.settings;
		Init();
	}

	MenuButton::MenuButton(const MenuButton& other)
		: Button({ other.settings.group, other.settings.type, other.settings.callback, other.settings.combo })
	{
		settings = other.settings;
		Init();
	}

	void MenuButton::Init()
	{
		height = 20;
		settings.Link(this);
	}

	void MenuButton::Update()
	{
		// Set the minimum width of this menu button to fit all text.
		float _minWidth = 0;
		_minWidth += GraphicsBase::StringWidth(settings.name, settings.font, settings.text.size);
		_minWidth += GraphicsBase::StringWidth(settings.combo.ToString(), settings.font, settings.text.size);
		_minWidth += height + 36;
		min.width = _minWidth;
	}

	void MenuButton::Render(CommandCollection& d) const
	{
		d.Fill(settings.border.color.Current());
		d.Quad(dimensions);

		d.Fill(settings.color.Current());
		d.Quad({ x + settings.border.width, y + settings.border.width, width - 2 * settings.border.width, height - 2 * settings.border.width });

		if (State<Selected>())
		{
			d.Fill(settings.select.Current());
			d.Quad({ x + 3, y + 3, height - 6, height - 6 });
		}

		d.Fill(settings.text.color.Current());
		d.TextAlign(Align::Middle | Align::Left);
		d.FontSize(settings.text.size);
		d.Font(settings.font);
		d.Text(settings.name, { x + height + 3, y + height / 2 });
		if (settings.combo)
		{
			d.TextAlign(Align::Middle | Align::Right);
			d.Text(settings.combo.ToString(), { x + width - 9, y + height / 2 });
		}
	}

	void SubMenuButton::Init()
	{
		Button::settings.type = Hover;
		Button::settings.callback = [this](bool v)
		{
			if (v)
				ContextMenu::Open(menu, { x + width, y });

			else
				ContextMenu::Close(menu);
		};
	}

	void SubMenuButton::Render(CommandCollection& d) const
	{
		d.Fill(settings.border.color.Current());
		d.Quad(dimensions);

		d.Fill(settings.color.Current());
		d.Quad({ x + settings.border.width, y + settings.border.width, width - 2 * settings.border.width, height - 2 * settings.border.width });

		d.Fill(settings.text.color.Current());
		d.TextAlign(Align::Middle | Align::Left);
		d.FontSize(settings.text.size);
		d.Font(settings.font);
		d.Text(settings.name, { x + height + 3, y + height / 2 });
		d.Triangle({ x + width - 9, y + height / 2 - 6, 6, 6 }, 45 * 3);
	}

	bool SubMenuButton::Hitbox(const Vec2<float>& pos) const
	{
		return MenuButton::Hitbox(pos) || menu.Hitbox(pos - Vec2<float>{ x + width, y });
	}

	void MenuBarButton::Init()
	{
		Button::settings.type = Toggle;
		Button::settings.callback = [this](bool v)
		{
			if (v)
				ContextMenu::Open(menu, { x, y + height });

			else
				ContextMenu::Close(menu);
		};

		listener += [&](const Unfocus& e)
		{
			State<Selected>(false);
			ContextMenu::Close(menu);
		};
	}

	void MenuBarButton::Update()
	{
		width = GraphicsBase::StringWidth(settings.name, settings.font, settings.text.size) + settings.border.width * 2 + padding * 2;
	}

	void MenuBarButton::Render(CommandCollection& d) const
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

	Divider::Divider(const Settings& settings)
		: settings(settings)
	{
		State<Disabled>(true);
	}

	void Divider::Update()
	{
		height = settings.padding.top + settings.padding.bottom + settings.stroke;
	}

	void Divider::Render(CommandCollection& d) const
	{
		d.Fill(settings.color);
		d.Quad({ x + settings.padding.left, y + settings.padding.top, width - settings.padding.left - settings.padding.right, settings.stroke });
	}
}