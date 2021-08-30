#pragma once
#include "GuiCode/ui/components/Container.hpp"
#include "GuiCode/ui/components/Layout.hpp"

// --------------------------------------------------------------------------
// -------------------------------- Panel -----------------------------------
// --------------------------------------------------------------------------

class Panel : public Container 
{
public:
	Panel(Vec2<int> size = { 40, 70 });

	auto Background(const Color& c) -> void { m_Background = c; };

	void Update(const Vec4<int>& viewport) override;
	void Render(CommandCollection& d) override;
	virtual void Background(CommandCollection& d);

	template<Layout::Type T, typename ...Args>
	void Layout(Args&& ...args) { m_LayoutManager.Layout<T>(std::forward<Args>(args)...); }

	void AutoResize(bool rx, bool ry) { m_AutoResizeX = rx, m_AutoResizeY = ry; };
	Vec2<bool> AutoResize() const { return { m_AutoResizeX, m_AutoResizeY }; };

private:
	::Color m_Background = { 0, 0, 0, 0 };
	LayoutManager m_LayoutManager;
	bool m_AutoResizeX = false, m_AutoResizeY = false;
};