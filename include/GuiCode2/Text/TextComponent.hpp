#pragma once
#include "GuiCode2/Text/TextBox.hpp"

namespace GuiCode 
{
	struct TextParser : public TextBoxParser
	{
		TextParser();
		Pointer<Component> Create() override;
	};

	class Text : public TextBox
	{
	public:
		void Update() override;
	};
}