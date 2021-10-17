#include "GuiCode2/Components/Menu.hpp"

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
		if (vertical)
		{
			padding = { 4, 4, 4, 4 };
			border.width = 1;
			border.color = { 64, 64, 64, 255 };
		}
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

	Menu& Menu::operator=(Menu&& other)
	{
		vertical = other.vertical;
		Panel::operator=(std::move(other));
		return *this;
	}

	void Menu::Clear()
	{
		panels.clear();
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
			Component::min.width = _min + (scrollbar.y.Necessary() ? scrollbar.y.width : 0) + padding.left + padding.right;
			Component::min.height = 39 + padding.top + padding.bottom + (scrollbar.x.Necessary() ? scrollbar.x.height : 0);
		}
		ConstrainSize();
	}

	bool Menu::Hitbox(const Vec2<float>& pos) const
	{
		if (Panel::Hitbox(pos))
			return true;

		// Make sure to include menu component hitboxes
		for (auto& i : panels)
			if (i->component->Hitbox(pos))
				return true;

		return false;
	}

	void Menu::Init()
	{
		settings.max.height = 500;
		settings.max.width = 500;
		settings.ratio = 1;
		settings.layout = vertical ? Layout::Column : Layout::Row;
		settings.overflow.x = vertical ? Overflow::Hide : Overflow::Scroll;
		settings.overflow.y = vertical ? Overflow::Scroll : Overflow::Hide;
		settings.size.width = vertical ? Inherit : Auto;
		settings.size.height = vertical ? Auto : Inherit;
		listener += [&](const KeyPress& e)
		{
			if (e.Handled() || !State(Focused))
				return;

			Component* _c = Get(Hovering);
			if ((vertical && e.keycode == Key::Down) || (!vertical && e.keycode == Key::Right))
			{
				if (_c && _c != this)
				{
					bool _found = false;
					for (auto& i : panels)
					{
						if (_found && !i->component->State(Disabled))
						{
							_c->State(Hovering) = false;
							_c->listener(MouseExit{});
							i->component->State(Hovering) = true;
							i->component->listener(MouseEnter{});
							e.Handle();
							break;
						}

						if (i->component == _c)
							_found = true;
					}
				}
				else
				{
					(*panels.begin())->component->State(Hovering) = true;
					(*panels.begin())->component->listener(MouseEnter{});
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
						if (i->component == _c)
						{
							if (_prev)
							{
								_c->State(Hovering) = false;
								_c->listener(MouseExit{});
								_prev->State(Hovering) = true;
								_prev->listener(MouseEnter{});
								e.Handle();
							}
							break;
						}

						if (!i->component->State(Disabled))
							_prev = i->component;
					}
				}
				else
				{
					(*(--panels.end()))->component->State(Hovering) = true;
					(*(--panels.end()))->component->listener(MouseEnter{});
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
				_c->State(Hovering) = false;
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

	MenuButton& MenuButton::operator=(MenuButton&& other)
	{
		settings = std::move(other.settings);
		Button::operator=(std::move(other));
		return *this;
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
		_minWidth += GraphicsBase::StringWidth(Button::settings.combo.ToString(), settings.font, settings.text.size);
		_minWidth += height + 36;
		min.width = _minWidth;
	}

	void MenuButton::Render(CommandCollection& d) const
	{
		d.Fill(settings.border.color.Current());
		d.Quad(dimensions);

		d.Fill(settings.color.Current());
		d.Quad({ x + settings.border.width, y + settings.border.width, width - 2 * settings.border.width, height - 2 * settings.border.width });

		if (State(Selected))
		{
			d.Fill(settings.select.Current());
			d.Quad({ x + 3, y + 3, height - 6, height - 6 });
		}

		d.Fill(settings.text.color.Current());
		d.TextAlign(Align::Middle | Align::Left);
		d.FontSize(settings.text.size);
		d.Font(settings.font);
		d.Text(settings.name, { x + height + 3, y + height / 2 });
		if (Button::settings.combo)
		{
			d.TextAlign(Align::Middle | Align::Right);
			d.Text(Button::settings.combo.ToString(), { x + width - 9, y + height / 2 });
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
		d.Triangle({ x + width - 8, y + height / 2, 4, 8 }, 0.0f);
	}

	bool SubMenuButton::Hitbox(const Vec2<float>& pos) const
	{
		return MenuButton::Hitbox(pos) || menu.Hitbox(pos - Vec2<float>{ x + width, y });
	}

	MenuBarButton& MenuBarButton::operator=(MenuBarButton&& other)
	{
		padding = std::move(other.padding);
		menu = std::move(other.menu);
		MenuButton::operator=(std::move(other));
		return *this;
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
			State(Selected) = false;
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

	/**
	 * Parsers
	 */

	MenuButtonParser::MenuButtonParser()
	{
		Parser::Link<ButtonParser>();
		settings.name = "menu-button";
		Attribute("name", &MenuButton::m_Name);
		Attribute("color", &MenuButton::m_Color);
		Attribute("color.transition", &StateColors::transition);
		Attribute("border-color", &MenuButton::m_BorderColor);
		Attribute("border-color.transition", &StateColors::transition);
		Attribute("border-width", &MenuButton::m_BorderWidth);
		Attribute("text-size", &MenuButton::m_TextSize);
		Attribute("text-color", &MenuButton::m_TextColor);
		Attribute("select-square", &MenuButton::m_Select);
		Attribute("font", &MenuButton::m_Font);
	}

	SubMenuButtonParser::SubMenuButtonParser()
	{
		settings.name = "sub-menu-button";
		alias["button"] = "menu-button";
		alias["menu"] = "sub-menu-button";
		Attribute("menu-border", &SubMenuButton::m_MenuBorder);
		Attribute("menu-padding", &SubMenuButton::m_MenuPadding);
		Attribute("menu-background", &SubMenuButton::m_MenuBackground);
	}

	MenuBarButtonParser::MenuBarButtonParser()
	{
		settings.name = "menu-bar-button";
		alias["button"] = "menu-button";
		alias["menu"] = "sub-menu-button";
		Attribute("menu-border", &MenuBarButton::m_MenuBorder);
		Attribute("menu-padding", &MenuBarButton::m_MenuPadding);
		Attribute("menu-background", &MenuBarButton::m_MenuBackground);
	}

	DividerParser::DividerParser()
	{
		settings.name = "divider";
		Attribute("padding", &Divider::m_Padding);
		Attribute("stroke", &Divider::m_Stroke);
		Attribute("color", &Divider::m_Color);
	}

	MenuParser::MenuParser()
	{
		Parser::Link<DividerParser>();
		Parser::Link<MenuButtonParser>();
		Parser::Link<MenuBarButtonParser>();
		Parser::Link<SubMenuButtonParser>();
		settings.name = "menu";
		alias["button"] = "menu-button";
		alias["menu"] = "sub-menu-button";
		Attribute("vertical", &Menu::vertical);
		Attribute("padding", &Menu::m_Padding);
		Attribute("background", &Menu::m_Background);
		Attribute("border", &Menu::m_Border);
		Attribute("border.width", &Border::width);
		Attribute("border.color", &Border::color);
		Attribute("border.left", &Border::left);
		Attribute("border.left.width", &Border::Side::width);
		Attribute("border.left.color", &Border::Side::color);
		Attribute("border.right", &Border::right);
		Attribute("border.right.width", &Border::Side::width);
		Attribute("border.right.color", &Border::Side::color);
		Attribute("border.top", &Border::top);
		Attribute("border.top.width", &Border::Side::width);
		Attribute("border.top.color", &Border::Side::color);
		Attribute("border.bottom", &Border::bottom);
		Attribute("border.bottom.width", &Border::Side::width);
		Attribute("border.bottom.color", &Border::Side::color);
	}

	MenuBarParser::MenuBarParser()
	{
		Parser::Link<MenuParser>();
		settings.name = "menu-bar";
		alias["button"] = "menu-bar-button";
		alias["menu"] = "sub-menu-button";
	}

	void MenuParser::Append(Component& c, Pointer<Component>&& obj)
	{
		if (Menu* _t = dynamic_cast<Menu*>(&c))
			_t->push_back(std::move(obj));
	}

	void MenuBarButtonParser::Append(Component& c, Pointer<Component>&& obj)
	{
		if (MenuBarButton* _t = dynamic_cast<MenuBarButton*>(&c))
			_t->menu.push_back(std::move(obj));
	}

	void SubMenuButtonParser::Append(Component& c, Pointer<Component>&& obj)
	{
		if (SubMenuButton* _t = dynamic_cast<SubMenuButton*>(&c))
			_t->menu.push_back(std::move(obj));
	}

	Pointer<Component> SubMenuButtonParser::Create() { return new SubMenuButton{}; }
	Pointer<Component> MenuButtonParser::Create() { return new MenuButton{}; }
	Pointer<Component> MenuBarButtonParser::Create() { return new MenuBarButton{}; }
	Pointer<Component> DividerParser::Create() { return new Divider{}; }
	Pointer<Component> MenuParser::Create() { return new Menu{}; }
	Pointer<Component> MenuBarParser::Create() { return new Menu{ false }; }
}