#pragma once
#include "GuiCode2/pch.hpp"
#include "GuiCode2/Components/Panel.hpp"
#include "GuiCode2/Text/TextDisplayer.hpp"
#include "GuiCode2/Components/Menu.hpp"

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
		int& align = displayer.align;
		TextContainer& container = displayer.container;
		std::string& font = displayer.font;
		float& fontSize = displayer.fontSize;
		float& lineHeight = displayer.lineHeight;
		std::string& placeholder = displayer.placeholder;
		Color& textColor = displayer.textColor;
		Color& selectColor = displayer.selectColor;
		Wrap& wrap = displayer.wrap;
		bool& editable = displayer.editable;
		std::string& content = displayer.content;

	private:
		Button m_Cut{ {.callback = [&](bool) { displayer.Cut(); }, .combo = 'X' | Mods::Control, .name = "Cut",}};
		Button m_Copy{ {.callback = [&](bool) { displayer.Copy(); }, .combo = 'C' | Mods::Control, .name = "Copy", } };
		Button m_Paste{ {.callback = [&](bool) { displayer.Paste(); }, .combo = 'V' | Mods::Control, .name = "Paste", } };

		Menu m_Menu;
		Vec2<float> m_Mouse;
		bool m_Dragging = false;
		int m_Update = 0;

		void UpdateScroll();
		void Init();
	};
}