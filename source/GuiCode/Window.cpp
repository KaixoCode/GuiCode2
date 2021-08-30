//#include "GuiCode2/Window.hpp"
//
//namespace GuiCode
//{
//    WindowsWindow::WindowsWindow()
//    {
//        //m_InitialSize = d.size;
//        //m_Projection = glm::ortho(0.0f, (float)std::max(d.size.width / m_Scale, 5.0f), 0.0f, (float)std::max(d.size.height / m_Scale, 5.0f));
//        if (m_WindowCount == 0 && !glfwInit())
//        {
//            LOG("Failed to initialize GLFW");
//            exit(-1);
//        }
//
//        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
//        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
//        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//        //glfwWindowHint(GLFW_FLOATING, d.alwaysOnTop);
//        //glfwWindowHint(GLFW_RESIZABLE, d.resizeable);
//        //glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, d.transparentBuffer);
//        //glfwWindowHint(GLFW_DECORATED, d.decorated);
//        glfwWindowHint(GLFW_SAMPLES, 4);
//        //glfwWindowHint(GLFW_VISIBLE, d.showOnCreate);
//        glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE);
//        glfwWindowHint(GLFW_AUTO_ICONIFY, GLFW_FALSE);
//
//        m_Window = glfwCreateWindow(55, 55, "NAME", NULL, NULL);
//
//        m_WindowId = m_WindowIdCounter++;
//        if (&m_Window == NULL)
//        {
//            LOG("Failed to create GLFW window");
//            if (--m_WindowCount == 0)
//                glfwTerminate();
//        }
//
//        /*if (!d.decorated)
//        {
//            long style = GetWindowLong(GetWin32Handle(), GWL_EXSTYLE);
//            style = style & ~WS_EX_APPWINDOW | WS_EX_TOOLWINDOW;
//            SetWindowLong(GetWin32Handle(), GWL_EXSTYLE, style);
//        }*/
//
//        glfwMakeContextCurrent(m_Window);
//
//        if (m_WindowCount == 0 && !gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
//        {
//            LOG("Failed to initialize GLAD");
//            exit(-1);
//        }
//
//        glEnable(GL_BLEND);
//        glEnable(GL_TEXTURE_2D);
//        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//        //glEnable(GL_MULTISAMPLE);
//        glEnable(GL_SCISSOR_TEST);
//        glfwSwapInterval(0);
//
//        m_WindowCount++;
//
//        // Add a subproc to handle some msgs ourselves.
//        SetWindowSubclass(GetWin32Handle(), &SubClassProc, m_WindowId, (DWORD_PTR) static_cast<void*>(this));
//
//        // Set the margins of the Win32 window.
//        MARGINS _margins = { 1, 1, 1, 1 };
//        //if (!shadow)
//        //    _margins = { 0, 0, 0, 0 };
//        DwmExtendFrameIntoClientArea(GetWin32Handle(), &_margins);
//
//        //Size(Width(), Height());    
//        //d.showOnCreate ? Show() : Hide();
//
//    }
//    // Hit test the frame for resizing and moving.
//    LRESULT HitTestNCA(HWND hWnd, WPARAM wParam, LPARAM lParam, float scale)
//    {
//        // Get the point coordinates for the hit test.
//        POINT _ptMouse = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
//
//        // Get the window rectangle.
//        RECT _rcWindow;
//        GetWindowRect(hWnd, &_rcWindow);
//
//        // Get the frame rectangle, adjusted for the style without a caption.
//        RECT _rcFrame = { 0 };
//        AdjustWindowRectEx(&_rcFrame, WS_OVERLAPPEDWINDOW & ~WS_CAPTION, FALSE, NULL);
//
//        // Determine if the hit test is for resizing. Default middle (1,1).
//        USHORT _uRow = 1;
//        USHORT _uCol = 1;
//        bool _fOnResizeBorder = false;
//
//        // Determine if the point is at the top or bottom of the window.
//        int _padding = 8 / scale;
//        if (_ptMouse.y >= _rcWindow.top && _ptMouse.y < _rcWindow.top + 32 / scale)
//        {
//            _fOnResizeBorder = (_ptMouse.y < (_rcWindow.top - _rcFrame.top));
//            _uRow = 0;
//        }
//        else if (_ptMouse.y < _rcWindow.bottom && _ptMouse.y >= _rcWindow.bottom - _padding)
//        {
//            _uRow = 2;
//        }
//
//        // Determine if the point is at the left or right of the window.
//        if (_ptMouse.x >= _rcWindow.left && _ptMouse.x < _rcWindow.left + _padding)
//            _uCol = 0; // left side
//
//        else if (_ptMouse.x < _rcWindow.right && _ptMouse.x >= _rcWindow.right - _padding)
//            _uCol = 2; // right side
//
//        // Hit test (HTTOPLEFT, ... HTBOTTOMRIGHT)
//        LRESULT _hitTests[3][3] =
//        {
//            { HTTOPLEFT,    _fOnResizeBorder ? HTTOP : HTCAPTION,    HTTOPRIGHT },
//            { HTLEFT,       HTNOWHERE,     HTRIGHT },
//            { HTBOTTOMLEFT, HTBOTTOM, HTBOTTOMRIGHT },
//        };
//
//        return _hitTests[_uRow][_uCol];
//    }
//
//    LRESULT CALLBACK WindowsWindow::SubClassProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
//    {
//        static bool _resizing = true;
//        WindowsWindow* _self = (WindowsWindow*)dwRefData;
//        if (_self == nullptr)
//            return 0;
//        bool _fCallDWP = true;
//        LRESULT _lRet = 0;
//
//        // ShellIconCallback
//        //if (uMsg == 10000 && m_ShellIconCallbacks.find(wParam) != m_ShellIconCallbacks.end())
//        //{
//        //    if (lParam == WM_LBUTTONDOWN)
//        //    {
//        //        Event _e{ Event::Type::MousePressed, 0, 0, Event::MouseButton::LEFT };
//        //        m_ShellIconCallbacks[wParam](_e);
//        //    }
//        //    if (lParam == WM_LBUTTONDBLCLK)
//        //    {
//        //        Event _e{ Event::Type::MousePressed, 0, 0, Event::MouseButton::LEFT, true };
//        //        m_ShellIconCallbacks[wParam](_e);
//        //    }
//        //    if (lParam == WM_RBUTTONDOWN)
//        //    {
//        //        Event _e{ Event::Type::MousePressed, 0, 0, Event::MouseButton::RIGHT };
//        //        m_ShellIconCallbacks[wParam](_e);
//        //    }
//        //    if (lParam == WM_RBUTTONDBLCLK)
//        //    {
//        //        Event _e{ Event::Type::MousePressed, 0, 0, Event::MouseButton::RIGHT, true };
//        //        m_ShellIconCallbacks[wParam](_e);
//        //    }
//        //}
//
//        // Winproc worker for custom frame issues.
//        _fCallDWP = !DwmDefWindowProc(hWnd, uMsg, wParam, lParam, &_lRet);
//
//        // Buttons if not on the edge or top bar
//        //int _offset = IsMaximized(hWnd) ? -8 : 0;
//        //double _padding = 8.0 / _self->m_Scale;
//        //if (_self->m_HoveringComponent != nullptr && _self->CursorPos().y > _self->Height() - (32 + _offset) / _self->m_Scale || _self->m_MousePressed != Event::MouseButton::NONE ||
//        //    _self->CursorPos().y < _self->Height() - (32 + _offset) / _self->m_Scale && _self->CursorPos().x > _padding && _self->CursorPos().x < _self->Width() - _padding && _self->CursorPos().y > _padding)
//        {
//            bool _dblclk = false;
//            bool _rClick = false;
//            bool _click = false;
//            switch (uMsg)
//            {
//            case WM_NCLBUTTONDBLCLK:
//            case WM_LBUTTONDBLCLK:
//                _dblclk = true;
//                [[fallthrough]];
//            case WM_NCLBUTTONDOWN:
//            case WM_LBUTTONDOWN:
//                SetCapture(hWnd);
//                _self->MouseButtonCallback(_self, Event::MouseButton::LEFT, Event::Type::MousePressed, _dblclk);
//                _fCallDWP = false;
//                _click = true;
//                break;
//
//            case WM_NCLBUTTONUP:
//            case WM_LBUTTONUP:
//                _self->MouseButtonCallback(_self, Event::MouseButton::LEFT, Event::Type::MouseReleased, 0);
//                ReleaseCapture();
//                _fCallDWP = false;
//                _click = true;
//                break;
//
//            case WM_NCRBUTTONDBLCLK:
//            case WM_RBUTTONDBLCLK:
//                _dblclk = true;
//                [[fallthrough]];
//            case WM_NCRBUTTONDOWN:
//            case WM_RBUTTONDOWN:
//                SetCapture(hWnd);
//                _self->MouseButtonCallback(_self, Event::MouseButton::RIGHT, Event::Type::MousePressed, _dblclk);
//                _fCallDWP = false;
//                _click = true;
//                break;
//
//            case WM_NCRBUTTONUP:
//            case WM_RBUTTONUP:
//                _rClick = true;
//                _self->MouseButtonCallback(_self, Event::MouseButton::RIGHT, Event::Type::MouseReleased, 0);
//                ReleaseCapture();
//                _fCallDWP = false;
//                _click = true;
//                break;
//
//            case WM_NCMBUTTONDBLCLK:
//            case WM_MBUTTONDBLCLK:
//                _dblclk = true;
//                [[fallthrough]];
//            case WM_NCMBUTTONDOWN:
//            case WM_MBUTTONDOWN:
//                SetCapture(hWnd);
//                _self->MouseButtonCallback(_self, MouseButton::Middle, Event::Type::MousePressed, _dblclk);
//                _fCallDWP = false;
//                _click = true;
//                break;
//
//            case WM_NCMBUTTONUP:
//            case WM_MBUTTONUP:
//                _self->MouseButtonCallback(_self, MouseButton::MIDDLE, Event::Type::MouseReleased, 0);
//                ReleaseCapture();
//                _fCallDWP = false;
//                _click = true;
//                break;
//            }
//            //if (_rClick && RightClickMenu::Get().Opened() && _self != &RightClickMenu::Get());
//            //else
//                if (_click && _fCallDWP == false && GetFocus() != _self->GetWin32Handle())
//                    ::SetFocus(_self->GetWin32Handle());
//        }
//
//        switch (uMsg)
//        {
//        case WM_HOTKEY:
//        {
//            //_self->m_Hotkeys[wParam]();
//        }
//        break;
//        case WM_WINDOWPOSCHANGED:
//        {
//            //if (IsWindowVisible(hWnd) == true)
//            //    SetWindowPos(hWnd, HWND_TOP, 0, 0, 0, 0, SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOMOVE);
//        }
//        break;
//        case WM_SYSKEYDOWN:
//        case WM_SYSKEYUP:
//        case WM_KEYDOWN:
//        case WM_KEYUP:
//        case WM_SYSCHAR:
//        case WM_CHAR:
//        {
//            int _mod = ((GetKeyState(VK_CONTROL) & 0x8000) ? EventMods::Control : 0)
//                | (((GetKeyState(VK_MENU) & 0x8000) || GetKeyState(VK_RMENU) & 0x8000) ? EventMods::Alt : 0)
//                | ((GetKeyState(VK_SHIFT) & 0x8000) ? EventMods::Shift : 0)
//                | ((GetKeyState(VK_NUMLOCK) & 0x8000) ? EventMods::NumLock : 0)
//                | ((GetKeyState(VK_CAPITAL) & 0x8000) ? EventMods::CapsLock : 0);
//
//            int _repeat = (lParam & 0x40000000);
//            _self->KeyCallback(_self, wParam, _repeat,
//                uMsg == WM_SYSKEYDOWN || uMsg == WM_KEYDOWN ? Event::Type::KeyPressed :
//                uMsg == WM_CHAR || uMsg == WM_SYSCHAR ? Event::Type::KeyTyped : Event::Type::KeyReleased, _mod);
//            return 0;
//        }
//        case WM_MOUSEWHEEL:
//        {
//            RECT _rect;
//            long _x = 0, _y = 0;
//            if (::GetWindowRect(hWnd, &_rect))
//                _x = _rect.left, _y = _rect.top;
//
//            int _win32mod = GET_KEYSTATE_WPARAM(wParam);
//            int _mod = ((_win32mod & 0x0008) ? EventMods::Control : 0)
//                | ((_win32mod & 0x0001) ? EventMods::Left : 0)
//                | ((_win32mod & 0x0010) ? EventMods::Middle : 0)
//                | ((_win32mod & 0x0002) ? EventMods::Right : 0)
//                | ((_win32mod & 0x0004) ? EventMods::Shift : 0);
//            int _amount = GET_WHEEL_DELTA_WPARAM(wParam);
//            int _xPos = GET_X_LPARAM(lParam);
//            int _yPos = GET_Y_LPARAM(lParam);
//            _self->MouseWheelCallback(_self, _amount, _mod, _xPos - _x, _yPos - _y);
//            break;
//        }
//        case WM_EXITSIZEMOVE:
//        case WM_SIZE:
//        case WM_SIZING:
//        {
//            RECT _rect;
//            int _width = 0, _height = 0;
//
//            if (::GetWindowRect(hWnd, &_rect))
//            {
//                _width = _rect.right - _rect.left;
//                _height = _rect.bottom - _rect.top;
//            }
//            _self->WindowSizeCallback(_self, _width, _height);
//            break;
//        }
//        case WM_MOUSEMOVE:
//        {
//            int x = GET_X_LPARAM(lParam);
//            int y = GET_Y_LPARAM(lParam);
//            _self->m_EventQueue.push(MouseMove{ { (double)x, (double)y } });
//            break;
//        }
//        case WM_MOUSELEAVE:
//            [[fallthrough]];
//        case WM_NCMOUSELEAVE:
//        {
//            POINT pos;
//            RECT rect;
//            GetCursorPos(&pos);
//            ::GetWindowRect(hWnd, &rect);
//            int x = pos.x - rect.left;
//            int y = pos.y - rect.top;
//            _self->CursorPosCallback(_self, x, y);
//            break;
//        }
//        case WM_NCMOUSEMOVE:
//        {
//            RECT _rect;
//            long _x = 0, _y = 0;
//            if (::GetWindowRect(hWnd, &_rect))
//                _x = _rect.left, _y = _rect.top;
//
//            _self->CursorPosCallback(_self, GET_X_LPARAM(lParam) - _x, GET_Y_LPARAM(lParam) - _y);
//            break;
//        }
//        // Handle the non-client size message.
//        case WM_NCCALCSIZE:
//        {
//            if (wParam == TRUE)
//                _fCallDWP = false;
//            break;
//        }
//        //case WM_GETMINMAXINFO:
//        //{
//        //    LPMINMAXINFO lpMMI = (LPMINMAXINFO)lParam;
//        //    lpMMI->ptMinTrackSize.x = _self->m_MinSize.width == -1 ? 0 : _self->m_MinSize.width;
//        //    lpMMI->ptMinTrackSize.y = _self->m_MinSize.height == -1 ? 0 : _self->m_MinSize.height;
//        //    lpMMI->ptMaxTrackSize.x = _self->m_MaxSize.width == -1 ? 100000 : _self->m_MaxSize.width;
//        //    lpMMI->ptMaxTrackSize.y = _self->m_MaxSize.height == -1 ? 100000 : _self->m_MaxSize.height;
//        //    _fCallDWP = false;
//        //    _lRet = 0;
//        //}
//        // Handle hit testing in the NCA if not handled by DwmDefWindowProc.
//        case WM_NCHITTEST:
//            //if (!_self->FullScreen())
//                if (_lRet == 0 && (_lRet = HitTestNCA(hWnd, wParam, lParam, 1 /*_self->m_Scale*/)) != HTNOWHERE)
//                    _fCallDWP = false;
//        }
//
//        // Winproc worker for the rest of the application.
//        if (_fCallDWP)
//            _lRet = DefSubclassProc(hWnd, uMsg, wParam, lParam);
//
//        return _lRet;
//    }
//}