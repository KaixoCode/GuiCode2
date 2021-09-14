#pragma once
#include "GuiCode2/pch.hpp"
#include "GuiCode2/Panel.hpp"
#include "GuiCode2/TextDisplayer.hpp"
#include "GuiCode2/Menu.hpp"

namespace GuiCode
{
	class TextArea : public Panel
	{
		using Panel::settings;
	public:
		TextArea();
		TextArea(const TextArea&);
		TextArea(TextArea&&);

		void Update() override;

		TextDisplayer displayer;
		int& align;
		TextContainer& container;
		std::string_view& font;
		float& fontSize;
		float& lineHeight;
		std::string& placeholder;
		Color& textColor;
		Color& selectColor;
		Wrap& wrap;

	private:
		MenuButton m_Cut{ {.callback = [&](bool) { displayer.Cut(); }, .combo = Key::CTRL_X, .name = "Cut", } };
		MenuButton m_Copy{ {.callback = [&](bool) { displayer.Copy(); }, .combo = Key::CTRL_C, .name = "Copy", } };
		MenuButton m_Paste{ {.callback = [&](bool) { displayer.Paste(); }, .combo = Key::CTRL_V, .name = "Paste", } };

		Menu m_Menu;
		Vec2<float> m_Mouse;
		bool m_Dragging = false;
		int m_Update = 0;

		void UpdateScroll();
		void Init();
	};
}