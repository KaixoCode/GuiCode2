#pragma once
#include "GuiCode2/Panel.hpp"

namespace GuiCode
{
	enum WindowState
	{
		Hide, Show, Minimize, Maximize, Close
	};

	class WindowBase;
	struct WindowData
	{
		std::string name;
		Dimensions dimensions{ 0, 0, 500, 500 };
		bool alwaysOnTop = false;
		bool transparentBuffer = false;
		bool hideOnClose = false;
		int state = Show;
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
		bool hideOnClose = false;
	};



	class WindowBase : public Panel
	{
	public:
		WindowBase(const WindowData& d)
		{
			windowId = m_WindowIdCounter++;
		}

		virtual bool Loop() = 0;

		MouseInfo mouseInfo;
		WindowInfo info;
		int windowId;
		std::unique_ptr<GraphicsBase> graphics;

	protected:
		virtual bool BorderHitbox(const Vec2<double>&) const { return false; }

	private:
		static inline int m_WindowIdCounter = 0;
	};
}