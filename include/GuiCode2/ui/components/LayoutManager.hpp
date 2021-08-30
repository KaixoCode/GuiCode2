#pragma once
#include "GuiCode/ui/components/Layout.hpp"
#include "GuiCode/ui/components/Component.hpp"
#include "GuiCode/ui/events/EventListener.hpp"

// --------------------------------------------------------------------------
// --------------------------- LayoutManager --------------------------------
// --------------------------------------------------------------------------

struct LayoutManager
{
	LayoutManager();

	void AddEvent(Event& e) { m_Listener.AddEvent(e); }

	int Cursor() { return m_Cursor; }
	
	void Update(const Vec4<int>& dim, ComponentCollection& components);

	::Layout Layout() const{ return m_Layout; }

	template<Layout::Type T, typename ...Args>
	void Layout(Args&& ...args) { m_Layout = ::Layout{ T, args... }; }

	template<Layout::Type T>
	void Update(const Vec4<int>& dim, ComponentCollection& components) {};

	template<>
	void Update<Layout::Grid>(const Vec4<int>& dim, ComponentCollection& components)
	{
		Layout::GridLayout& _grid = m_Layout.grid;
		
		int _index = 0;
		for (auto& _c : components)
		{
			// Calculate the index in the grid
			int _xindex = _index % _grid.cols;
			int _yindex = _index / _grid.cols;
			if (_yindex > _grid.rows)
				break; // Break when more rows

			// Calculate a single slot size
			int _w = (dim.width - _grid.hgap) / _grid.cols;
			int _h = (dim.height - _grid.vgap) / _grid.rows;

			// Calculate the actual position
			int _x = _xindex * _w + _grid.hgap + dim.x;
			int _y = _yindex * _h + _grid.vgap + dim.y;

			_c->Size({ _w - _grid.hgap, _h - _grid.vgap });
			_c->Position({ _x, _y });

			_index++;
		}
	}

	template<>
	void Update<Layout::Border>(const Vec4<int>& dim, ComponentCollection& components)
	{
		Layout::BorderLayout& _border = m_Layout.border;

		int _p = _border.padding;
		Component* _north = 0;
		Component* _south = 0;
		Component* _east = 0;
		Component* _west = 0;
		Component* _center = 0;

		for (auto& _c : components) switch(_c->LayoutHint())
		{
		case Layout::Hint::North: _north = _c.get(); break;
		case Layout::Hint::South: _south = _c.get(); break;
		case Layout::Hint::East: _east = _c.get(); break;
		case Layout::Hint::West: _west = _c.get(); break;
		case Layout::Hint::Center: _center = _c.get(); break;
		}

		int _northHeight = _p;
		if (_north)
		{
			if (m_Dragging == 1)
				_north->Height(_north->Height() - m_MouseY + m_PMouseY);

			_northHeight += _north->Height() + _p;
			int _x = _p,
				_y = dim.height - _northHeight + _p,
				_w = dim.width - _p * 2,
				_h = _north->Height();

			if (m_MouseX > _x && m_MouseX < _x + _w &&
				m_MouseY > _y - _p && m_MouseY < _y)
			{
				m_Cursor = GLFW_RESIZE_NS_CURSOR;
				if (m_MousePress == Event::MouseButton::LEFT)
					m_Dragging = 1;
			}

			_north->Width(_w);
			_north->Position({_x, _y });
		}
	
		int _southHeight = _p;
		if (_south)
		{
			if (m_Dragging == 2)
				_south->Height(_south->Height() + m_MouseY - m_PMouseY);

			_southHeight += _south->Height() + _p;
			int _x = _p,
				_y = _p,
				_w = dim.width - _p * 2,
				_h = _south->Height();

			if (m_MouseX > _x && m_MouseX < _x + _w &&
				m_MouseY > _y + _h && m_MouseY < _y + _h + _p)
			{
				m_Cursor = GLFW_RESIZE_NS_CURSOR;
				if (m_MousePress == Event::MouseButton::LEFT)
					m_Dragging = 2;
			}

			_south->Width(_w);
			_south->Position({ _x, _y });
		}

		int _eastWidth = _p;
		if (_east)
		{
			if (m_Dragging == 3)
				_east->Width(_east->Width() - m_MouseX + m_PMouseX);

			_eastWidth += _east->Width() + _p;
			int _x = dim.width - _eastWidth + _p,
				_y = _southHeight,
				_w = _eastWidth - _p * 2,
				_h = dim.height - _southHeight - _northHeight;

			if (m_MouseX > _x - _p && m_MouseX < _x &&
				m_MouseY > _y && m_MouseY < _y + _h)
			{
				m_Cursor = GLFW_RESIZE_EW_CURSOR;
				if (m_MousePress == Event::MouseButton::LEFT)
					m_Dragging = 3;
			}

			_east->Size({ _w, _h });
			_east->Position({ _x, _y });
		}

		int _westWidth = _p;
		if (_west)
		{
			if (m_Dragging == 4)
				_west->Width(_west->Width() + m_MouseX - m_PMouseX);

			_westWidth += _west->Width() + _p;
			int _x = _p,
				_y = _southHeight,
				_w = _westWidth - _p * 2,
				_h = dim.height - _southHeight - _northHeight;

			if (m_MouseX > _x + _w && m_MouseX < _x + _w + _p &&
				m_MouseY > _y && m_MouseY < _y + _h)
			{
				m_Cursor = GLFW_RESIZE_EW_CURSOR;
				if (m_MousePress == Event::MouseButton::LEFT)
					m_Dragging = 4;
			}

			_west->Size({ _w, _h });
			_west->Position({ _x, _y });
		}

		if (_center)
		{
			_center->Size({ dim.width - _westWidth - _eastWidth, dim.height - _southHeight - _northHeight });
			_center->Position({ _westWidth, _southHeight });
		}
	}

	template<>
	void Update<Layout::Stack>(const Vec4<int>& dim, ComponentCollection& components)
	{
		Layout::StackLayout& _stack = m_Layout.stack;

		int _padding = _stack.padding;
		int _y = dim.y;
		size_t _size = components.size();
		for (int _i = _size - 1; _i >= 0; _i--)
		{
			auto& _c = components[_i];
			if (!_c->Visible())
				continue;

			int _x = _padding + dim.x;
			_c->Position({ _x, _y });
			_c->Width(dim.width - _padding);
			_y += _c->Height();
		}
		m_BiggestX = dim.x + dim.width;
		m_BiggestY = _y;
	}

	Vec2<int> BiggestCoords() { return { m_BiggestX, m_BiggestY }; }

private:
	::Layout m_Layout = { ::Layout::Free };
	EventListener m_Listener;

	int m_MouseX = 0,
		m_MouseY = 0,
		m_PMouseX = 0,
		m_PMouseY = 0,
		m_Dragging = 0,
		m_Cursor = -1,
		m_BiggestX = 0,
		m_BiggestY = 0;

	Event::MouseButton m_MouseDrag = Event::MouseButton::NONE,
		m_MousePress = Event::MouseButton::NONE;
};