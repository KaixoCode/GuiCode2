#include "GuiCode2/WindowsWindow.hpp"

namespace GuiCode
{

    GLFWmonitor* get_current_monitor(GLFWwindow* window)
    {
        int nmonitors, i;
        int wx, wy, ww, wh;
        int mx, my, mw, mh;
        int overlap, bestoverlap;
        GLFWmonitor* bestmonitor;
        GLFWmonitor** monitors;
        const GLFWvidmode* mode;

        bestoverlap = 0;
        bestmonitor = NULL;

        glfwGetWindowPos(window, &wx, &wy);
        glfwGetWindowSize(window, &ww, &wh);
        monitors = glfwGetMonitors(&nmonitors);

        for (i = 0; i < nmonitors; i++) {
            mode = glfwGetVideoMode(monitors[i]);
            glfwGetMonitorPos(monitors[i], &mx, &my);
            mw = mode->width;
            mh = mode->height;

            overlap =
                std::max(0, std::min(wx + ww, mx + mw) - std::max(wx, mx)) *
                std::max(0, std::min(wy + wh, my + mh) - std::max(wy, my));

            if (bestoverlap < overlap) {
                bestoverlap = overlap;
                bestmonitor = monitors[i];
            }
        }

        return bestmonitor;
    }

    int WindowsWindow::m_WindowCount = 0;

    WindowsWindow::WindowsWindow(const WindowData& data)
        : WindowBase(data)
    {
        info.hideOnClose = data.hideOnClose;
        graphics = std::make_unique<OpenGL>();
        dimensions = data.dimensions.dimensions;
        
        if (m_WindowCount == 0 && !glfwInit())
        {
            LOG("Failed to initialize GLFW");
            exit(-1);
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_FLOATING, data.alwaysOnTop);
        glfwWindowHint(GLFW_RESIZABLE, data.resizeable);
        glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, data.transparentBuffer);
        glfwWindowHint(GLFW_DECORATED, data.decorated);
        glfwWindowHint(GLFW_SAMPLES, 4);
        glfwWindowHint(GLFW_VISIBLE, data.state != 0);
        glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE);
        glfwWindowHint(GLFW_AUTO_ICONIFY, GLFW_FALSE);

        m_Window = glfwCreateWindow(55, 55, "NAME", NULL, NULL);

        if (&m_Window == NULL)
        {
            LOG("Failed to create GLFW window");
            if (--m_WindowCount == 0)
                glfwTerminate();
        }

        if (!data.decorated)
        {
            long style = GetWindowLong(GetWin32Handle(), GWL_EXSTYLE);
            style = style & ~WS_EX_APPWINDOW | WS_EX_TOOLWINDOW;
            SetWindowLong(GetWin32Handle(), GWL_EXSTYLE, style);
        }

        glfwMakeContextCurrent(m_Window);

        if (m_WindowCount == 0 && !gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            LOG("Failed to initialize GLAD");
            exit(-1);
        }

        glEnable(GL_BLEND);
        glEnable(GL_TEXTURE_2D);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        //glEnable(GL_MULTISAMPLE);
        glEnable(GL_SCISSOR_TEST);
        glfwSwapInterval(0);

        m_WindowCount++;

        // Add a subproc to handle some msgs ourselves.
        SetWindowSubclass(GetWin32Handle(), &SubClassProc, windowId, (DWORD_PTR) static_cast<void*>(this));

        // Set the margins of the Win32 window.
        MARGINS _margins = { 1, 1, 1, 1 };
        DwmExtendFrameIntoClientArea(GetWin32Handle(), &_margins);

        State<Visible>(data.state);
    }


    bool WindowsWindow::Loop()
    {
        static int _currentId = -1;

        // If the dimensions changed, update the window size
        if (m_PrevDims.dimensions != dimensions)
        {
            m_PrevDims.dimensions = dimensions;
            MoveWindow(GetWin32Handle(), x, y, width, height, false);
        }

        int _visible = State<Visible>();

        if (m_PrevVisibility != _visible)
        {
            int _cmd = 0;
            if (_visible == Close && info.hideOnClose) ShowWindow(GetWin32Handle(), SW_HIDE);
            else if (_visible == Close) return false;
            else if (_visible == Show && m_PrevVisibility == Hide) ShowWindow(GetWin32Handle(), SW_SHOW);
            else if (_visible == Show && m_PrevVisibility == Minimize) ShowWindow(GetWin32Handle(), SW_RESTORE);
            else if (_visible == Show && m_PrevVisibility == Maximize) ShowWindow(GetWin32Handle(), SW_RESTORE);
            else if (_visible == Hide && m_PrevVisibility == Show) ShowWindow(GetWin32Handle(), SW_HIDE);
            else if (_visible == Hide && m_PrevVisibility == Minimize) ShowWindow(GetWin32Handle(), SW_HIDE);
            else if (_visible == Hide && m_PrevVisibility == Maximize) ShowWindow(GetWin32Handle(), SW_HIDE);
            else if (_visible == Minimize && m_PrevVisibility == Show) ShowWindow(GetWin32Handle(), SW_MINIMIZE);
            else if (_visible == Minimize && m_PrevVisibility == Hide) ShowWindow(GetWin32Handle(), SW_MINIMIZE);
            else if (_visible == Minimize && m_PrevVisibility == Maximize) ShowWindow(GetWin32Handle(), SW_MINIMIZE);
            else if (_visible == Maximize && m_PrevVisibility == Show) ShowWindow(GetWin32Handle(), SW_MAXIMIZE);
            else if (_visible == Maximize && m_PrevVisibility == Hide) ShowWindow(GetWin32Handle(), SW_MAXIMIZE);
            else if (_visible == Maximize && m_PrevVisibility == Minimize) ShowWindow(GetWin32Handle(), SW_MAXIMIZE);

            m_PrevVisibility = _visible;
        }
        WINDOWPLACEMENT placement;
        placement.length = sizeof(WINDOWPLACEMENT);
        GetWindowPlacement(GetWin32Handle(), &placement);
        if (placement.showCmd == SW_NORMAL) _visible = Show;
        else if (placement.showCmd == SW_HIDE) _visible = Hide;
        else if (placement.showCmd == SW_SHOWMAXIMIZED) _visible = Maximize;
        else if (placement.showCmd == SW_SHOWMINIMIZED) _visible = Minimize;
        State<Visible>(_visible);

        // Only change context if it's necessary, because this seems to
        // have a relatively high impact on the CPU usage.
        if (_currentId != windowId && _visible)
        {
            _currentId = windowId;
            glfwMakeContextCurrent(m_Window);
        }

        if (_visible)
        {
            //    if (RightClickMenu::Get().GetWin32Handle() != GetWin32Handle() && RightClickMenu::Get().Opened())
            //        m_MousePressed = Event::MouseButton::NONE;

            //CurrentWindow(windowId);
            //Graphics::Scaling(m_Scale);
            //WindowFocused(GetForegroundWindow() == GetWin32Handle());
            
            ForwardUpdate();
        }

        if (_visible)
        {
            graphics->collection.Clear();
            graphics->collection.Clip({ -1, -1, width + 2, height + 2 });
            graphics->collection.Viewport({ 0, 0, width, height });
            ForwardRender(graphics->collection);
            graphics->Render();
            glfwSwapBuffers(m_Window);

            // Go through the even queue
            while (!m_EventQueue.empty())
            {
                listener(*m_EventQueue.front());

                // Check again if the eventqueue is empty, because some button somewhere
                // might trigger a resize of the window, which will trigger this WindowsWindow::Update
                // which will finish this while loop, and then exit out of AddEvent about here ^
                // and then m_EventQueue will be empty, causing an exception when calling pop().
                if (!m_EventQueue.empty())
                    m_EventQueue.pop();
            }
        }
        glfwPollEvents();
        return true;
    }

    // Hit test the frame for resizing and moving.
    LRESULT HitTestNCA(HWND hWnd, int x, int y, float scale)
    {
        // Get the point coordinates for the hit test.
        POINT _ptMouse = { x, y };

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
        int _padding = 8 / scale;
        if (_ptMouse.y >= _rcWindow.top && _ptMouse.y < _rcWindow.top + 32 / scale)
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
        WindowsWindow* _self = (WindowsWindow*)dwRefData;
        if (_self == nullptr)
            return 0;
        bool _fCallDWP = true;
        LRESULT _lRet = 0;

        // Winproc worker for custom frame issues.
        _fCallDWP = !DwmDefWindowProc(hWnd, uMsg, wParam, lParam, &_lRet);

        int _keymod =
            (((GetKeyState(VK_LCONTROL) & 0x8000) || GetKeyState(VK_RCONTROL) & 0x8000) ? EventMods::Control : 0)
            | ((GetKeyState(VK_RCONTROL) & 0x8000) ? EventMods::RightControl : 0)
            | ((GetKeyState(VK_LCONTROL) & 0x8000) ? EventMods::LeftControl : 0)
            | (((GetKeyState(VK_LMENU) & 0x8000) || GetKeyState(VK_RMENU) & 0x8000) ? EventMods::Alt : 0)
            | ((GetKeyState(VK_LMENU) & 0x8000) ? EventMods::LeftAlt : 0)
            | ((GetKeyState(VK_RMENU) & 0x8000) ? EventMods::RightAlt : 0)
            | (((GetKeyState(VK_LSHIFT) & 0x8000) || GetKeyState(VK_RSHIFT) & 0x8000) ? EventMods::Shift : 0)
            | ((GetKeyState(VK_RSHIFT) & 0x8000) ? EventMods::RightShift : 0)
            | ((GetKeyState(VK_LSHIFT) & 0x8000) ? EventMods::LeftShift : 0)
            | ((GetKeyState(VK_NUMLOCK) & 0x8000) ? EventMods::NumLock : 0)
            | ((GetKeyState(VK_CAPITAL) & 0x8000) ? EventMods::CapsLock : 0);


        int _offset = IsMaximized(hWnd) ? 8 : 0;
        float _padding = 8.0;
        if (_self->mouseInfo.pressedMouseButtons != MouseButton::None || !_self->BorderHitbox(_self->mouseInfo.cursorPosition))
            switch (uMsg)
            {
                {
            case WM_NCLBUTTONDBLCLK:
            case WM_LBUTTONDBLCLK:
            case WM_NCLBUTTONDOWN:
            case WM_LBUTTONDOWN:
                SetCapture(hWnd);
                _self->MouseButtonCallback(MouseButton::Left, true, _keymod);
                _fCallDWP = false;
                break;
            case WM_NCLBUTTONUP:
            case WM_LBUTTONUP:
                _self->MouseButtonCallback(MouseButton::Left, false, _keymod);
                ReleaseCapture();
                _fCallDWP = false;
                break;
            case WM_NCRBUTTONDBLCLK:
            case WM_RBUTTONDBLCLK:
            case WM_NCRBUTTONDOWN:
            case WM_RBUTTONDOWN:
                SetCapture(hWnd);
                _self->MouseButtonCallback(MouseButton::Right, true, _keymod);
                _fCallDWP = false;
                break;
            case WM_NCRBUTTONUP:
            case WM_RBUTTONUP:
                _self->MouseButtonCallback(MouseButton::Right, false, _keymod);
                ReleaseCapture();
                _fCallDWP = false;
                break;
            case WM_NCMBUTTONDBLCLK:
            case WM_MBUTTONDBLCLK:
            case WM_NCMBUTTONDOWN:
            case WM_MBUTTONDOWN:
                SetCapture(hWnd);
                _self->MouseButtonCallback(MouseButton::Middle, true, _keymod);
                _fCallDWP = false;
                break;
            case WM_NCMBUTTONUP:
            case WM_MBUTTONUP:
                _self->MouseButtonCallback(MouseButton::Middle, false, _keymod);
                ReleaseCapture();
                _fCallDWP = false;
                break;
                }
            }
        else
            switch (uMsg)
            {
            case WM_NCLBUTTONDOWN:
            case WM_LBUTTONDOWN:
                _self->MouseButtonCallback(MouseButton::Left, true, _keymod);
                _self->MouseButtonCallback(MouseButton::Left, false, _keymod);
                break;

            case WM_NCRBUTTONDOWN:
            case WM_RBUTTONDOWN:
                _self->MouseButtonCallback(MouseButton::Right, true, _keymod);
                _self->MouseButtonCallback(MouseButton::Right, false, _keymod);
                break;

            case WM_NCMBUTTONDOWN:
            case WM_MBUTTONDOWN:
                _self->MouseButtonCallback(MouseButton::Middle, true, _keymod);
                _self->MouseButtonCallback(MouseButton::Middle, false, _keymod);
                break;
            }

        switch (uMsg)
        {
        case WM_SYSKEYDOWN:
        case WM_SYSKEYUP:
        case WM_KEYDOWN:
        case WM_KEYUP:
        case WM_SYSCHAR:
        case WM_CHAR:
        {
            bool _repeat = (lParam & 0x40000000);
            int _type = 0; // Release
            if (uMsg == WM_SYSKEYDOWN || uMsg == WM_KEYDOWN)
                _type = 1; // Press
            else if (uMsg == WM_CHAR || uMsg == WM_SYSCHAR)
                _type = 2; // Type

            _self->KeyCallback(wParam, _repeat, _type, _keymod);
            return 0;
        }
        case WM_MOUSEWHEEL:
        {
            RECT _rect;
            long _x = 0, _y = 0;
            if (::GetWindowRect(hWnd, &_rect))
                _x = _rect.left, _y = _rect.top;

            int _amount = GET_WHEEL_DELTA_WPARAM(wParam);
            int _xPos = GET_X_LPARAM(lParam);
            int _yPos = GET_Y_LPARAM(lParam);
            _self->MouseWheelCallback(_amount, _keymod, _xPos - _x, _yPos - _y);
            break;
        }
        case WM_EXITSIZEMOVE:
        case WM_SIZE:
        case WM_SIZING:
        {
            RECT _rect;
            int _width = 0, _height = 0;

            if (::GetWindowRect(hWnd, &_rect))
            {
                _width = _rect.right - _rect.left;
                _height = _rect.bottom - _rect.top;
                _self->WindowSizeCallback(_rect.left, _rect.top, _width, _height);
            }
            break;
        }
        case WM_MOUSEMOVE:
        {
            int x = GET_X_LPARAM(lParam);
            int y = GET_Y_LPARAM(lParam);
            _self->CursorPosCallback(x, y, _keymod);
            break;
        }
        case WM_MOUSELEAVE:
            [[fallthrough]];
        case WM_NCMOUSELEAVE:
        {
            POINT pos;
            RECT rect;
            GetCursorPos(&pos);
            ::GetWindowRect(hWnd, &rect);
            int x = pos.x - rect.left;
            int y = pos.y - rect.top;
            _self->CursorPosCallback(x, y, _keymod);
            break;
        }
        case WM_NCMOUSEMOVE:
        {
            RECT _rect;
            long _x = 0, _y = 0;
            if (::GetWindowRect(hWnd, &_rect))
                _x = _rect.left, _y = _rect.top;

            _self->CursorPosCallback(GET_X_LPARAM(lParam) - _x, GET_Y_LPARAM(lParam) - _y, _keymod);
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
            if (_lRet == 0 && (_lRet = HitTestNCA(hWnd, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), 1 /*_self->m_Scale*/)) != HTNOWHERE)
                _fCallDWP = false;
        }

        // Winproc worker for the rest of the application.
        if (_fCallDWP)
            _lRet = DefSubclassProc(hWnd, uMsg, wParam, lParam);

        return _lRet;
    }

    void WindowsWindow::CursorPosCallback(int x, int y, int mod)
    {
        mouseInfo.cursorPosition.x = x;
        mouseInfo.cursorPosition.y = y;
        if (mouseInfo.pressedMouseButtons == MouseButton::None)
            m_EventQueue.emplace(new MouseMove{ { (float)x, (float)y } });
        else
            m_EventQueue.emplace(new MouseDrag{ { (float)x, (float)y }, mouseInfo.pressedMouseButtons, mod });
    }

    void WindowsWindow::MouseButtonCallback(int button, bool press, int mod)
    {
        if (press)
        {
            mouseInfo.pressedMouseButtons |= button; // Add button to pressed
            mouseInfo.pressedCursorPosition = mouseInfo.cursorPosition; // Set press cursor position
            m_EventQueue.emplace(new MousePress{ mouseInfo.cursorPosition, button, mod });
        }
        else
        {
            mouseInfo.pressedMouseButtons ^= button; // Remove button from pressed
            if (mouseInfo.pressedCursorPosition == mouseInfo.cursorPosition) // If pos not changed, add click
                m_EventQueue.emplace(new MouseClick{ mouseInfo.cursorPosition, button, mod });

            m_EventQueue.emplace(new MouseRelease{ mouseInfo.cursorPosition, button, mod });
        }
    }

    void WindowsWindow::MouseWheelCallback(int amount, int mod, int x, int y)
    {
        m_EventQueue.emplace(new MouseWheel{ { (float)x, (float)y }, amount, mod });
    }

    void WindowsWindow::KeyCallback(int key, bool repeat, int action, int mod)
    {
        if (action == 0) // Release
            m_EventQueue.emplace(new KeyRelease{ key, mod });
        else if (action == 1) // Press
            m_EventQueue.emplace(new KeyPress{ key, mod, repeat });
        else // Type
        {
            std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> conversion;
            m_EventQueue.emplace(new KeyType{ conversion.to_bytes(key)[0], mod, ((char)key) == 0xffff });
        }
    }

    void WindowsWindow::WindowSizeCallback(int x, int y, int width, int height)
    {
        dimensions = { (float)x, (float)y, (float)width, (float)height };
        m_PrevDims = { (float)x, (float)y, (float)width, (float)height };

        graphics->SetProjection(glm::ortho(0.0f, (float)std::max(width, 5), 0.0f, (float)std::max(height, 5)));
    }
}