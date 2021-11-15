#include "GuiCode2/Window/Gui.hpp"
#include "GuiCode2/Utils/ContextMenu.hpp"

namespace GuiCode
{
	bool Gui::Loop()
	{
		LIMIT_FPS(60);
		for (int i = m_Windows.size() - 1; i >= 0; i--)
		{
			auto& _w = m_Windows[i];
			if (!_w->Loop())
			{
				if (!_w->settings.hideOnClose)
					m_Running = false;

				else
					_w->State(Visible) = Hide;
			}
		}

		ContextMenu::Loop();

		return m_Running;
	}
}