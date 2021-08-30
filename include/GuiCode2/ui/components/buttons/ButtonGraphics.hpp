#pragma once
#include "GuiCode/pch.hpp"
#include "GuiCode/ui/components/buttons/Button.hpp"
#include "GuiCode/ui/components/buttons/ButtonType.hpp"
#include "GuiCode/ui/graphics/Graphics.hpp"
#include "GuiCode/ui/graphics/Theme.hpp"

// --------------------------------------------------------------------------
// -------------------------- Button Graphics -------------------------------
// --------------------------------------------------------------------------

namespace ButtonGraphics
{
    using namespace Graphics;

    // --------------------------------------------------------------------------
    // ------------------------------- Normal -----------------------------------
    // --------------------------------------------------------------------------

    template<Align Hori = Align::CENTER, Align Vert = Align::CENTER>
    class Normal
    {
        static void Render(ButtonBase& b, CommandCollection& d)
        {
            // TODO    
        }
    };

    // --------------------------------------------------------------------------
    // -------------------------------- Menu ------------------------------------
    // --------------------------------------------------------------------------

    struct Menu
    {
        static void Render(ButtonBase& b, CommandCollection& d);
    };

    // --------------------------------------------------------------------------
    // ------------------------------ Sub Menu ----------------------------------
    // --------------------------------------------------------------------------

    struct SubMenu
    {
        static void Render(ButtonBase& b, CommandCollection& d);
    };

    // --------------------------------------------------------------------------
    // ----------------------------- Title Menu ---------------------------------
    // --------------------------------------------------------------------------

    struct TitleMenu
    {
        static void Render(ButtonBase& b, CommandCollection& d);
    };

    // --------------------------------------------------------------------------
    // ------------------------------ Textured ----------------------------------
    // --------------------------------------------------------------------------

    template<Align Hori = Align::CENTER, Align Vert = Align::CENTER>
    struct Textured
    {
        template<typename Type = ButtonBase>
        static void Render(ButtonType::Textured<Type>& b, CommandCollection& d)
        {
            Graphics::Texture& _t = b.Active() ? b.m_Texture3 : b.Hovering() ? b.m_Texture2 : b.m_Texture1;
            b.Size({ _t.Width, _t.Height });
            d.Command<TexturedQuad>(_t, b.X(), b.Y(), b.Width(), b.Height());
            d.Command<Fill>(Color{ 1.0f, 1.0f, 1.0f, 1.0f });
            d.Command<Font>(Fonts::Gidole14, 14);
            d.Command<TextAlign>(Hori, Vert);
            if (Hori == Align::CENTER)
                d.Command<Text>(&b.Name(), b.X() + b.Width() / 2, b.Y() + b.Height() / 2);
            else if (Hori == Align::RIGHT)
                d.Command<Text>(&b.Name(), b.X() + b.Width() - 6, b.Y() + b.Height() / 2);
            else if (Hori == Align::LEFT)
                d.Command<Text>(&b.Name(), b.X() + 6, b.Y() + b.Height() / 2);
        }
    };
}
