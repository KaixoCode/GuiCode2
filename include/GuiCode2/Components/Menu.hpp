#pragma once
#include "GuiCode2/Components/Component.hpp"
#include "GuiCode2/Events/BasicEvents.hpp"
#include "GuiCode2/Events/Key.hpp"
#include "GuiCode2/Components/Panel.hpp"
#include "GuiCode2/Components/Button.hpp"
#include "GuiCode2/Utils/ContextMenu.hpp"

namespace GuiCode
{
	/**
	 * Basic menu, makes use of a Panel to layout.
	 */
	class Menu : public Panel
	{
		using Panel::panels;
		using Panel::settings;

	public:
		Menu(bool vertical = true);
		Menu(Menu&&);
		Menu(const Menu&) = delete;
		Menu& operator=(Menu&& other);
		Menu& operator=(const Menu& other) = delete;

		/**
		 * Add a component to this menu.
		 * @param object
		 * @return the object
		 */
		Pointer<Component>& push_back(Pointer<Component>&& object)
		{
			auto& panel = panels.push_back(new Panel{ {.ratio = 0,.size{ (float)(vertical ? Inherit : Auto), (float)(vertical ? Auto : Inherit) } }, std::move(object) });
			RefreshLayout();
			return panel.component;
		}

		void Clear();
		void Update() override;
		bool Hitbox(const Vec2<float>& pos) const override;

		Vec4<float>& padding;
		Color& background;
		Border& border;

	private:
		bool vertical = true;

		void Init();
	};

	/**
	 * Button with menu graphics.
	 */
	class MenuButton : public Button
	{
	public:
		struct Settings
		{
			Button::Group group;
			Button::Type type = Click;
			Button::Callback callback;
			Key combo;

			std::string name = "Button";

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

			void Link(Component* c) { select.Link(c), color.Link(c), border.color.Link(c), text.color.Link(c); }
		};

		MenuButton(const Settings& settings = { });
		MenuButton(MenuButton&& other);
		MenuButton(const MenuButton& other);
		MenuButton& operator=(MenuButton&& other);

		void Update() override;
		void Render(CommandCollection& d) const override;

		Settings settings;

	private:
		void Init();
	};

	/**
	 * Button with menu graphics, that opens a 
	 * sub-menu when pressed.
	 */
	struct SubMenuButton : public MenuButton
	{
		SubMenuButton(const Settings& settings = {}) : MenuButton(settings) { Init(); }
		SubMenuButton(SubMenuButton&& other) : MenuButton(std::forward<SubMenuButton>(other)) { Init(); }
		SubMenuButton(const SubMenuButton& other) : MenuButton(other) { Init(); }

		void Render(CommandCollection& d) const override;
		bool Hitbox(const Vec2<float>& pos) const override;

		Menu menu;

	private:
		void Init();
	};

	/**
	 * Menu-bar-button is used in the MenuBar in a frame.
	 */
	class MenuBarButton : public MenuButton
	{
	public:
		MenuBarButton(const Settings& settings = {}) : MenuButton(settings) { Init(); }
		MenuBarButton(MenuBarButton&& other) : MenuButton(std::forward<MenuBarButton>(other)) { Init(); }
		MenuBarButton(const MenuBarButton& other) : MenuButton(other) { Init(); }
		MenuBarButton& operator=(MenuBarButton&& other);

		void Update() override;
		void Render(CommandCollection& d) const override;

		Menu menu;
		float padding = 6;

	private:
		void Init();
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

		void Update() override;
		void Render(CommandCollection& d) const override;
	};
}