#pragma once
#include "GuiCode2/pch.hpp"
#include "GuiCode2/Components/Panel.hpp"
#include "GuiCode2/Text/TextDisplayer.hpp"

namespace GuiCode
{
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
	};
}