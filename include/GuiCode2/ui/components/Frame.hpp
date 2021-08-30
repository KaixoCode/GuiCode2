#pragma once
#include "GuiCode/ui/window/Window.hpp"
#include "GuiCode/ui/components/panels/Panel.hpp"
#include "GuiCode/ui/components/buttons/Button.hpp"
#include "GuiCode/ui/components/buttons/ButtonGraphics.hpp"
#include "GuiCode/ui/components/buttons/ButtonType.hpp"
#include "GuiCode/ui/components/menus/Menu.hpp"
#include "GuiCode/ui/components/menus/MenuGraphics.hpp"
#include "GuiCode/ui/components/menus/MenuType.hpp"
#include "GuiCode/ui/graphics/Graphics.hpp"

// --------------------------------------------------------------------------
// ------------------------------- Frame ------------------------------------
// --------------------------------------------------------------------------

class Frame : public Window 
{
public:
	Frame(const std::string& name, const int width, const int height)
		: Window(name, width, height), 
		m_CloseButton(&Emplace<Button<ButtonGraphics::Textured<>, ButtonType::Textured<ButtonType::Normal>>>(Graphics::Textures::Cross1, Graphics::Textures::Cross2, Graphics::Textures::Cross3, [&]() { this->Close(); }, "")),
		m_MaxiButton1(&Emplace<Button<ButtonGraphics::Textured<>, ButtonType::Textured<ButtonType::Normal>>>(Graphics::Textures::Maxi1, Graphics::Textures::Maxi2, Graphics::Textures::Maxi3, [&]() { this->Restore(); }, "")),
		m_MaxiButton2(&Emplace<Button<ButtonGraphics::Textured<>, ButtonType::Textured<ButtonType::Normal>>>(Graphics::Textures::Maxi4, Graphics::Textures::Maxi5, Graphics::Textures::Maxi6, [&]() { this->Maximize(); }, "")),
		m_MinimButton(&Emplace<Button<ButtonGraphics::Textured<>, ButtonType::Textured<ButtonType::Normal>>>(Graphics::Textures::Mini1, Graphics::Textures::Mini2, Graphics::Textures::Mini3, [&]() { this->Iconify(); }, "")),
		m_Menu(&Emplace<::Menu<MenuGraphics::Horizontal, MenuType::ToggleHover>>()),
		m_Panel(&Emplace<::Panel>())
	{}

	void Update(const Vec4<int>& viewport) override;
	void Render(CommandCollection& d) override;
	MenuBase& Menu() { return *m_Menu; }
	::Panel& Panel() { return *m_Panel; }

private:
	::Panel* m_Panel;

	ButtonBase* m_CloseButton;
	ButtonBase* m_MaxiButton1;
	ButtonBase* m_MaxiButton2;
	ButtonBase* m_MinimButton;
	MenuBase* m_Menu;
};