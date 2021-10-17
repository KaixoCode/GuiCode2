#pragma once
#include "GuiCode2/Components/Panel.hpp"
#include "GuiCode2/Window/WindowBase.hpp"
#include "GuiCode2/Graphics/OpenGL.hpp"

namespace GuiCode
{
    class WindowsWindow : public WindowBase
    {
    public:
        WindowsWindow();
        WindowsWindow(const WindowData&);

        void InitializeWindow(const WindowData& d) override;


        ~WindowsWindow()
        {
            glfwDestroyWindow(m_Window);
        }


        HWND GetWin32Handle() const { return !m_Window ? nullptr : glfwGetWin32Window(m_Window); }


        bool Loop() override;

    private:
        std::queue<std::unique_ptr<Event>> m_EventQueue;
        GLFWwindow* m_Window = nullptr;
        
        bool m_InitialResize = true;
        Dimensions m_PrevDims{ 0, 0, 0, 0 };
        int m_PrevVisibility = 0;
        GLFWcursor* m_GLFWCursor = 0;
        Cursor m_Cursorid = Cursor::Arrow;

        bool WindowsLoop();

        void UpdateCursor(Cursor);

        void CursorPosCallback(int x, int y, int mod);
        void MouseButtonCallback(int button, bool press, int mod);
        void MouseWheelCallback(int amount, int mod, int x, int y);
        void KeyCallback(int key, bool repeat, int action, int mod);
        void WindowSizeCallback(int x, int y, int width, int height);

        static LRESULT SubClassProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);
        static int m_WindowCount;
        static WindowsWindow* m_MainWindow;
    };
}