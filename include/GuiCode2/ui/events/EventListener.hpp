#pragma once
#include "GuiCode/pch.hpp"
#include "GuiCode/ui/events/Event.hpp"

class EventListener {
public:
    template<typename T>
    using Callback = std::function<void(T&)>;

    void AddCallback(Callback<Event> c) { m_EventCallbacks.emplace_back(c); }
    void AddCallback(Callback<Event::KeyPressed> c) { m_KeyPressedCallbacks.emplace_back(c); }
    void AddCallback(Callback<Event::KeyReleased> c) { m_KeyReleasedCallbacks.emplace_back(c); }
    void AddCallback(Callback<Event::MousePressed> c) { m_MousePressedCallbacks.emplace_back(c); }
    void AddCallback(Callback<Event::MouseReleased> c) { m_MouseReleasedCallbacks.emplace_back(c); }
    void AddCallback(Callback<Event::MouseClicked> c) { m_MouseClickedCallbacks.emplace_back(c); }
    void AddCallback(Callback<Event::MouseMoved> c) { m_MouseMovedCallbacks.emplace_back(c); }
    void AddCallback(Callback<Event::MouseDragged> c) { m_MouseDraggedCallbacks.emplace_back(c); }
    void AddCallback(Callback<Event::MouseWheel> c) { m_MouseWheelCallbacks.emplace_back(c); }
    void AddCallback(Callback<Event::MouseEntered> c) { m_MouseEnteredCallbacks.emplace_back(c); }
    void AddCallback(Callback<Event::MouseExited> c) { m_MouseExitedCallbacks.emplace_back(c); }
    void AddCallback(Callback<Event::Focused> c) { m_FocusedCallbacks.emplace_back(c); }
    void AddCallback(Callback<Event::Unfocused> c) { m_UnfocusedCallbacks.emplace_back(c); }

    void AddEvent(Event& e)
    {
        for (auto& a = m_EventCallbacks.rbegin(); a != m_EventCallbacks.rend(); ++a)
            a->operator()(e);

        switch (e.type)
        {
        case Event::Type::KeyPressed:    AddEvent(e.keyPressed);    break;
        case Event::Type::KeyReleased:   AddEvent(e.keyReleased);   break;
        case Event::Type::MousePressed:  AddEvent(e.mousePressed);  break;
        case Event::Type::MouseReleased: AddEvent(e.mouseReleased); break;
        case Event::Type::MouseClicked:  AddEvent(e.mouseClicked);  break;
        case Event::Type::MouseMoved:    AddEvent(e.mouseMoved);    break;
        case Event::Type::MouseDragged:  AddEvent(e.mouseDragged);  break;
        case Event::Type::MouseWheel:    AddEvent(e.mouseWheel);    break;
        case Event::Type::MouseEntered:  AddEvent(e.mouseEntered);  break;
        case Event::Type::MouseExited:   AddEvent(e.mouseExited);   break;
        case Event::Type::Focused:       AddEvent(e.focused);       break;
        case Event::Type::Unfocused:     AddEvent(e.unfocused);     break;
        }
    }

    void AddEvent(Event::KeyPressed& e) { for (auto& a = m_KeyPressedCallbacks.rbegin(); a != m_KeyPressedCallbacks.rend(); ++a) a->operator()(e); }
    void AddEvent(Event::KeyReleased& e) { for (auto& a = m_KeyReleasedCallbacks.rbegin(); a != m_KeyReleasedCallbacks.rend(); ++a) a->operator()(e); }
    void AddEvent(Event::MousePressed& e) { for (auto& a = m_MousePressedCallbacks.rbegin(); a != m_MousePressedCallbacks.rend(); ++a) a->operator()(e); }
    void AddEvent(Event::MouseReleased& e) { for (auto& a = m_MouseReleasedCallbacks.rbegin(); a != m_MouseReleasedCallbacks.rend(); ++a) a->operator()(e); }
    void AddEvent(Event::MouseClicked& e) { for (auto& a = m_MouseClickedCallbacks.rbegin(); a != m_MouseClickedCallbacks.rend(); ++a) a->operator()(e); }
    void AddEvent(Event::MouseMoved& e) { for (auto& a = m_MouseMovedCallbacks.rbegin(); a != m_MouseMovedCallbacks.rend(); ++a) a->operator()(e); }
    void AddEvent(Event::MouseDragged& e) { for (auto& a = m_MouseDraggedCallbacks.rbegin(); a != m_MouseDraggedCallbacks.rend(); ++a) a->operator()(e); }
    void AddEvent(Event::MouseWheel& e) { for (auto& a = m_MouseWheelCallbacks.rbegin(); a != m_MouseWheelCallbacks.rend(); ++a) a->operator()(e); }
    void AddEvent(Event::MouseEntered& e) { for (auto& a = m_MouseEnteredCallbacks.rbegin(); a != m_MouseEnteredCallbacks.rend(); ++a) a->operator()(e); }
    void AddEvent(Event::MouseExited& e) { for (auto& a = m_MouseExitedCallbacks.rbegin(); a != m_MouseExitedCallbacks.rend(); ++a) a->operator()(e); }
    void AddEvent(Event::Focused& e) { for (auto& a = m_FocusedCallbacks.rbegin(); a != m_FocusedCallbacks.rend(); ++a) a->operator()(e); }
    void AddEvent(Event::Unfocused& e) { for (auto& a = m_UnfocusedCallbacks.rbegin(); a != m_UnfocusedCallbacks.rend(); ++a) a->operator()(e); }

    template<typename T>
    void operator +=(T t) { AddCallback(t); }

private:
    std::vector<Callback<Event>> m_EventCallbacks;
    std::vector<Callback<Event::KeyPressed>> m_KeyPressedCallbacks;
    std::vector<Callback<Event::KeyReleased>> m_KeyReleasedCallbacks;
    std::vector<Callback<Event::MousePressed>> m_MousePressedCallbacks;
    std::vector<Callback<Event::MouseReleased>> m_MouseReleasedCallbacks;
    std::vector<Callback<Event::MouseClicked>> m_MouseClickedCallbacks;
    std::vector<Callback<Event::MouseMoved>> m_MouseMovedCallbacks;
    std::vector<Callback<Event::MouseDragged>> m_MouseDraggedCallbacks;
    std::vector<Callback<Event::MouseWheel>> m_MouseWheelCallbacks;
    std::vector<Callback<Event::MouseEntered>> m_MouseEnteredCallbacks;
    std::vector<Callback<Event::MouseExited>> m_MouseExitedCallbacks;
    std::vector<Callback<Event::Focused>> m_FocusedCallbacks;
    std::vector<Callback<Event::Unfocused>> m_UnfocusedCallbacks;
};
