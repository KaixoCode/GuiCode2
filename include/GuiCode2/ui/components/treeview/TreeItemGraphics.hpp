#pragma once
#include "GuiCode/pch.hpp"
#include "GuiCode/ui/components/treeview/TreeItem.hpp"
#include "GuiCode/ui/components/treeview/TreeItemType.hpp"
#include "GuiCode/ui/graphics/Graphics.hpp"
#include "GuiCode/ui/graphics/Theme.hpp"

// --------------------------------------------------------------------------
// ------------------------- TreeItem Graphics ------------------------------
// --------------------------------------------------------------------------

namespace TreeItemGraphics
{
	using namespace Graphics;

    // --------------------------------------------------------------------------
    // ------------------------------- Normal -----------------------------------
    // --------------------------------------------------------------------------

    struct Normal
    {
        template<typename TreeItemType>
        static void Render(TreeItem<Normal, TreeItemType>& b, CommandCollection& d) { LOG(typeid(b).name()); }

        template<>
        static void Render(TreeItem<Normal, TreeItemType::Normal>& b, CommandCollection& d)
        {
            Color _c2 = Theme::Menu::button_text * (Graphics::WindowFocused() ? 1.0f : 0.8f);
            Color _c3 = Theme::Menu::keycombo_text * (Graphics::WindowFocused() ? 1.0f : 0.8f);
            Color _c1 = b.Hovering() ? Theme::Menu::button_hover : Theme::Menu::button;
            d.Command<Fill>(_c1);
            d.Command<Quad>(b.Position(), b.Size());

            int _x = b.Position().x + 24;
            int _y = b.Position().y + b.Height() / 2 - 8;
            int _w = 16;
            int _h = 16;
            unsigned int _texture = Graphics::Textures::FileIcon;
            auto& _ext = b.Extension();
            if (_ext == ".wav" || _ext == ".mp3")
                _texture = Graphics::Textures::AudioFileIcon;
            d.Command<TexturedQuad>(_texture, _x, _y, _w, _h);

            d.Command<Font>(Fonts::Gidole14, 14);
            d.Command<Fill>(_c2);
            d.Command<TextAlign>(Align::LEFT, Align::CENTER);
            d.Command<Text>(&b.Name(), b.X() + 48, b.Y() + b.Height() / 2);
            d.Command<Fill>(_c3);
            d.Command<TextAlign>(Align::RIGHT, Align::CENTER);
        }

        template<>
        static void Render(TreeItem<Normal, TreeItemType::Folder>& b, CommandCollection& d)
        {
            Color _c2 = Theme::Menu::button_text * (Graphics::WindowFocused() ? 1.0f : 0.8f);
            Color _c3 = Theme::Menu::keycombo_text * (Graphics::WindowFocused() ? 1.0f : 0.8f);
            Color _c1 = b.Hovering() ? Theme::Menu::button_hover : Theme::Menu::button;
            d.Command<Fill>(_c1);
            d.Command<Quad>(b.X(), b.Y() + b.Height() - b.ActualHeight(), b.Width(), b.ActualHeight());

            int _x = b.X() + 24;
            int _y = b.Y() + b.Height() - b.ActualHeight() / 2 - 8;
            int _w = 16;
            int _h = 16;
            d.Command<TexturedQuad>(Graphics::Textures::FolderIcon, _x, _y, _w, _h);

            d.Command<Font>(Fonts::Gidole14, 14);
            d.Command<Fill>(_c2);
            d.Command<TextAlign>(Align::LEFT, Align::CENTER);
            d.Command<Text>(&b.Name(), b.X() + 48, b.Y() + b.Height() - b.ActualHeight() / 2);
            d.Command<Fill>(_c3);
            _x = b.Position().x + 12;
            _y = b.Position().y + b.Height() - b.ActualHeight() / 2;
            _w = 8;
            _h = 8;
            d.Command<Triangle>(_x, _y, _w, _h, b.Expanded() ? -90.0f : 0.0f);
        }
    };
}