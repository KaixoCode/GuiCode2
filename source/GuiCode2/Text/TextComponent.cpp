#include "GuiCode2/Text/TextComponent.hpp"

namespace GuiCode
{
	void Text::Update()
	{
		TextBox::Update();
		Panel::settings.min.width = GraphicsBase::StringWidth(content, font, fontSize) + 4;
		editable = false;
	}

	TextParser::TextParser()
	{
		settings.name = "text";
	}

	Pointer<Component> TextParser::Create()
	{
		return new Text{};
	}
}