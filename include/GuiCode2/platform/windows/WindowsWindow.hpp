#include "GuiCode/ui/window/WindowBase.hpp"

class WindowsWindow : public WindowBase
{
public:
	WindowsWindow(const std::string& name, const int width, const int height);
	~WindowsWindow() 
	{
		if (--m_WindowCount == 0)
			glfwTerminate();
	}
	
	auto Size(int w, int h)  -> void override { glfwSetWindowSize(*this, w, h); }
	auto Size(Vec2<int> s)   -> void override { glfwSetWindowSize(*this, s.width, s.height); }
	auto Maximize()          -> void override { glfwMaximizeWindow(*this); }
    auto Restore()           -> void override { glfwRestoreWindow(*this); }
    auto Iconify()           -> void override { glfwIconifyWindow(*this); }
    auto Close()             -> void override { glfwSetWindowShouldClose(*this, GL_TRUE); }
    auto Show()              -> void override { ShowWindow(GetWin32Handle(), SW_SHOW); }
    auto Hide()              -> void override { ShowWindow(GetWin32Handle(), SW_HIDE); }
	auto ShouldClose(bool b) -> void override { glfwSetWindowShouldClose(m_Window, b); };

	operator    GLFWwindow* ()   const { return GetWindow(); }
	GLFWwindow* GetWindow()      const { return m_Window; }
	HWND        GetWin32Handle() const { return glfwGetWin32Window(*this); }
	Vec2<int>   Size()           const override { return { Width(), Height() }; }
	Vec2<int>   Location()       const override { int x, y; glfwGetWindowPos(m_Window, &x, &y); return { x, y }; }
	Vec2<int>   CursorPos()      const override { return { m_MouseX, m_MouseY }; }
	int         Width()          const override { return m_Size.width; }
	int         Height()         const override { return m_Size.height; }
    bool        Maximized()      const override { return IsMaximized(GetWin32Handle()); }
    bool        Visible()        const override { return IsWindowVisible(GetWin32Handle()); }
	bool        ShouldClose()    const override { return glfwWindowShouldClose(m_Window); };

	void Loop() override;
	void Update(const Vec4<int>& viewport) override;
	void Render(CommandCollection&) override;

private:
	bool m_InitialResize = true;

	GLFWwindow* m_Window;

	Event::MouseButton m_MousePressed = Event::MouseButton::NONE;
	int m_PressMouseX = 0,
		m_PressMouseY = 0,
		m_MouseX = 0,
		m_MouseY = 0,
		m_WindowId = -1,
		m_Cursorid = 0;
	
	GLFWcursor* m_GLFWCursor = 0;
	glm::mat4 m_Projection;

	std::queue<Event> m_EventQueue;

	void UpdateCursor(int);

	static void CursorPosCallback(WindowsWindow* window, int x, int y);
	static void MouseButtonCallback(WindowsWindow* window, Event::MouseButton button, Event::Type action, int mod);
	static void MouseWheelCallback(WindowsWindow* window, int amount, int mod, int x, int y);
	static void KeyCallback(WindowsWindow* window, int key, int scancode, Event::Type action, int mod);
	static void WindowSizeCallback(WindowsWindow* window, int width, int height);

	static LRESULT SubClassProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);
	static int m_WindowCount, m_WindowIdCounter;
	static GLFWwindow* m_MainWindow;
};




