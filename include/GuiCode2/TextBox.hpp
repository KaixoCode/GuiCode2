//#pragma once
//#include "GuiCode/ui/components/panels/ScrollPanel.hpp"
//#include "GuiCode/ui/components/text/TextDisplayer.hpp"
//
//// --------------------------------------------------------------------------
//// -------------------------------- TextBox ---------------------------------
//// --------------------------------------------------------------------------
//
///**
// * Single line TextBox.
// */
//class TextBox : public Panel
//{
//public:
//
//    /**
//     * Constructor.
//     */
//    TextBox();
//
//    void Update(const Vec4<int>& v) override;
//    void Render(CommandCollection& d) override;
//    
//    /**
//     * Set the horizontal alignment of the lines in this TextDisplayer.
//     * @param a alignment
//     */
//    void AlignLines(Align a) { m_Displayer.AlignLines(a); }
//
//    /**
//     * Set the content of this container.
//     * @param c content
//     */
//    void Content(const std::string& c) { m_Displayer.Content(c); }
//
//    /**
//     * Set if this container is editable.
//     * @param a editable
//     */
//    void Editable(bool e) { m_Displayer.Editable(e); }
//
//    /**
//     * Set the text color.
//     * @param c color
//     */
//    void TextColor(Color c) { m_Displayer.TextColor(c); }
//
//    /**
//     * Set the selection color.
//     * @param c color
//     */
//    void SelectColor(Color c) { m_Displayer.SelectColor(c); }
//
//    /**
//     * Set the text wrap mode.
//     * @param wrap wrap mode
//     */
//    void TextWrap(TextDisplayer::Wrap wrap) { m_Displayer.TextWrap(wrap); }
//
//    /**
//     * Set the line height.
//     * @param h line height
//     */
//    void LineHeight(int h) { m_Displayer.LineHeight(h); };
//
//    /**
//     * Set the font and font size.
//     * @param f font
//     * @param size font size
//     */
//    void Font(int f, float size) { m_Displayer.Font(f, size); };
//
//    /**
//     * Set the padding around the text.
//     * @param p padding
//     */
//    void Padding(int p) { m_Displayer.Padding(p); };
//
//    /**
//     * Get the current horizontal alignment of the text.
//     * @return text alignment
//     */
//    auto AlignLines() -> Align { return m_Displayer.AlignLines(); }
//
//    /**
//     * Get the entire content of this container as a string.
//     * @return content
//     */
//    auto Content() -> std::string& { return m_Displayer.Content(); }
//
//    /**
//     * Returns true when this text container is editable.
//     * @return true if editable
//     */
//    bool Editable() { return m_Displayer.Container().Editable(); }
//
//    /**
//     * Get the text color.
//     * @return text color
//     */
//    auto TextColor() const -> Color { return m_Displayer.TextColor(); }
//
//    /**
//     * Get the selection color.
//     * @return selection color
//     */
//    auto SelectColor() const -> Color { return m_Displayer.SelectColor(); }
//
//    /**
//     * Get the current wrap mode.
//     * @return wrap
//     */
//    auto TextWrap() const -> TextDisplayer::Wrap { return m_Displayer.TextWrap(); }
//
//    /**
//     * Get the current line height.
//     * @return line height
//     */
//    int LineHeight() const { return m_Displayer.LineHeight(); };
//
//    /**
//     * Get the current font.
//     * @return font
//     */
//    int Font() const { return m_Displayer.Font(); };
//
//    /**
//     * Get the current font size.
//     * @return font size
//     */
//    float FontSize() const { return m_Displayer.FontSize(); };
//
//    /**
//     * Get the current padding around the text.
//     * @return padding
//     */
//    int Padding() const { return m_Displayer.Padding(); };
//    
//    /**
//     * Set a placeholder string.
//     * @param p placeholder
//     */
//    void Placeholder(const std::string& p) { m_Displayer.Placeholder(p); }
//
//    /**
//     * Get the placeholder string.
//     * @return placeholder
//     */
//    auto Placeholder() -> std::string& { return m_Displayer.Placeholder(); }
//
//    auto Displayer() ->TextDisplayer& { return m_Displayer; }
//
//protected:
//    TextDisplayer& m_Displayer;
//    Vec2<int> m_Mouse{ 0, 0 };
//
//    int m_ScrollX = 0;
//
//    void UpdateScroll();
//};