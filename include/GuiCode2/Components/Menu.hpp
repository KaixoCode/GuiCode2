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
			Vec4<float> margin{ 2, 2, 2, 2 }; // Outer margin
			StateColors background{ {.base = { 20, 20, 20 }} };
			Border border = Border{ .width = 1, .color = { {.base = { 64, 64, 64 } } } };

			void Link(Component* c) { background.Link(c), border.Link(c); }
		} settings;

		Menu(const Settings& = {});
		Menu(const Menu&);
		Menu(Menu&&);
		Menu& operator=(const Menu&) = default;
		Menu& operator=(Menu&&) = default;

		void ForwardRender(CommandCollection&) override;
		void ForwardUpdate() override;
		bool Hitbox(const Vec2<float>& pos) const override;

	private:
		void InitListeners();
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
}