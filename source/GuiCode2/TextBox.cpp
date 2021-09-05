//#include "GuiCode/ui/components/text/TextBox.hpp"
//
//// --------------------------------------------------------------------------
//// -------------------------------- TextBox ---------------------------------
//// --------------------------------------------------------------------------
//
//TextBox::TextBox()
//    : m_Displayer(Emplace<TextDisplayer>())
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
//    m_Displayer.TextWrap(TextDisplayer::Wrap::None);
//}
//
//void TextBox::Update(const Vec4<int>& v)
//{
//    m_Displayer.LineHeight(Height() - 2 * Padding());
//    if (!m_Displayer.Focused() && Focused())
//    {
//        m_Displayer.Focused(true);
//        m_FocusedComponent = &m_Displayer;
//    }
//    Panel::Update(v);
//    m_Displayer.Position({ -m_ScrollX, 0 });
//    m_Displayer.Height(Height());
//
//    m_Displayer.MinWidth(Width());
//
//    if (m_Displayer.Dragging() && m_Displayer.Width() - m_Displayer.Padding() * 2 > Width())
//    {
//        if (m_Mouse.x < Width() * 0.1)
//            m_ScrollX += -5 * 0.02 * ((Width() * 0.1 - m_Mouse.x));
//
//        if (m_Mouse.x > Width() - Width() * 0.1)
//            m_ScrollX += 5 * 0.02 * ((m_Mouse.x - (Width() - Width() * 0.1)));
//    }
//
//    m_ScrollX = constrain(m_ScrollX, 0, std::max(m_Displayer.Width() - Width(), 0));
//}
//
//void TextBox::Render(CommandCollection& d)
//{
//    using namespace Graphics;
//    d.Command<Clip>(X(), Y(), Width(), Height());
//    d.Command<PushMatrix>();
//    d.Command<Translate>(Vec2<int>{ X(), Y() });
//    Background(d);
//    m_Displayer.Render(d);
//    d.Command<PopMatrix>();
//    d.Command<PopClip>();
//    NeedsRedraw(false);
//    Component::Render(d);
//}
//    
//void TextBox::UpdateScroll()
//{
//    auto pos = m_Displayer.IndexToPosition(m_Displayer.Container().Selection().start);
//
//    if (pos.x > m_ScrollX + Width() - m_Displayer.Padding() * 2) {
//        m_ScrollX = pos.x - Width() + m_Displayer.Padding() * 2;
//    }
//    else if (pos.x < m_ScrollX + m_Displayer.Padding()) {
//        m_ScrollX = pos.x - m_Displayer.Padding();
//    }
//}
