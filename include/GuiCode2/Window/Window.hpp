#pragma once
#include "GuiCode2/Components/Panel.hpp"

namespace GuiCode
{
	enum WindowState
	{
		Hide = 1, Show, Minimize, Maximize, Close
	};

	class WindowBase : public Component
	{
	public:
		static inline WindowBase* CURRENT = nullptr;
		static inline size_t WINDOW_ID_COUNTER = 0;

		struct Settings
		{
			std::string name = "Window";
			Dimensions dimensions{ 0, 0, 500, 500 };
			bool alwaysOnTop = false;
			bool transparentBuffer = false;
			int state = Show;
			bool resizeable = true;
			bool decorated = true;
			bool animations = true;
			bool hideOnClose = false;
		};

		WindowBase(const Settings& d)
			: settings(d), id(WINDOW_ID_COUNTER++)
		{}

		virtual void Create() = 0;
		virtual bool Loop() = 0;

		struct CursorState
		{
			int buttons = 0;
			Vec2<float> position{ 0, 0 };
			Vec2<float> pressed{ 0, 0 };
		} cursor;

		int id;
		std::unique_ptr<GraphicsBase> graphics;
		Settings settings;

		Vec4<float> BoundingBox() const override { return { 0, 0, width, height }; }
		virtual bool BorderHitbox(const Vec2<float>&) const { return false; }
	};
}

#ifdef WIN32
#include "GuiCode2/Window/WindowsWindow.hpp"
#endif
