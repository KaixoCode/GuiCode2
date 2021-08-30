#pragma once
#include "GuiCode2/Panel.hpp"
#include "GuiCode2/WindowBase.hpp"
#include "GuiCode2/Graphics.hpp"

namespace GuiCode
{
	class WindowsWindow;
	using Window = WindowsWindow;


	class WindowsWindow : public WindowBase
	{
	public:
		WindowsWindow(const WindowData&);





		HWND GetWin32Handle() const { return glfwGetWin32Window(m_Window); }


		void Loop() override
		{
            static int _currentId = -1;

            // Only change context if it's necessary, because this seems to
            // have a relatively high impact on the CPU usage.
            //if (_currentId != m_WindowId && Visible())
            //{
            //    _currentId = m_WindowId;
                glfwMakeContextCurrent(m_Window);
            //}

            //if (Visible())
            //{
            //    if (RightClickMenu::Get().GetWin32Handle() != GetWin32Handle() && RightClickMenu::Get().Opened())
            //        m_MousePressed = Event::MouseButton::NONE;

                CurrentWindow(m_WindowId);
                //Graphics::Scaling(m_Scale);
                //WindowFocused(GetForegroundWindow() == GetWin32Handle());
                SetProjection(m_Projection);

                //panel.Update({ 0, 0, Width(), Height() });
            //}

            //if (Visible())
            //{

                //d.Command<Graphics::FrameBuffer>(9999, true, Vec4<int>{0, 0, Width(), Height()});
                CommandCollection d;
                //d.Get().reserve(m_EstimateCommandSize);
                d.Clip({ 0, 0, width, height });
                d.Viewport({ 0, 0, width, height });
                panel.listener(Render{ d });
                d.PopClip();
                //m_EstimateCommandSize = d.Get().size() + 100;
                //d.Command<Graphics::FrameBufferEnd>();
                RunCommands(d);
                glfwSwapBuffers(m_Window);


                // Need to trigger a resize after setup because otherwise
                // stuff fails for some reason...
                //if (Visible() && m_InitialResize)
                //{
                //    m_InitialResize = false;
                //    Size(m_InitialSize);
                //}

                // Go through the even queue
                while (!m_EventQueue.empty())
                {
                    panel.listener(*m_EventQueue.front());

                    // Check again if the eventqueue is empty, because some button somewhere
                    // might trigger a resize of the window, which will trigger this WindowsWindow::Update
                    // which will finish this while loop, and then exit out of AddEvent about here ^
                    // and then m_EventQueue will be empty, causing an exception when calling pop().
                    if (!m_EventQueue.empty())
                        m_EventQueue.pop();
                }
            //}
			glfwPollEvents();
		}





	private:
		std::queue<std::unique_ptr<Event>> m_EventQueue;
		GLFWwindow* m_Window;
		int m_WindowId;

		glm::mat4 m_Projection;

		void CursorPosCallback(int x, int y, int mod);
		void MouseButtonCallback(int button, bool press, int mod);
		void MouseWheelCallback(int amount, int mod, int x, int y);
		void KeyCallback(int key, bool repeat, int action, int mod);
		void WindowSizeCallback(int width, int height);

		static LRESULT SubClassProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);
		static int m_WindowCount, m_WindowIdCounter;
	};
}