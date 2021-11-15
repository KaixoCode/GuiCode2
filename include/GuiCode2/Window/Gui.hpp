#pragma once
#include "GuiCode2/pch.hpp"
#include "GuiCode2/Window/Window.hpp"

namespace GuiCode
{
	class Gui
	{
	public:

		template<std::derived_from<WindowBase> T>
		T& push(Pointer<T>&& window)
		{
			return m_Windows.emplace_back(std::forward<T>(window));
		}

		template<std::derived_from<WindowBase> T>
		T& push(Pointer<T>& window)
		{
			m_Windows.push_back(window);
			return window;
		}

		template<std::derived_from<WindowBase> T, class ...Args>
		T& emplace(Args&& ...args) requires std::constructible_from<T, Args...>
		{
			return m_Windows.emplace_back(Pointer<T>{ new T{ std::forward<Args>(args)... } });
		}

		bool Loop();
		void Close() { m_Running = false; }

	private:
		bool m_Running = true;
		std::vector<Pointer<WindowBase>> m_Windows;
	};
}