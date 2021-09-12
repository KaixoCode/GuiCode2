#pragma once
#include "GuiCode2/pch.hpp"
#include "GuiCode2/Panel.hpp"
#include "GuiCode2/TextDisplayer.hpp"

namespace GuiCode
{
	class TextArea : public Panel
	{
		using Panel::settings;
	public:
		TextArea();
		TextArea(const TextArea&) = delete;
		TextArea(const TextArea&&);

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
		Vec2<float> m_Mouse;
		bool m_Dragging = false;
		int m_Update = 0;

		void UpdateScroll();
		void Init();
	};
}