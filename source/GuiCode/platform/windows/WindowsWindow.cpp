#include "GuiCode/platform/windows/WindowsWindow.hpp"
#include "GuiCode/ui/events/Event.hpp"
#include "GuiCode/ui/graphics/Theme.hpp"
#include "GuiCode/ui/graphics/Graphics.hpp"

int WindowsWindow::m_WindowCount = 0;
int WindowsWindow::m_WindowIdCounter = 0;
GLFWwindow* WindowsWindow::m_MainWindow = nullptr;

WindowsWindow::WindowsWindow(const std::string& name, const int width, const int height)
	: WindowBase(name, width, height)
{
	m_Projection = glm::ortho(0.0f, (float)width, 0.0f, (float)height);
	if (m_WindowCount == 0 && !glfwInit())
	{
		LOG("Failed to initialize GLFW");
		exit(-1);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GL_TRUE);
	glfwWindowHint(GLFW_SAMPLES, 4);

	m_Window = glfwCreateWindow(55, 55, name.c_str(), NULL, WindowsWindow::m_MainWindow);
	m_WindowId = m_WindowIdCounter++;
	if (&m_Window == NULL)
	{
		LOG("Failed to create GLFW window");
		if (--m_WindowCount == 0)
			glfwTerminate();
	}

	glfwMakeContextCurrent(*this);

	if (m_WindowCount == 0)
		WindowsWindow::m_MainWindow = m_Window;

	if (m_WindowCount == 0 && !gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		LOG("Failed to initialize GLAD");
		exit(-1);
	}

	glEnable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_SCISSOR_TEST);
	glfwSwapInterval(0);

	m_WindowCount++;

	// Windows Aero effect on transparent parts
	//const HINSTANCE _hModule = LoadLibrary(TEXT("user32.dll"));
	//if (_hModule)
	//{
	//	struct ACCENTPOLICY
	//	{ int nAccentState; int nFlags; int nColor; int nAnimationId; };
	//	struct WINCOMPATTRDATA
	//	{ int nAttribute; PVOID pData; ULONG ulDataSize; };
	//	typedef BOOL(WINAPI* pSetWindowCompositionAttribute)(HWND, WINCOMPATTRDATA*);
	//	const pSetWindowCompositionAttribute SetWindowCompositionAttribute = (pSetWindowCompositionAttribute)GetProcAddress(_hModule, "SetWindowCompositionAttribute");
	//	if (SetWindowCompositionAttribute)
	//	{
	//		ACCENTPOLICY policy = { 3, 0, 0, 0 }; // ACCENT_ENABLE_BLURBEHIND=3...
	//		WINCOMPATTRDATA data = { 19, &policy, sizeof(ACCENTPOLICY) }; // WCA_ACCENT_POLICY=19
	//		SetWindowCompositionAttribute(GetWin32Handle(), &data);
	//	}
	//	FreeLibrary(_hModule);
	//}

	// Add a subproc to handle some msgs ourselves.
	SetWindowSubclass(GetWin32Handle(), &SubClassProc, m_WindowId, (DWORD_PTR) static_cast<void*>(this));

	// Set the margins of the Win32 window.
	const MARGINS _margins = { 1, 1, 1, 1 };
	DwmExtendFrameIntoClientArea(GetWin32Handle(), &_margins);
}

void WindowsWindow::Update(const Vec4<int>& viewport) 
{
	// Need to trigger a resize after setup because otherwise
	// stuff fails for some reason...
	if (m_InitialResize) { m_InitialResize = false; Size(Width(), Height()); }

	// Go through the even queue
	while (!m_EventQueue.empty())
	{
		AddEvent(m_EventQueue.front());

		// Check again if the eventqueue is empty, because some button somewhere
		// might trigger a resize of the window, which will trigger this WindowsWindow::Update
		// which will finish this while loop, and then exit out of AddEvent about here ^
		// and then m_EventQueue will be empty, causing an exception when calling pop().
		if (!m_EventQueue.empty())
			m_EventQueue.pop();
	}

	// Update all the components
	Container::Update(viewport);

	// Update the cursor after updating the components
	UpdateCursor(Cursor());
}

void WindowsWindow::Render(CommandCollection& d)
{
	using namespace Graphics;
	d.Command<Clip>(0, 0, Width(), Height());
	d.Command<Type::Viewport>(0, 0, Width(), Height());
	d.Command<Fill>(Theme::Window::background);
	d.Command<Quad>(0, 0, Width(), Height());
	WindowBase::Render(d);
}

void WindowsWindow::Loop()
{
	glfwMakeContextCurrent(*this);

	Graphics::CurrentWindow(m_WindowId);
	Graphics::WindowFocused(GetForegroundWindow() == GetWin32Handle());
	Graphics::SetProjection(m_Projection);

	CommandCollection d;
	Update({ 0, 0, Width(), Height() });
	if (Visible())
	{
		Render(d);
		Graphics::RunCommands(d);
	}

	glfwSwapBuffers(*this);
	
	glfwPollEvents();
}

// Hit test the frame for resizing and moving.
LRESULT HitTestNCA(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	// Get the point coordinates for the hit test.
	POINT _ptMouse = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };

	// Get the window rectangle.
	RECT _rcWindow;
	GetWindowRect(hWnd, &_rcWindow);

	// Get the frame rectangle, adjusted for the style without a caption.
	RECT _rcFrame = { 0 };
	AdjustWindowRectEx(&_rcFrame, WS_OVERLAPPEDWINDOW & ~WS_CAPTION, FALSE, NULL);

	// Determine if the hit test is for resizing. Default middle (1,1).
	USHORT _uRow = 1;
	USHORT _uCol = 1;
	bool _fOnResizeBorder = false;

	// Determine if the point is at the top or bottom of the window.
	int _padding = 8;
	if (_ptMouse.y >= _rcWindow.top && _ptMouse.y < _rcWindow.top + 32)
	{
		_fOnResizeBorder = (_ptMouse.y < (_rcWindow.top - _rcFrame.top));
		_uRow = 0;
	}
	else if (_ptMouse.y < _rcWindow.bottom && _ptMouse.y >= _rcWindow.bottom - _padding)
	{
		_uRow = 2;
	}

	// Determine if the point is at the left or right of the window.
	if (_ptMouse.x >= _rcWindow.left && _ptMouse.x < _rcWindow.left + _padding)
		_uCol = 0; // left side

	else if (_ptMouse.x < _rcWindow.right && _ptMouse.x >= _rcWindow.right - _padding)
		_uCol = 2; // right side

	// Hit test (HTTOPLEFT, ... HTBOTTOMRIGHT)
	LRESULT _hitTests[3][3] =
	{
		{ HTTOPLEFT,    _fOnResizeBorder ? HTTOP : HTCAPTION,    HTTOPRIGHT },
		{ HTLEFT,       HTNOWHERE,     HTRIGHT },
		{ HTBOTTOMLEFT, HTBOTTOM, HTBOTTOMRIGHT },
	};

	return _hitTests[_uRow][_uCol];
}

LRESULT CALLBACK WindowsWindow::SubClassProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
	static bool _resizing = true;
	WindowsWindow* _self = (WindowsWindow*)dwRefData;
	bool _fCallDWP = true;
	LRESULT _lRet = 0;

	// Winproc worker for custom frame issues.
	_fCallDWP = !DwmDefWindowProc(hWnd, uMsg, wParam, lParam, &_lRet);

	// Buttons if not on the edge or top bar
	int _offset = IsMaximized(hWnd) ? -8 : 0;
	int _padding = 8;
	if (_self->m_Hovering != nullptr && _self->CursorPos().y > _self->Height() - 32 + _offset || _self->m_MousePressed != Event::MouseButton::NONE ||
		_self->CursorPos().y < _self->Height() - 32 + _offset && _self->CursorPos().x > _padding && _self->CursorPos().x < _self->Width() - _padding && _self->CursorPos().y > _padding)
	{
		bool _dblclk = false;
		switch (uMsg)
		{
		case WM_NCLBUTTONDBLCLK:
		case WM_LBUTTONDBLCLK:
			_dblclk = true;
			[[fallthrough]];
		case WM_NCLBUTTONDOWN:
		case WM_LBUTTONDOWN:
			SetCapture(hWnd);
			_self->MouseButtonCallback(_self, Event::MouseButton::LEFT, Event::Type::MousePressed, _dblclk);
			_fCallDWP = false;
			break;

		case WM_NCLBUTTONUP:
		case WM_LBUTTONUP:
			ReleaseCapture();
			_self->MouseButtonCallback(_self, Event::MouseButton::LEFT, Event::Type::MouseReleased, 0);
			_fCallDWP = false;
			break;

		case WM_NCRBUTTONDBLCLK:
		case WM_RBUTTONDBLCLK:
			_dblclk = true;
			[[fallthrough]];
		case WM_NCRBUTTONDOWN:
		case WM_RBUTTONDOWN:
			SetCapture(hWnd);
			_self->MouseButtonCallback(_self, Event::MouseButton::RIGHT, Event::Type::MousePressed, _dblclk);
			_fCallDWP = false;
			break;

		case WM_NCRBUTTONUP:
		case WM_RBUTTONUP:
			ReleaseCapture();
			_self->MouseButtonCallback(_self, Event::MouseButton::RIGHT, Event::Type::MouseReleased, 0);
			_fCallDWP = false;
			break;

		case WM_NCMBUTTONDBLCLK:
		case WM_MBUTTONDBLCLK:
			_dblclk = true;
			[[fallthrough]];
		case WM_NCMBUTTONDOWN:
		case WM_MBUTTONDOWN:
			SetCapture(hWnd);
			_self->MouseButtonCallback(_self, Event::MouseButton::MIDDLE, Event::Type::MousePressed, _dblclk);
			_fCallDWP = false;
			break;

		case WM_NCMBUTTONUP:
		case WM_MBUTTONUP:
			ReleaseCapture();
			_self->MouseButtonCallback(_self, Event::MouseButton::MIDDLE, Event::Type::MouseReleased, 0);
			_fCallDWP = false;
			break;
		}

		if (_fCallDWP == false && GetFocus() != _self->GetWin32Handle())
			::SetFocus(_self->GetWin32Handle());
	}
	else
	{
		
		// Cheaty instant click when pressing on the window dragging part of the title bar
		switch (uMsg)
		{
		case WM_NCLBUTTONDOWN:
		case WM_LBUTTONDOWN:
			_self->MouseButtonCallback(_self, Event::MouseButton::LEFT, Event::Type::MousePressed, 0);
			_self->MouseButtonCallback(_self, Event::MouseButton::LEFT, Event::Type::MouseReleased, 0);
			break;

		case WM_NCRBUTTONDOWN:
		case WM_RBUTTONDOWN:
			_self->MouseButtonCallback(_self, Event::MouseButton::RIGHT, Event::Type::MousePressed, 0);
			_self->MouseButtonCallback(_self, Event::MouseButton::RIGHT, Event::Type::MouseReleased, 0);
			break;

		case WM_NCMBUTTONDOWN:
		case WM_MBUTTONDOWN:
			_self->MouseButtonCallback(_self, Event::MouseButton::MIDDLE, Event::Type::MousePressed, 0);
			_self->MouseButtonCallback(_self, Event::MouseButton::MIDDLE, Event::Type::MouseReleased, 0);
			break;
		}
	}

	switch (uMsg)
	{
	case WM_KEYDOWN:
	case WM_KEYUP:
	{
		int _mod = ((GetKeyState(VK_CONTROL) & 0x8000) ? Event::Mod::CONTROL : 0)
			| ((GetKeyState(VK_MENU) & 0x8000) ? Event::Mod::ALT : 0)
			| ((GetKeyState(VK_SHIFT) & 0x8000) ? Event::Mod::SHIFT : 0)
			| ((GetKeyState(VK_NUMLOCK) & 0x8000) ? Event::Mod::NUM_LOCK : 0)
			| ((GetKeyState(VK_CAPITAL) & 0x8000) ? Event::Mod::CAPS_LOCK : 0);

		_self->KeyCallback(_self, wParam, 0, uMsg == WM_KEYDOWN ? Event::Type::KeyPressed : Event::Type::KeyReleased, _mod);
		break;
	}
	case WM_MOUSEWHEEL:
	{
		RECT _rect;
		long _x = 0, _y = 0;
		if (GetWindowRect(hWnd, &_rect))
			_x = _rect.left, _y = _rect.top;

		// TODO: translate these mod params to Event params
		int _mod = GET_KEYSTATE_WPARAM(wParam);
		int _amount = GET_WHEEL_DELTA_WPARAM(wParam);
		int _xPos = GET_X_LPARAM(lParam);
		int _yPos = GET_Y_LPARAM(lParam);
		_self->MouseWheelCallback(_self, _amount, _mod, _xPos - _x, _yPos - _y);
		break;
	}
	case WM_EXITSIZEMOVE:
	case WM_SIZE:
	case WM_SIZING:
	{	
		RECT _rect;
		int _width = 0, _height = 0;

		if (GetWindowRect(hWnd, &_rect))
		{
			_width = _rect.right - _rect.left;
			_height = _rect.bottom - _rect.top;
		}
		_self->WindowSizeCallback(_self, _width, _height);
		break;
	}
	case WM_MOUSEMOVE:
	{
		_self->CursorPosCallback(_self, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		break;
	}
	case WM_NCMOUSEMOVE:
	{
		RECT _rect;
		long _x = 0, _y = 0;
		if (GetWindowRect(hWnd, &_rect))
			_x = _rect.left, _y = _rect.top;

		_self->CursorPosCallback(_self, GET_X_LPARAM(lParam) - _x, GET_Y_LPARAM(lParam) - _y);
		break;
	}
	// Handle the non-client size message.
	case WM_NCCALCSIZE:
	{
		if (wParam == TRUE)
			_fCallDWP = false;
		break;
	}
	// Handle hit testing in the NCA if not handled by DwmDefWindowProc.
	case WM_NCHITTEST:
		if (_lRet == 0 && (_lRet = HitTestNCA(hWnd, wParam, lParam)) != HTNOWHERE)
			_fCallDWP = false;
	}

	// Winproc worker for the rest of the application.
	if (_fCallDWP)
		_lRet = DefSubclassProc(hWnd, uMsg, wParam, lParam);

	return _lRet;
}

// --------------------------------------------------------------------------
// ---------------------------- Callbacks -----------------------------------
// --------------------------------------------------------------------------

void WindowsWindow::CursorPosCallback(WindowsWindow* window, int x, int y)
{
	auto _self = window;

	y = _self->Height() - y;

	_self->m_MouseX = x;
	_self->m_MouseY = y;

	if (_self->m_MousePressed != Event::MouseButton::NONE)
		_self->m_EventQueue.emplace(Event::Type::MouseDragged, x, y, _self->m_MousePressed);
	else
		_self->m_EventQueue.emplace(Event::Type::MouseMoved, x, y);
}

void WindowsWindow::MouseButtonCallback(WindowsWindow* window, Event::MouseButton button, Event::Type action, int mod)
{
	auto _self = window;

	if (action == Event::Type::MousePressed)
	{
		_self->m_PressMouseX = _self->m_MouseX;
		_self->m_PressMouseY = _self->m_MouseY;
		_self->m_MousePressed = button;
	}
	else
		_self->m_MousePressed = Event::MouseButton::NONE;

	if (action == Event::Type::MouseReleased && _self->m_MousePressed != Event::MouseButton::NONE
		&& _self->m_PressMouseX == _self->m_MouseX && _self->m_PressMouseY == _self->m_MouseY)
		_self->m_EventQueue.emplace(Event::Type::MouseClicked, _self->m_MouseX, _self->m_MouseY, button, mod);

	_self->m_EventQueue.emplace(action, _self->m_MouseX, _self->m_MouseY, button, mod);
}

void WindowsWindow::MouseWheelCallback(WindowsWindow* window, int amount, int mod, int x, int y)
{
	auto _self = window;

	_self->m_EventQueue.emplace(Event::Type::MouseWheel, x, y, amount, mod);
}

void WindowsWindow::KeyCallback(WindowsWindow* window, int key, int scancode, Event::Type action, int mod)
{
	auto _self = window;

	_self->m_EventQueue.emplace(action, key, mod);
}

void WindowsWindow::WindowSizeCallback(WindowsWindow* window, int width, int height)
{
	auto _self = window;

	_self->m_Size.width = width;
	_self->m_Size.height = height;

	_self->m_Projection = glm::ortho(0.0f, (float)width, 0.0f, (float)height);

	_self->Loop();
}

void WindowsWindow::UpdateCursor(int c)
{
	if (m_Cursorid == c)
		return;

	m_Cursorid = c;

	if (m_GLFWCursor)
		glfwDestroyCursor(m_GLFWCursor);

	if (m_Cursorid == -1)
		glfwSetCursor(*this, NULL);
	else
		glfwSetCursor(*this,
			(m_GLFWCursor = glfwCreateStandardCursor(m_Cursorid)));
}
