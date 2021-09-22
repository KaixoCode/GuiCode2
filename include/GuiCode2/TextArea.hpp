#pragma once
#include "GuiCode2/pch.hpp"
#include "GuiCode2/Panel.hpp"
#include "GuiCode2/TextDisplayer.hpp"
#include "GuiCode2/Menu.hpp"

namespace GuiCode
{
	struct TextAreaParser : public ComponentParser
	{
		TextAreaParser();
		Pointer<Component> Create() override;
	};

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
		MenuButton m_Cut{ {.callback = [&](bool) { displayer.Cut(); }, .combo = Key::CTRL_X, .name = "Cut", } };
		MenuButton m_Copy{ {.callback = [&](bool) { displayer.Copy(); }, .combo = Key::CTRL_C, .name = "Copy", } };
		MenuButton m_Paste{ {.callback = [&](bool) { displayer.Paste(); }, .combo = Key::CTRL_V, .name = "Paste", } };

		Menu m_Menu;
		Vec2<float> m_Mouse;
		bool m_Dragging = false;
		int m_Update = 0;

		void UpdateScroll();
		void Init();

		Ref<int> m_Align = displayer.align;
		Ref<TextContainer> m_Container = displayer.container;
		Ref<std::string> m_Font = displayer.font;
		Ref<float> m_FontSize = displayer.fontSize;
		Ref<float> m_LineHeight = displayer.lineHeight;
		Ref<std::string> m_Placeholder = displayer.placeholder;
		Ref<Color> m_TextColor = displayer.textColor;
		Ref<Color> m_SelectColor = displayer.selectColor;
		Ref<Wrap> m_Wrap = displayer.wrap;
		Ref<bool> m_Editable = displayer.editable;
		Ref<std::string> m_Content = displayer.content;

		friend class TextAreaParser;
	};
}