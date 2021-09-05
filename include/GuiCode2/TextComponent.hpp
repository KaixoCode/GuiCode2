//#pragma once
//#include "GuiCode/pch.hpp"
//#include "GuiCode/ui/components/Component.hpp"
//#include "GuiCode/ui/graphics/Graphics.hpp"
//#include "GuiCode/ui/components/text/TextBox.hpp"
//
//// --------------------------------------------------------------------------
//// ---------------------------- Text Component ------------------------------
//// --------------------------------------------------------------------------
//
///**
// * Simple TextComponent for displaying a single line of non-editable text.
// */
//class TextComponent : public TextBox
//{
//public:
//
//	/**
//	 * Constructor.
//	 * @param t content
//	 * @param f font
//	 * @param fsize font size
//	 */
//	TextComponent(const std::string& t, int f = Graphics::Fonts::Gidole14, float fsize = 14);
//
//	/**
//	 * Set the content.
//	 * @param c content
//	 */
//	void Content(const std::string& c);
//
//	/**
//	 * Get the content.
//	 * @return content
//	 */
//	auto Content() -> std::string& { return TextBox::Content(); }
//
//	/**
//	 * Set the font and font size.
//	 * @param f font
//	 * @param size font size
//	 */
//	void Font(int f, float size);
//
//	/**
//	 * Get the font.
//	 * @return font
//	 */
//	int Font() const { return TextBox::Font(); }
//};
