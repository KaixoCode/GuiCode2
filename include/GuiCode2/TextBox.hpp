#pragma once
#include "GuiCode2/pch.hpp"
#include "GuiCode2/Panel.hpp"
#include "GuiCode2/TextDisplayer.hpp"

namespace GuiCode
{
	struct TextBoxParser : public ComponentParser
	{
		TextBoxParser();
		Pointer<Component> Create() override;
	}; 

	struct TextParser : public TextBoxParser
	{
		TextParser();
		Pointer<Component> Create() override;
	};

	class TextBox : public Panel
	{
		using Panel::settings;
	public:
		TextBox();
		TextBox(const TextBox&);
		TextBox(TextBox&&);

		void Update() override;

		TextDisplayer displayer;
		int& align = displayer.align;
		TextContainer& container = displayer.container;
		std::string& font = displayer.font;
		float& fontSize = displayer.fontSize;
		std::string& placeholder = displayer.placeholder;
		Color& textColor = displayer.textColor;
		Color& selectColor = displayer.selectColor;
		bool& editable = displayer.editable;
		std::string& content = displayer.content;
	
	private:
		Vec2<float> m_Mouse;
		bool m_Dragging = false;
		int m_Update = 0;

		void UpdateScroll();
		void Init();

		Ref<int> m_Align = displayer.align;
		Ref<TextContainer> m_Container = displayer.container;
		Ref<std::string> m_Font = displayer.font;
		Ref<float> m_FontSize = displayer.fontSize;
		Ref<std::string> m_Placeholder = displayer.placeholder;
		Ref<Color> m_TextColor = displayer.textColor;
		Ref<Color> m_SelectColor = displayer.selectColor;
		Ref<bool> m_Editable = displayer.editable;
		Ref<std::string> m_Content = displayer.content;

		friend class TextBoxParser;
	};

	class Text : public TextBox
	{
	public:
		void Update() override;
	};
}