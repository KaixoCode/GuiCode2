//#include "GuiCode/ui/components/text/TextArea.hpp"
//
//// --------------------------------------------------------------------------
//// ------------------------------- TextArea ---------------------------------
//// --------------------------------------------------------------------------
//
//TextArea::TextArea()
//    : m_Displayer(Panel<TextDisplayer>())
//{
//    m_Listener += [this](Event::MousePressed& e)
//    {
//        if (e.button != Event::MouseButton::LEFT)
//            return;
//
//        m_Mouse = { e.x, e.y };
//    };
//
//    m_Listener += [this](Event::MouseDragged& e)
//    {
//        if (e.button != Event::MouseButton::LEFT)
//            return;
//
//        m_Mouse = { e.x, e.y };
//    };
//
//    m_Listener += [this](Event::KeyPressed& e)
//    {
//        UpdateScroll();
//    };
//
//    m_Listener += [this](Event::KeyTyped& e)
//    {
//        UpdateScroll();
//    };
//
//    Background(Color{ 255, 255, 255, 255 });
//    EnableScrollbars(false, true);
//    m_Displayer.TextWrap(TextDisplayer::Wrap::Word);
//}
//
//void TextArea::Update(const Vec4<int>& v)
//{
//    ScrollPanel::Update(v);
//
//    if (Focused())
//    {
//        m_FocusedComponent = &m_Displayer;
//        m_Displayer.Focused(true);
//    }
//
//    if (m_Displayer.Dragging())
//    {
//        if (m_EnableY && m_Mouse.y < Height() * 0.1)
//            m_ScrollbarY->Scroll(5 * 0.02 * ((Height() * 0.1 - m_Mouse.y)));
//
//        if (m_EnableY && m_Mouse.y > Height() - Height() * 0.1)
//            m_ScrollbarY->Scroll(-5 * 0.02 * ((m_Mouse.y - (Height() - Height() * 0.1))));
//
//        if (m_EnableX && m_Mouse.x < Width() * 0.1)
//            m_ScrollbarX->Scroll(-5 * 0.02 * ((Width() * 0.1 - m_Mouse.x)));
//
//        if (m_EnableX && m_Mouse.x > Width() - Width() * 0.1)
//            m_ScrollbarX->Scroll(5 * 0.02 * ((m_Mouse.x - (Width() - Width() * 0.1))));
//    }
//}
//
//void TextArea::UpdateScroll()
//{
//    auto pos = m_Displayer.IndexToPosition(m_Displayer.Container().Selection().start);
//    pos.y = m_Displayer.Height() - pos.y;
//
//    if (pos.y > m_ScrollbarY->Value() + Height() - m_Displayer.Padding() * 2 - m_Displayer.LineHeight()) {
//        m_ScrollbarY->Value(pos.y - Height() + m_Displayer.Padding() * 2 + m_Displayer.LineHeight());
//    }
//    else if (pos.y < m_ScrollbarY->Value() + m_Displayer.Padding()) {
//        m_ScrollbarY->Value(pos.y - m_Displayer.Padding());
//    }
//
//    if (pos.x > m_ScrollbarX->Value() + Width() - m_Displayer.Padding() * 2) {
//        m_ScrollbarX->Value(pos.x - Width() + m_Displayer.Padding() * 2);
//    }
//    else if (pos.x < m_ScrollbarX->Value() + m_Displayer.Padding()) {
//        m_ScrollbarX->Value(pos.x - m_Displayer.Padding());
//    }
//}