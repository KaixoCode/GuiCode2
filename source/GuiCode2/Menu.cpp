#include "GuiCode2/Menu.hpp"

namespace GuiCode
{
	Menu::Menu(std::initializer_list<Pointer<Component>> comp)
	{
		for (auto& i : comp)
			panels.Emplace({ {.ratio = 0,.size{ Inherit, Auto }  }, std::move(i) });

		Init();
	}

	Menu::Menu() { Init(); }
	Menu::Menu(Menu&&) { Init(); }
	Menu::Menu(const Menu&) { Init(); }

	void Menu::Clear()
	{
		panels.Clear();
	}

	void Menu::Init()
	{
		settings.ratio = 1;
		settings.layout = Layout::Column;
		settings.overflow = Overflow::Scroll;
		listener += [&](const KeyPress& e)
		{
			if (e.Handled() || !State<Focused>())
				return;

			Component* _c = Get(Hovering);
			if (e.keycode == Key::Down)
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
			else if (e.keycode == Key::Up)
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
								_c->listener(MouseEnter{});
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
		std::cout << "CONSTRUCT" << std::endl;
	}

	MenuButton::MenuButton(MenuButton&& other)
		: Button({ other.settings.group, other.settings.type, other.settings.callback, other.settings.combo })
	{
		settings = other.settings;
		Init();
		std::cout << "MOVE" << std::endl;
	}

	MenuButton::MenuButton(const MenuButton& other)
		: Button({ other.settings.group, other.settings.type, other.settings.callback, other.settings.combo })
	{
		Init();
		settings = other.settings;
		std::cout << "COPY" << std::endl;
	}

	MenuButton::~MenuButton()
	{
		std::cout << "DESTRUCT" << std::endl;
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
			d.Text(settings.combo.ToString(), { x + width - 9, y + height / 2});
		}
	}

	void MenuButton::Init()
	{
		height = 20;
		settings.Link(this);
		settings.color.base = { 0, 0, 0, 0 };
		settings.color.transition = 10;
		settings.color.State<Disabled>({ 0, 0, 0, 0 });
		settings.color.State<Pressed>(0x414141);
		settings.color.State<Hovering>(0x262626);
		settings.border.color.base = { 0, 0, 0, 0 };
		settings.border.color.State<Disabled>({ 0, 0, 0, 0 });
		settings.border.color.State<Pressed>(0x919191);
		settings.border.color.State<Hovering>(0x767676);
		settings.text.color.base = 0xE0E0E0;
		settings.text.color.State<Disabled>(0xA0A0A0);
		settings.select.base = 0x414141;
		settings.select.State<Hovering>(0x767676);
	}

	Divider::Divider(const Settings& settings)
		: settings(settings)
	{
		State<Disabled>(true);
	}

	void Divider::Update()
	{
		height = settings.padding.y * 2 + settings.stroke;
	}

	void Divider::Render(CommandCollection& d) const
	{
		d.Fill(settings.color);
		d.Quad({ x + settings.padding.x, y + settings.padding.y, width - 2 * settings.padding.x, settings.stroke });
	}
}