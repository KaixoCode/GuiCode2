#pragma once
#include "GuiCode2/Component.hpp"
#include "GuiCode2/BasicEvents.hpp"
#include "GuiCode2/Key.hpp"
#include "GuiCode2/Panel.hpp"
#include "GuiCode2/Button.hpp"
#include "GuiCode2/ContextMenu.hpp"

namespace GuiCode
{
	class Menu : public Panel
	{
		using Panel::panels;
		using Panel::settings;

	public:
		Menu(bool vertical = true);
		Menu(Menu&&);
		Menu(const Menu&);

		template<std::derived_from<Component> T>
		T& Emplace(T&& object)
		{
			auto& panel = panels.Emplace({ {.ratio = 0,.size{ (float)(vertical ? Inherit : Auto), (float)(vertical ? Auto : Inherit) } }, std::forward<T>(object) });
			RefreshLayout();
			return panel.component;
		}

		template<std::derived_from<Component> T>
		T& Emplace(T& object)
		{
			auto& panel = panels.Emplace({ {.ratio = 0,.size{ (float)(vertical ? Inherit : Auto), (float)(vertical ? Auto : Inherit) } }, object });
			RefreshLayout();
			return panel.component;
		}

		void Clear();

		bool Hitbox(const Vec2<float>& pos) const override;

		Vec4<float>& padding;
		Color& background;
		Border& border;
		const bool vertical = true;
	
	private:
		void Init();
	};

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

			struct
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

			std::string_view font = "segoeui";

			void Link(Component* c) { select.Link(c), color.Link(c), border.color.Link(c), text.color.Link(c); }
		};

		MenuButton(const Settings& settings = {});
		MenuButton(MenuButton&& other);
		MenuButton(const MenuButton& other);

		void Update() override;
		void Render(CommandCollection& d) const override;

		Settings settings;

	private:
		void Init();
	};

	struct SubMenuButton : public MenuButton
	{
		SubMenuButton(const Settings& settings = {}) : MenuButton(settings) { Init(); }
		SubMenuButton(SubMenuButton&& other) : MenuButton(std::forward<SubMenuButton>(other)) { Init(); }
		SubMenuButton(const SubMenuButton& other) : MenuButton(other) { Init(); }

		void Update() override;
		void Render(CommandCollection& d) const override;
		bool Hitbox(const Vec2<float>& pos) const override;

		Menu menu;

	private:
		void Init();
	};

	class MenuBarButton : public MenuButton
	{
	public:
		MenuBarButton(const Settings& settings = {}) : MenuButton(settings) { Init(); }
		MenuBarButton(MenuBarButton&& other) : MenuButton(std::forward<MenuBarButton>(other)) { Init(); }
		MenuBarButton(const MenuBarButton& other) : MenuButton(other) { Init(); }

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
			struct {
				float x = 1;
				float y = 2;
			} padding;
			float stroke = 1;
			Color color{ 51, 51, 51, 255 };
		} settings;

		Divider(const Settings& settings = {});

		void Update() override;
		void Render(CommandCollection& d) const override;
	};
}