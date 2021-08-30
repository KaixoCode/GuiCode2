#include "GuiCode/ui/components/buttons/ButtonGraphics.hpp"

// --------------------------------------------------------------------------
// -------------------------- Button Graphics -------------------------------
// --------------------------------------------------------------------------

namespace ButtonGraphics
{
    using namespace Graphics;

    // --------------------------------------------------------------------------
    // -------------------------------- Menu ------------------------------------
    // --------------------------------------------------------------------------

    void Menu::Render(ButtonBase& b, CommandCollection& d)
    {
        int _padding = 20;
        Color _c1;
        Color _c2 = (b.Disabled() ? Theme::Menu::disabled_button_text : Theme::Menu::button_text) * (Graphics::WindowFocused() ? 1.0f : 0.8f);
        Color _c3 = (b.Disabled() ? Theme::Menu::disabled_keycombo_text : Theme::Menu::keycombo_text) * (Graphics::WindowFocused() ? 1.0f : 0.8f);

        // TODO: get rid of these ugly ass dynamic casts
        if (dynamic_cast<::ButtonType::Toggle*>(&b) != nullptr)
        {
            _c1 = b.Disabled() ?
                b.Hovering() ? Theme::Menu::disabled_button_hover : Theme::Menu::disabled_button
                :
                b.Hovering() ? Theme::Menu::button_hover : Theme::Menu::button;

            d.Command<Fill>(_c1);
            d.Command<Quad>(b.Position(), b.Size());

            if (b.Active())
            {
                d.Command<Fill>(Theme::Menu::button_press);
                int _p = 2;
                d.Command<Quad>(b.Position() + Vec2<int>{ _p, _p }, Vec2<int>{ b.Size().y - _p * 2, b.Size().y - _p * 2 });
            }
        }
        else if (dynamic_cast<::ButtonType::Hover*>(&b) != nullptr)
        {
            _c1 = b.Disabled() ?
                b.Hovering() ? Theme::Menu::disabled_button_hover : Theme::Menu::disabled_button
                :
                b.Hovering() ? Theme::Menu::button_hover : Theme::Menu::button;
            d.Command<Fill>(_c1);
            d.Command<Quad>(b.Position(), b.Size());
        }
        else
        {
            _c1 = b.Disabled() ?
                b.Active() ? Theme::Menu::disabled_button_press : b.Hovering() ? Theme::Menu::disabled_button_hover : Theme::Menu::disabled_button
                :
                b.Active() ? Theme::Menu::button_press : b.Hovering() ? Theme::Menu::button_hover : Theme::Menu::button;
            d.Command<Fill>(_c1);
            d.Command<Quad>(b.Position(), b.Size());
        }

        d.Command<Font>(Fonts::Gidole14, 14);
        d.Command<Fill>(_c2);
        d.Command<TextAlign>(Align::LEFT, Align::CENTER);
        d.Command<Text>(&b.Name(), _padding + b.X() + 6, b.Y() + b.Height() / 2);
        d.Command<Fill>(_c3);
        d.Command<TextAlign>(Align::RIGHT, Align::CENTER);
        if (b.KeyCombo() != Key::NONE)
            d.Command<Text>(&b.KeyCombo().ToString(), b.X() + b.Width() - 6, b.Y() + b.Height() / 2);
    }

    // --------------------------------------------------------------------------
    // ------------------------------ Sub Menu ----------------------------------
    // --------------------------------------------------------------------------

    void SubMenu::Render(ButtonBase& b, CommandCollection& d)
    {
        Menu::Render(b, d);
        float _w = 7;
        float _padding = 4;
        d.Command<Triangle>(b.X() + b.Width() - _w - _padding, b.Y() + b.Height() / 2 - _w / 2, _w / 2, _w, 0.0f);
    }
    
    // --------------------------------------------------------------------------
    // ----------------------------- Title Menu ---------------------------------
    // --------------------------------------------------------------------------

    void TitleMenu::Render(ButtonBase& b, CommandCollection& d)
    {
        Color _c1 = b.Active() ? Theme::TitleBar::button_press : b.Hovering() ? Theme::TitleBar::button_hover : Theme::TitleBar::button;
        Color _c2 = Theme::TitleBar::button_text * (Graphics::WindowFocused() ? 1.0f : 0.8f);
        float _padding = 20;
        if (b.Active())
        {
            d.Command<Fill>(Theme::TitleBar::button_border);
            d.Command<Quad>(b.X() - 1, b.Y(), b.Width() + 2, b.Height());
        }
        d.Command<Fill>(_c1);
        d.Command<Quad>(b.X(), b.Y(), b.Width(), b.Height() - 1);
        d.Command<Fill>(_c2);
        d.Command<Font>(Fonts::Gidole14, 14);
        d.Command<TextAlign>(Align::CENTER, Align::CENTER);
        d.Command<Text>(&b.Name(), b.X() + b.Width() / 2, b.Y() + b.Height() / 2);
    }
}
