#pragma once
#include "GuiCode2/pch.hpp"
#include "GuiCode2/Window.hpp"


namespace GuiCode
{

	class ContextFrame : public Window
	{
		bool shouldClose = false;
	public:
		ContextFrame()
			: Window({.alwaysOnTop = true, .state = Hide, .resizeable = false, .decorated = true, .noAnimations = true })
		{}

		void Update() override
		{
			if (components.size())
			{
				Component& _c = *components.begin();
				size = _c.size;
			}

			if (shouldClose)
			{
				if (components.size())
				{
					Component& _c = *components.begin();
					if (_c.State<Focused>())
					{
						_c.State<Focused>(false);
						_c.listener(Unfocus{});
					}
					components.clear();
				}
				State<Visible>(Hide);
				owner = nullptr;
				shouldClose = false;
			}
		}

		void Render(CommandCollection& d) const override
		{
			d.Fill({ 0, 0, 0 });
			d.Quad(dimensions);
		}

		void Close()
		{
			shouldClose = true;
		}

		WindowBase* owner = nullptr;
	};

	class ContextMenu
	{
	public:

		static inline Vec2<float> offset;

		static inline void Open(Component& c, const Vec2<float> position)
		{
			ContextFrame* _theChosenOne = nullptr;
			for (auto& _window : m_WindowPool)
			{
				if (_window.State<Visible>() == Hide)
				{
					_theChosenOne = &_window;
					break;
				}
			}

			if (!_theChosenOne)
				_theChosenOne = &m_WindowPool.emplace_back();

			_theChosenOne->position = offset + position;
			_theChosenOne->size = c.size;
			_theChosenOne->components.push_back(c);
			_theChosenOne->owner = WindowBase::currentWindow;
			if (!c.State<Focused>())
			{
				c.State<Focused>(true);
				c.listener(Focus{});
			}
			_theChosenOne->State<Visible>(Show);
		}

		static inline void Close(Component& c)
		{
			for (auto& _window : m_WindowPool)
			{
				if (_window.components.size() && &*_window.components.begin() == &c)
				{
					_window.Close();
				}
			}
		}

		static inline void Loop()
		{
			for (auto& _window : m_WindowPool)
			{
				_window.Loop();
			}
		}

	private:
		static inline std::list<ContextFrame> m_WindowPool;

		friend class WindowsWindow;
	};
}