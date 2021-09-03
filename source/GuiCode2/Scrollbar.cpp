#include "GuiCode2/Scrollbar.hpp"

namespace GuiCode
{
	Scrollbar::Scrollbar()
	{
		size = { 16, 16 }; // Base width/height is 16.
		bar.base = { 128, 128, 128 }; // scrollbar's base color

		listener += [this](const MousePress& e)
		{
			m_ValueWhenPressed = value; // pressed value

			// Calculate things using axis
			float _size = axis == Horizontal ? width : height;
			float _pos = axis == Horizontal ? x : y;
			float _mousePos = axis == Horizontal ? e.pos.x : e.pos.y;

			float _barSize = _size * visibleRange / (range.end - range.start + visibleRange);
			float _relPos = _pos + (_size - _barSize) * (value - range.start) / (range.end - range.start);

			// If hovering over the bar, set dragging to true
			if (_mousePos > _relPos && _mousePos < _relPos + _barSize)
				m_IsDragging = true;
		};

		listener += [this](const MouseRelease& e)
		{
			m_IsDragging = false;
		};

		listener += [this](const MouseDrag& e)
		{
			// Only scroll when left mouse and dragging
			if (!(e.buttons & MouseButton::Left) || !m_IsDragging)
				return;

			// Calculate things using axis
			float _size = axis == Horizontal ? width : height;
			float _pos = axis == Horizontal ? x : y;
			float _mousePos = axis == Horizontal ? e.pos.x : e.pos.y;
			float _sourcePos = axis == Horizontal ? e.source.x : e.source.y;
			float _barSize = _size * visibleRange / (range.end - range.start + visibleRange);

			// Calculate the value, and then constrain it
			value = m_ValueWhenPressed + (_mousePos - _sourcePos) * (range.end - range.start) / (_size - _barSize);
			value = constrain(value, range.start, range.end);
		};
	}

	bool Scrollbar::Necessary() const
	{
		// Scrollbar is necessary when scrollable range > 1
		return range.end - range.start > 1;
	}

	void Scrollbar::Update()
	{
		// Always constrain the value
		value = constrain(value, range.start, range.end);
	}

	void Scrollbar::Render(CommandCollection& d) const
	{
		float _size = axis == Horizontal ? width : height;
		float _pos = axis == Horizontal ? x : y;
		float _barSize = _size * visibleRange / (range.end - range.start + visibleRange);
		float _relPos = _pos + (_size - _barSize) * (value - range.start) / (range.end - range.start);

		d.Fill(background);
		d.Quad(dimensions);

		d.Fill(bar.Current());
		if (axis == Horizontal)
			d.Quad({ _relPos, y, _barSize, height });
		else
			d.Quad({ x, _relPos, width, _barSize });
	}

	bool Scrollbar::Hitbox(const Vec2<float>& pos) const
	{
		float _size = axis == Horizontal ? width : height;
		float _pos = axis == Horizontal ? x : y;
		float _barSize = _size * visibleRange / (range.end - range.start + visibleRange);
		float _relPos = _pos + (_size - _barSize) * (value - range.start) / (range.end - range.start);

		if (axis == Horizontal)
			return pos.x > _relPos && pos.x < _relPos + _barSize && pos.y > y && pos.y < y + height;
		else
			return pos.y > _relPos && pos.y < _relPos + _barSize && pos.x > x && pos.x < x + width;
	}
}