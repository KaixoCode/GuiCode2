#pragma once
#include "GuiCode2/pch.hpp"
#include "GuiCode2/Panel.hpp"
#include "GuiCode2/TextDisplayer.hpp"

namespace GuiCode
{
	class TextBox : public Panel
	{
		using Panel::settings;
	public:
		TextBox();

		void Update() override;

		TextDisplayer displayer;
		int& align;
		TextContainer& container;
		std::string_view& font;
		float& fontSize;
		std::string& placeholder;
		Color& textColor;
		Color& selectColor;
	
	private:
		Vec2<float> m_Mouse;
		bool m_Dragging = false;
		int m_Update = 0;

		void UpdateScroll();
	};
}