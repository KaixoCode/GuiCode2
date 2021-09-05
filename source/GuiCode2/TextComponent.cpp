//#include "GuiCode/ui/components/text/TextComponent.hpp"
//
//// --------------------------------------------------------------------------
//// ---------------------------- Text Component ------------------------------
//// --------------------------------------------------------------------------
//
//TextComponent::TextComponent(const std::string& t, int f, float fsize)
//{
//	Content(t);
//	Font(f, fsize);
//	AlignLines(Align::LEFT);
//	TextColor({ 255, 255, 255, 255 });
//	Editable(false);
//	Width(Graphics::StringWidth(t, f, fsize) + 6);
//	Height(fsize + 4);
//	Padding(2);
//	Background({ 0, 0, 0, 0 });
//}
//
//void TextComponent::Content(const std::string& c)
//{
//	//Width(Graphics::StringWidth(c, Font(), FontSize()) + 4);
//	//Height(FontSize() + 4);
//	TextBox::Content(c);
//}
//
//void TextComponent::Font(int f, float size)
//{
//	Width(Graphics::StringWidth(Content(), f, size) + 6);
//	Height(size + 4);
//	TextBox::Font(f, size);
//}
//
