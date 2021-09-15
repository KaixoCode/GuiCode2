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

		template<std::derived_from<WindowBase> T>
		T& push_back(T&& window)
		{
			return m_Windows.emplace_back(std::forward<T>(window));
		}

		template<std::derived_from<WindowBase> T>
		T& push_back(T& window)
		{
			m_Windows.push_back(window);
			return window;
		}

		bool Loop();
		void Close() { m_Running = false; }

	private:
		bool m_Running = true;
		std::vector<Pointer<WindowBase>> m_Windows;
	};
}