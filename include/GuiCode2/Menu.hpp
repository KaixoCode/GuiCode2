#pragma once
#include "GuiCode2/Component.hpp"
#include "GuiCode2/BasicEvents.hpp"
#include "GuiCode2/Key.hpp"
#include "GuiCode2/Panel.hpp"
#include "GuiCode2/Button.hpp"

namespace GuiCode
{
	class Menu : public Panel
	{
		using Panel::panels;
		using Panel::settings;

	public:

		Menu(std::initializer_list<Pointer<Component>> comp);
		Menu();
		Menu(Menu&&);
		Menu(const Menu&);

		template<std::derived_from<Component> T>
		T& Emplace(T&& object)
		{
			auto& panel = panels.Emplace({ {.ratio = 0,.size{ Inherit, Auto } }, std::forward<T>(object) });
			return panel.component;
		}

		template<std::derived_from<Component> T>
		T& Emplace(T& object)
		{
			auto& panel = panels.Emplace({ {.ratio = 0,.size{ Inherit, Auto } }, object });
			return panel.component;
		}

		void Clear();

		Vec4<float> padding;

	private:
		void Init();
	};


	class MenuButton : public Button
	{
	public:
		struct Settings
		{
			Button::Group group;
			Button::Type type;
			Button::Callback callback;
			Key combo;

			std::string name = "Button";

			StateColors color;
			struct
			{
				float width = 1;
				StateColors color;
			} border;

			struct
			{
				float size = 12;
				StateColors color;
				StateColors combo;
			} text;

			StateColors select;

			std::string_view font = "segoeui";

			void Link(Component* c) { select.Link(c), color.Link(c), border.color.Link(c), text.color.Link(c); }
		};

		MenuButton(const Settings& settings = {});
		MenuButton(MenuButton&& other);
		MenuButton(const MenuButton& other);
		~MenuButton();

		void Render(CommandCollection& d) const override;

		Settings settings;

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