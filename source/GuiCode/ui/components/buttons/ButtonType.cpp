#include "GuiCode/ui/components/buttons/ButtonType.hpp"

// --------------------------------------------------------------------------
// ---------------------------- Button Type ---------------------------------
// --------------------------------------------------------------------------

namespace ButtonType
{
    // --------------------------------------------------------------------------
    // ------------------------------- Normal -----------------------------------
    // --------------------------------------------------------------------------
    
    Normal::Normal(Callback c, const std::string& name, Vec2<int> size, Key key)
        : ButtonBase(name, size, key), m_Callback(c)
    {
        m_Listener += [this](Event::MousePressed& event)
        {
            if (event.button == Event::MouseButton::LEFT && Component::WithinBounds({ event.x, event.y }))
                m_Active = true;
        };

        m_Listener += [this](Event::MouseExited& event)
        {
            m_Hovering = false;
        };

        m_Listener += [this](Event::MouseEntered& event)
        {
            m_Hovering = true;
        };

        m_Listener += [this](Event::MouseReleased& event)
        {
            if (event.button == Event::MouseButton::LEFT)
            {
                m_Active = false;
                if (!Disabled() && Component::WithinBounds({ event.x, event.y }))
                {
                    m_Callback();
                    m_Clicked = true;
                }
            }
        };

        m_Listener += [this](Event::KeyPressed& event)
        {
            if (m_KeyCombo == event && !Disabled())
                m_Callback();
        };
    }

    // --------------------------------------------------------------------------
    // ------------------------------- Toggle -----------------------------------
    // --------------------------------------------------------------------------

    Toggle::Toggle(bool* a, const std::string& name, Vec2<int> size, Key key)
        : m_Link(a), ButtonBase(name, size, key)
    {
        SetupCallbacks();
    };

    Toggle::Toggle(const std::string& name, Vec2<int> size, Key key)
        : ButtonBase(name, size, Key::NONE)
    {
        SetupCallbacks();
    };

    void Toggle::Update(const Vec4<int>& viewport)
    {
        if (m_Link)
            m_Active = *m_Link;
        ButtonBase::Update(viewport);
    }

    void Toggle::SetupCallbacks()
    {
        m_Listener += [this](Event::MouseExited& event)
        {
            m_Hovering = false;
        };

        m_Listener += [this](Event::MouseEntered& event)
        {
            m_Hovering = true;
        };

        m_Listener += [this](Event::KeyPressed& event)
        {
            if (m_KeyCombo == event && !Disabled())
            {
                if (m_Link)
                {
                    *m_Link ^= true;
                    m_Active = *m_Link;
                }
                else
                    m_Active ^= true;
            }
        };

        m_Listener += [this](Event::MousePressed& event)
        {
            if (event.button == Event::MouseButton::LEFT && !Disabled() && Component::WithinBounds({ event.x, event.y }))
            {
                if (m_Link)
                {
                    *m_Link ^= true;
                    m_Active = *m_Link;
                }
                else
                    m_Active ^= true;
            }
        };
    }

    // --------------------------------------------------------------------------
    // ---------------------------- Focus Toggle --------------------------------
    // --------------------------------------------------------------------------

    void FocusToggle::SetupCallbacks()
    {
        m_Listener += [this](Event::Unfocused& event)
        {
            Active(false);
        };
    }

    void FocusToggle::Update(const Vec4<int>& viewport)
    {
        if (!::Graphics::WindowFocused())
            Active(false);

        Toggle::Update(viewport);
    }

    // --------------------------------------------------------------------------
    // -------------------------------- Hover -----------------------------------
    // --------------------------------------------------------------------------

    Hover::Hover(const std::string& name, Vec2<int> size)
        : ButtonBase(name, size, Key::NONE)
    {
        m_Listener += [this](Event::MouseEntered& event)
        {
            m_Hovering = true;
            m_Active = true;
            
        };

        m_Listener += [this](Event::MouseExited& event)
        {
            m_Hovering = false;
            m_Active = false;
        };
    };
}