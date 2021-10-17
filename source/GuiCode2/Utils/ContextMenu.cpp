#include "GuiCode2/Utils/ContextMenu.hpp"

namespace GuiCode
{
	ContextFrame::ContextFrame()
		: Window()
	{
		listener += [this](const MouseRelease& e)
		{
			if (hideOnClick)
				m_ShouldClose = true;
		};
	}

	void ContextFrame::Initialize()
	{
		InitializeWindow({ .alwaysOnTop = true, .state = Hide, .resizeable = false, .decorated = false, .noAnimations = true });
	}

	void ContextFrame::Update()
	{
		if (components.size())
			size = (*components.begin())->size;

		if (m_ShouldClose)
			CloseNow();
	}

	void ContextFrame::Close()
	{
		m_ShouldClose = true;
	}

	void ContextFrame::CloseNow()
	{
		if (components.size())
		{
			Component& _c = *components.begin();
			if (_c.State(Focused))
			{
				_c.State(Focused) = false;
				_c.listener(Unfocus{});
			}
			components.clear();
		}
		State(Visible) = Hide;
		owner = nullptr;
		m_ShouldClose = false;
	}

	void ContextMenu::Open(const Pointer<Component>& c, const Vec2<float> position, bool hideOnClick)
	{
		ContextFrame* _theChosenOne = nullptr;
		for (auto& _window : m_WindowPool)
			if (_window.State(Visible) == Hide)
			{
				_theChosenOne = &_window;
				break;
			}

		if (!_theChosenOne)
			_theChosenOne = &m_WindowPool.emplace_back();

		Pointer _c = c;
		_theChosenOne->components.push_back(c);
		_theChosenOne->owner = WindowBase::currentWindow;
		_theChosenOne->hideOnClick = hideOnClick;
		if (_theChosenOne->GetWin32Handle() == nullptr) 
			_theChosenOne->Initialize();
		_theChosenOne->position = offset + position;
		_theChosenOne->size = c->size;
		_theChosenOne->State(Visible) = Show;
		if (!_c->State(Focused))
		{
			_c->State(Focused) = true;
			_c->listener(Focus{});
		}
	}

	void ContextMenu::Close(const Pointer<Component>& c)
	{
		for (auto& _window : m_WindowPool)
			if (_window.components.size() && &*_window.components.begin() == &c)
				_window.Close();
	}

	void ContextMenu::CloseNow(const Pointer<Component>& c)
	{
		for (auto& _window : m_WindowPool)
			if (_window.components.size() && &*_window.components.begin() == &c)
				_window.CloseNow();
	}

	void ContextMenu::Loop()
	{
		for (auto& _window : m_WindowPool)
			_window.Loop();
	}
}