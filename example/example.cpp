#include "GuiCode2/Window/Gui.hpp"
#include "GuiCode2/Window/Frame.hpp"
#include "GuiCode2/Components/Menu.hpp"

using namespace GuiCode;

namespace GuiCode
{

	namespace Graphics
	{
		struct MenuButton : public Button::Graphics
		{
			StateColors color{ {
				.base{ 38, 38, 38, 0 },
				.colors{
					{ Disabled, { 0, 0, 0, 0 } },
					{ Pressed, 0x414141 },
					{ Hovering, 0x262626 },
				},
				.transition = 10
			} };

			struct Border
			{
				float width = 1;
				StateColors color{ {
					.base{ 118, 118, 118, 0 },
					.colors{
						{ Disabled, { 0, 0, 0, 0 } },
						{ Pressed, 0x919191 },
						{ Hovering, 0x767676 },
					},
					.transition = 100
				} };
			} border;

			struct
			{
				float size = 12;
				StateColors color{ {
					.base = 0xE0E0E0,
					.colors{
						{ Disabled, 0xA0A0A0 }
					}
				} };
			} text;

			StateColors select{ {
				.base = 0x414141,
				.colors{
					{ Hovering, 0x767676 }
				}
			} };

			std::string font = GraphicsBase::DefaultFont;

			void Link(Button* button)
			{
				select.Link(button);
				color.Link(button);
				border.color.Link(button);
				text.color.Link(button);
				button->height = 20;
			}

			void Update() override
			{
				// Set the minimum width of this menu button to fit all text.
				float _minWidth = 0;
				_minWidth += GraphicsBase::StringWidth(button->settings.name, font, text.size);
				_minWidth += GraphicsBase::StringWidth(button->settings.combo.ToString(), font, text.size);
				_minWidth += button->height + 36;
				button->min.width = _minWidth;
				button->ConstrainSize();
			}

			void Render(CommandCollection& d) const override
			{
				d.Fill(border.color.Current());
				d.Quad(button->dimensions);

				d.Fill(color.Current());
				d.Quad({ button->x + border.width, button->y + border.width, button->width - 2 * border.width, button->height - 2 * border.width });

				if (button->State(Selected) && button->settings.type != Button::Hover)
				{
					d.Fill(select.Current());
					d.Quad({ button->x + 3, button->y + 3, button->height - 6, button->height - 6 });
				}

				d.Fill(text.color.Current());
				d.TextAlign(Align::Middle | Align::Left);
				d.FontSize(text.size);
				d.Font(font);
				d.Text(button->settings.name, { button->x + button->height + 3, button->y + button->height / 2 });
				if (button->settings.combo)
				{
					d.TextAlign(Align::Middle | Align::Right);
					d.Text(button->settings.combo.ToString(), { button->x + button->width - 9, button->y + button->height / 2 });
				}
			}
		};
	}
}

int main() {
	Gui _gui;

	Window::Settings _settings{
		.name = "Window"
	};

	Frame& _frame = _gui.emplace<Frame>(_settings);
	
	_frame.Create();

	Menu _file;
	Button& _b1 = _frame.titlebar.menu.push_back<Button>({
		.type = Button::Click,
		.name = "File",
		.graphics = new Graphics::MenuButton
	});

	_b1.settings.callback = [&](bool v) { 
		v ? ContextMenu::Open(_file, { _b1.x, _b1.y + _b1.height }) 
		  : ContextMenu::Close(_file); 
	};

	Menu _other;
	Button& _b2 = _file.push_back<Button>({
		.type = Button::Hover,
		.combo = 'A' | Mods::Control,
		.name = "test",
		.graphics = new Graphics::MenuButton
	});
	
	_b2.settings.callback = [&](bool v) { 
		v ? ContextMenu::Open(_other, { _b2.x + _b2.width, _b2.y })
		  : ContextMenu::Close(_other);
	};

	_other.push_back<Button>({
		.name = "Carrot",
		.graphics = new Graphics::MenuButton
	});

	_other.push_back<Button>({
		.name = "Soup",
		.graphics = new Graphics::MenuButton
	});

	_file.push_back<Button>({
		.combo = 'C' | Mods::Shift,
		.name = "test2",
		.graphics = new Graphics::MenuButton
	});

	_file.push_back<Button>({
		.combo = 'Q' | Mods::Alt | Mods::Shift,
		.name = "test3",
		.graphics = new Graphics::MenuButton
	});

	_file.push_back<Button>({
		.combo = Key::F11 | Mods::Control | Mods::Shift,
		.name = "test4",
		.graphics = new Graphics::MenuButton
	});

	while (_gui.Loop()) {}
}