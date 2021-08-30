#pragma once
#include "GuiCode/ui/components/Container.hpp"

using Callback = std::function<void()>;

// --------------------------------------------------------------------------
// ---------------------------- Button Base ---------------------------------
// --------------------------------------------------------------------------

class ButtonBase : public Container
{
public:
    ButtonBase(const std::string& name = "Button", Vec2<int> size = { 70, 40 }, Key key = Key::NONE)
        : m_KeyCombo(key), m_Name(name), Container(size)
    {}

    operator bool() { return m_Clicked && ((m_Clicked = false) == false); }

    virtual auto Enable()           -> void { m_Enabled = true; }
    virtual auto Disable()          -> void { m_Enabled = false; }
    virtual auto Active(bool a)     -> void { m_Active = a; }

    virtual bool Enabled()    const { return m_Enabled; }
    virtual bool Disabled()   const { return !m_Enabled; }
    virtual bool Active()     const { return m_Active; }
    virtual bool Hovering()   const { return m_Hovering; }
    Key          KeyCombo()   const { return m_KeyCombo; }
    const std::string& Name() const { return m_Name; }

protected:
    bool m_Clicked = false,
        m_Active = false,
        m_Hovering = false,
        m_Enabled = true;

    Key m_KeyCombo;
    std::string m_Name;
};

// --------------------------------------------------------------------------
// ------------------------------ Button ------------------------------------
// --------------------------------------------------------------------------

template<typename ButtonGraphics, typename ButtonType = ButtonBase>
struct Button : public ButtonType
{
	using ButtonType::ButtonType;

	void Render(CommandCollection& d) override
	{
        ButtonType::Render(d);
		ButtonGraphics::Render(*this, d);
	};
};

