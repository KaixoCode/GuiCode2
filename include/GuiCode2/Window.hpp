#pragma once

#ifdef WIN32
#include "GuiCode2/WindowsWindow.hpp"
namespace GuiCode {
	using Window = WindowsWindow;
}
#endif

namespace GuiCode
{
	class Gui
	{
	public:
		void Add(const Pointer<WindowBase>& window);
		bool Loop();
		void Close() { m_Running = false; }

	private:
		bool m_Running = true;
		std::vector<Pointer<WindowBase>> m_Windows;
	};
}