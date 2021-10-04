#pragma once
#include "GuiCode2/Panel.hpp"

namespace GuiCode
{
	enum WindowState
	{
		Hide = 1, Show, Minimize, Maximize, Close
	};

	class WindowBase;
	struct WindowData
	{
		std::string name;
		Dimensions dimensions{ 0, 0, 500, 500 };
		bool alwaysOnTop = false;
		bool transparentBuffer = false;
		int state = Show;
		bool resizeable = true;
		bool decorated = true;
		bool noAnimations = false;
		bool hideOnClose = false;
	};

	struct MouseInfo
	{
		int pressedMouseButtons = 0;
		Vec2<float> cursorPosition{ 0, 0 };
		Vec2<float> pressedCursorPosition{ 0, 0 };
	};

	struct WindowInfo
	{
		std::string name;
		bool hideOnClose;
	};

	class WindowBase : public Component
	{
	public:
		static inline WindowBase* currentWindow = nullptr;

		WindowBase(const WindowData& d)
		{
			windowId = m_WindowIdCounter++;
			info.name = d.name;
			info.hideOnClose = d.hideOnClose;
		}

		virtual bool Loop() = 0;

		MouseInfo mouseInfo;
		WindowInfo info;
		int windowId;
		std::unique_ptr<GraphicsBase> graphics;

		Vec4<float> BoundingBox() const override { return { 0, 0, width, height }; }

	protected:
		virtual bool BorderHitbox(const Vec2<float>&) const { return false; }

	private:
		static inline int m_WindowIdCounter = 0;
	};
}