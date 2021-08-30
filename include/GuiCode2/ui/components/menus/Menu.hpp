#pragma once
#include "GuiCode/pch.hpp"
#include "GuiCode/ui/components/Container.hpp"

// --------------------------------------------------------------------------
// ---------------------------- Menu Base -----------------------------------
// --------------------------------------------------------------------------

class MenuBase : public Container
{
public:
	MenuBase()
		: Container()
	{};

	auto Get() -> std::vector<std::unique_ptr<Component>>& { return m_Components; }
	auto MenuSize() ->                              size_t { return m_Components.size(); }

	bool WithinBounds(const Vec2<int>& pos) const override;
};

// --------------------------------------------------------------------------
// ------------------------------- Menu -------------------------------------
// --------------------------------------------------------------------------

template<typename MenuGraphics, typename MenuType = MenuBase>
class Menu : public MenuType
{
public:
	using MenuType::MenuType;

	void Render(CommandCollection& d) override
	{
		MenuGraphics::Render(*this, d);
		MenuType::Render(d);
	}
};