#pragma once
#include "GuiCode2/Panel.hpp"

namespace GuiCode
{
	class WindowBase;
	struct WindowData
	{
		std::string name;
		Vec2<int> size;
		bool alwaysOnTop = false;
		bool transparentBuffer = false;
		bool hideOnClose = false;
		bool showOnCreate = true;
		bool resizeable = true;
		bool decorated = true;
		WindowBase* parent = nullptr;
	};

	struct MouseInfo
	{
		int pressedMouseButtons = 0;
		Vec2<double> cursorPosition{ 0, 0 };
		Vec2<double> pressedCursorPosition{ 0, 0 };
	};

	struct WindowInfo
	{

	};

	class WindowBase : public Component
	{
	public:
		WindowBase(const WindowData& d)
		{
			RegisterComponent(panel);
		}

		virtual void Loop() = 0;

		MouseInfo mouseInfo;
		WindowInfo info;
		Panel panel;
	};
}