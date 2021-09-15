#include "GuiCode2/Window.hpp"
#include "GuiCode2/ContextMenu.hpp"

namespace GuiCode
{
	void Gui::Add(const Pointer<WindowBase>& window)
	{
		m_Windows.push_back(window);
	}

	bool Gui::Loop()
	{
		LIMIT_FPS(60);
		for (int i = m_Windows.size() - 1; i >= 0; i--)
		{
			auto& _w = m_Windows[i];
			if (!_w->Loop())
				m_Running = false;
		}

		ContextMenu::Loop();

		return m_Running;
	}
}