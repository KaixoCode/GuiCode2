#pragma once
#include "GuiCode2/Panel.hpp"

namespace GuiCode
{
	class WindowBase
	{

		
		Panel panel;
	};

	class WindowsWindow : public WindowBase
	{
		WindowsWindow();

		HWND GetWin32Handle() const { return glfwGetWin32Window(m_Window); }

		std::queue<Event> m_EventQueue;
		GLFWwindow* m_Window;
		int m_WindowId;

		static void CursorPosCallback(WindowsWindow* window, int x, int y);
		static void MouseButtonCallback(WindowsWindow* window, int button, int action, int mod);
		static void MouseWheelCallback(WindowsWindow* window, int amount, int mod, int x, int y);
		static void KeyCallback(WindowsWindow* window, int key, int scancode, int action, int mod);
		static void WindowSizeCallback(WindowsWindow* window, int width, int height);

		static LRESULT SubClassProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);
		static int m_WindowCount, m_WindowIdCounter;
	};
}