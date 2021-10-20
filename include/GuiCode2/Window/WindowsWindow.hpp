#pragma once
#ifdef WIN32

#include "GuiCode2/Components/Panel.hpp"
#include "GuiCode2/Graphics/OpenGL.hpp"

namespace GuiCode
{
    class Window : public WindowBase
    {
    public:
        using WindowBase::WindowBase;
        ~Window();

        void Create();
        bool Loop() override;

        void Aero(bool b);

        HWND GetWin32Handle() const { return !m_Window ? nullptr : glfwGetWin32Window(m_Window); }

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
        static Window* m_MainWindow;
        friend class Hotkey;
    };

    class Hotkey
    {
    public:
        using Id = size_t;
        using Callback = Function<void(void)>;

        static inline Id fail = static_cast<Id>(-1);

        static inline Id Add(Key k, const Callback& callback)
        {
            if (!Window::m_MainWindow)
                return fail;

            int value = k;
            int key = value & 0x0000ffff;
            int mod = value & 0xffff0000;
            int winMod = 0;
            int id = m_HotkeyCounter++;

            if (mod & Mods::Control) { winMod |= MOD_CONTROL; }
            if (mod & Mods::Alt) { winMod |= MOD_ALT; }
            if (mod & Mods::Shift) { winMod |= MOD_SHIFT; }
            if (mod & Mods::Windows) { winMod |= MOD_WIN; }

            if (RegisterHotKey(Window::m_MainWindow->GetWin32Handle(), id, winMod, key))
                m_Hotkeys.emplace(id, callback);

            return id;
        };

        static inline void Remove(Id id)
        {
            if (!Window::m_MainWindow || id == fail)
                return;

            m_Hotkeys.erase(id);
            UnregisterHotKey(Window::m_MainWindow->GetWin32Handle(), id);
        }
    private:
        static inline std::map<Id, Callback> m_Hotkeys;
        static inline Id m_HotkeyCounter = 0;
        friend class Window;
    };
}

#endif