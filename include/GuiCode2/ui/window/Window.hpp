#pragma once
#include "GuiCode/pch.hpp"

// --------------------------------------------------------------------------
// ------------------------------ Window ------------------------------------
// --------------------------------------------------------------------------

#ifdef WIN32
#include "GuiCode/platform/windows/WindowsWindow.hpp"
using WindowType = WindowsWindow;
#endif

struct Window : public WindowType
{
	Window(const std::string& name, const int width, const int height)
		: WindowType(name, width, height)
	{}
};