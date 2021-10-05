#pragma once
#include "GuiCode2/pch.hpp"
#include "GuiCode2/Window.hpp"

namespace GuiCode
{
	/**
	 * Frame for a context menu.
	 */
	class ContextFrame : public Window
	{
		bool m_ShouldClose = false;

	public:
		ContextFrame();

		void Initialize();
		void Update() override;
		void Close();
		void CloseNow();

		WindowBase* owner = nullptr;
		bool hideOnClick = false;
	};

	/**
	 * Context menu singleton, contains a pool of ContextFrames
	 * to open context menus.
	 */
	class ContextMenu
	{
	public:
		static inline Vec2<float> offset;

		/**
		 * Open a ContextFrame with the component.
		 * @param c the component
		 * @param position the position relative to the current window
		 */
		static void Open(const Pointer<Component>& c, const Vec2<float> position, bool hideOnClick = false);

		/**
		 * Close the ContextFrame that is displaying the component.
		 * @param c the component
		 */
		static void Close(const Pointer<Component>& c);
		static void CloseNow(const Pointer<Component>& c);
		static void Loop();

	private:
		static inline std::list<ContextFrame> m_WindowPool;

		friend class WindowsWindow;
	};
}