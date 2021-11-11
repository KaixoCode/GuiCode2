#pragma once
#include "GuiCode2/Components/Component.hpp"
#include "GuiCode2/Events/BasicEvents.hpp"
#include "GuiCode2/Events/Key.hpp"
#include "GuiCode2/Components/Panel.hpp"
#include "GuiCode2/Components/Button.hpp"
#include "GuiCode2/Utils/ContextMenu.hpp"

namespace GuiCode
{
	class Menu : public Component
	{
	public:
		struct Settings 
		{
			bool vertical = true;
			float padding = 0;  // Inter-component padding
			Vec4<float> margin; // Outer margin
		} settings;

		Menu(const Settings& = {});
		Menu(const Menu&);
		Menu(Menu&&);
		Menu& operator=(const Menu&) = default;
		Menu& operator=(Menu&&) = default;

		void ForwardUpdate() override;
		bool Hitbox(const Vec2<float>& pos) const override;

	private:
		void InitListeners();
	};

	struct SubMenuButton : public Button
	{
		SubMenuButton(const Settings&);
		SubMenuButton(const SubMenuButton&) = default;
		SubMenuButton(SubMenuButton&&) = default;
		SubMenuButton& operator=(const SubMenuButton&) = default;
		SubMenuButton& operator=(SubMenuButton&&) = default;

		bool Hitbox(const Vec2<float>& pos) const override;

		Menu menu;
	};

	class Divider : public Component
	{
	public:
		struct Settings
		{
			Vec4<float> padding{ 22, 2, 1, 2, };
			float stroke = 1;
			Color color{ 51, 51, 51, 255 };
		} settings;

		Divider(const Settings& settings = {});
		Divider(const Divider&) = default;
		Divider(Divider&&) = default;
		Divider& operator=(const Divider&) = default;
		Divider& operator=(Divider&&) = default;

		void Update() override;
		void Render(CommandCollection& d) const override;
	};

	struct MenuButtonGraphics : public Button::Graphics
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
		}

		void Render(CommandCollection& d) const override 
		{
			d.Fill(border.color.Current());
			d.Quad(button->dimensions);

			d.Fill(color.Current());
			d.Quad({ button->x + border.width, button->y + border.width, button->width - 2 * border.width, button->height - 2 * border.width });

			if (button->State(Selected))
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