#include "GuiCode2/Panel.hpp"

// Panel id
namespace GuiCode
{
	Panel::Id::Id()
		: m_Id(m_Counter++)
	{}

	Panel::Id::Id(const Id& other)
		: m_Id(other.m_Id)
	{
		if (other.m_Used)
			throw std::exception("You can't assign an id more than once");
		other.m_Used = true;
	}

	Panel::Id& Panel::Id::operator=(const Id& other)
	{
		if (other.m_Used)
			throw std::exception("You can't assign an id more than once");
		m_Id = other.m_Id;
	}

	Panel::Panel()
	{
		Init();
	}

	Panel::Panel(const Settings& s)
		: settings(s)
	{
		Init();
	}

	Panel::Panel(const Settings& s, const std::list<Panel>& d)
		: settings(s),
		panels(d)
	{
		Init();
	}

	Panel::Panel(const Settings& s, Component& c)
		: settings(s),
		component(&c)
	{
		Init();
	}

	Panel::Panel(const Panel& other)
		: settings(other.settings),
		component(other.component),
		panels(other.panels)
	{
		Init();
	}

	Panel& Panel::operator=(const Panel& other)
	{
		components.clear();

		settings = other.settings;
		component = other.component;
		panels = other.panels;

		if (component)
			components.push_back(component);

		scrollbar.x.zIndex = std::numeric_limits<int>::max();
		scrollbar.y.zIndex = std::numeric_limits<int>::max();

		for (auto& i : panels)
			components.push_back(&i);

		components.push_back(&scrollbar.x);
		components.push_back(&scrollbar.y);
		return *this;
	}

	void Panel::Init()
	{
		if (component)
			components.push_back(component);

		for (auto& i : panels)
			components.push_back(&i);

		scrollbar.x.zIndex = std::numeric_limits<int>::max();
		scrollbar.y.zIndex = std::numeric_limits<int>::max();

		components.push_back(&scrollbar.x);
		components.push_back(&scrollbar.y);
		
		listener += [this](const MouseWheel& e)
		{
			// If mousewheel already handled by sub-component, don't scroll
			if (e.Handled())
				return;

			if (scrollbar.y.Necessary() && settings.overflow.y == Overflow::Scroll)
			{
				e.Handle(); // Notify we've handled the scrolling
				scrollbar.y.value -= e.amount * 0.15, scrollbar.y.ConstrainValue();
			}
		};
	}

	Panel* Panel::Find(Id& id)
	{
		if (settings.id == id)
			return this;

		for (auto& i : panels)
			if (Panel* _div = i.Find(id))
				return _div;

		return nullptr;
	}

	void Panel::RefreshLayout()
	{
		if (component)
		{
			component->dimensions =
			{
				x + settings.padding.left,
				y + settings.padding.top,
				width - settings.padding.left - settings.padding.right,
				height - settings.padding.top - settings.padding.bottom
			};
		}
		else
		{
			if (panels.size() == 0)
				return;

			Vec2<float> _scrollTranslate{ 0, 0 };
			Vec2<float> _scrollbarOffsets{ 0, 0 };

			if (settings.overflow.x == Overflow::Scroll)
				if (scrollbar.x.Necessary())
					_scrollTranslate.x = -scrollbar.x.value,
					_scrollbarOffsets.y = scrollbar.x.height;

			if (settings.overflow.y == Overflow::Scroll)
				if (scrollbar.y.Necessary())
					_scrollTranslate.y = -scrollbar.y.value,
					_scrollbarOffsets.x = scrollbar.y.width;

			Vec4<float> _content;
			_content.x = x + settings.padding.left + _scrollTranslate.x;
			_content.y = y + settings.padding.top + _scrollTranslate.y;
			_content.width = width - settings.padding.left - settings.padding.right - _scrollbarOffsets.x;
			_content.height = height - settings.padding.top - settings.padding.bottom - _scrollbarOffsets.y;

			// layouting
			switch (settings.layout)
			{
			case Layout::Row:
			{
				RowLayout(_content);
				break;
			}
			case Layout::Column:
			{
				ColumnLayout(_content);
				break;
			}
			}
		}
	}

	void Panel::RowLayout(const Vec4<float>& content)
	{
		// Calculate Row layout in 3 steps
		//  1: Determine sum of ratios and explicit sizes if no ratio was given
		//  2: Go through panels and set their sizes, retrieve actual size and increment x with width
		//  3: If there's some leftover space, loop for sub-panels.size() time to fill that space.
		// Also calculate the biggest and smallest y and x to set the viewport

		m_Viewport.x = 0;
		m_Viewport.y = 0;

		// Calculate the sum of all the ratios
		float _ratioSum = 0, _explicitWidthSum = 0, _biggestHeight = 0;
		for (auto& _s : panels)
		{
			Vec2<float> _sizes = _s.SetDimensions({ -1, -1, -1, -1 });
			if (_s.settings.ratio == 0) // If no ratio, check what width it will become
				_explicitWidthSum += _sizes.width;
			else // Otherwise add to ratio sum
				_ratioSum += _s.settings.ratio;

			// Get biggest height
			if (_sizes.height > _biggestHeight)
				_biggestHeight = _sizes.height;
		}

		// If no height can be determined, set height to biggest of sub-component
		if (!component && settings.size.height < 0 && settings.ratio == 0)
			height = _biggestHeight + settings.padding.top + settings.padding.bottom;

		// Account for padding in the size, and 
		float _x = content.x;
		float _y = content.y;
		float _ratioWidth = content.width - _explicitWidthSum;

		// Loop
		for (auto& _s : panels)
		{
			Vec4<float> _newDims{ _x, _y, Auto, Auto };

			// When height not set, set to this height.
			if (_s.settings.size.height == Inherit) _newDims.height = content.height;

			// Set the width if ratio is defined
			if (_s.settings.ratio != 0)
				_newDims.width = _ratioWidth * _s.settings.ratio / _ratioSum;

			// Set the dimensions, taking into account margin etc. incr with width
			Vec2<float> _actual = _s.SetDimensions(_newDims);
			_x += _actual.width;

			// Alignment only if not full height
			if (_s.settings.size.height != Inherit)
				if (_s.settings.align & Align::CenterY)
				{
					_s.y = content.y + content.height / 2 - _s.height / 2;
					Vec4<float> _offsets = _s.Offsets();
					if (_s.y - _offsets.top - content.y < m_Viewport.y)
						m_Viewport.y = _s.y - _offsets.top - content.y;
				}
				else if (_s.settings.align & Align::Bottom)
				{
					_s.y = content.y + content.height - _s.height;
					Vec4<float> _offsets = _s.Offsets();
					if (_s.y - _offsets.top - content.y < m_Viewport.y)
						m_Viewport.y = _s.y - _offsets.top - content.y;
				}

			// If we didn't add everything to width, we've reached max. Account for
			// that by acting like this component now has a fixed size (it's max size)
			if (_s.settings.ratio != 0 && _actual.width != _newDims.width)
				_ratioWidth -= _actual.width,   // By removing the width from ratio
				_ratioSum -= _s.settings.ratio; // And removing ratio from sum.
		}

		// If there is remaining space due to the max size of some components,
		// we'll loop here until that space is <= 1.
		float _toFill = (content.x + content.width) - _x;
		for (int tries = 0; tries < panels.size(); tries++)
		{
			// x + padding + width = goal for x to reach, difference needs to be filled
			_toFill = (content.x + content.width) - _x;
			if (_toFill < 1 && _toFill > -1)
				break;
			_x = content.x; // Reset x
			for (auto& _s : panels)
			{
				Vec4<float> _offsets = _s.Offsets();

				// If the width != max width, we can add stuff
				if (_s.settings.ratio != 0 && _s.width != _s.max.width && _s.width != _s.min.width)
				{
					Vec4<float> _newDims{ _x, _y, Auto, Auto };

					// When height not set, set to this height.
					if (_s.settings.size.height == Inherit) _newDims.height = content.height;

					// using the offsets, calculate the new width by adding the ratio of
					// toFill to the width.
					_newDims.width = _offsets.left + _offsets.right + _s.width
						+ _toFill * _s.settings.ratio / _ratioSum;

					// Set the dimensions, taking into account margin etc. incr with width
					Vec2<float> _actual = _s.SetDimensions(_newDims);
					_x += _actual.width;

					// Alignment only if not full height
					if (_s.settings.size.height != Inherit)
						if (_s.settings.align & Align::CenterY)
						{
							_s.y = content.y + content.height / 2 - _s.height / 2;
							if (_s.y - _offsets.top - content.y < m_Viewport.y)
								m_Viewport.y = _s.y - _offsets.top - content.y;
						}
						else if (_s.settings.align & Align::Bottom)
						{
							_s.y = content.y + content.height - _s.height;
							if (_s.y - _offsets.top - content.y < m_Viewport.y)
								m_Viewport.y = _s.y - _offsets.top - content.y;
						}

					// If we didn't add everything to width, we've reached max. Account for
					// that by acting like this component now has a fixed size (it's max size)
					if (_newDims.width != -1 && _actual.width != _newDims.width)
						_ratioWidth -= _actual.width,   // By removing the width from ratio
						_ratioSum -= _s.settings.ratio;	// And removing ratio from sum.
				}
				// Otherwise just set the correct new x coord, and increment _x
				else
				{
					_s.x = _x + _offsets.left;
					_x += _s.width + _offsets.left + _offsets.right;
				}
			}
		}

		m_Viewport.width = _x - content.x - content.width + width;
		m_Viewport.height = _biggestHeight - content.height + height;
	}

	void Panel::ColumnLayout(const Vec4<float>& content)
	{		
		// Calculate Column layout in 3 steps
		//  1: Determine sum of ratios and explicit sizes if no ratio was given
		//  2: Go through panels and set their sizes, retrieve actual size and increment y with height
		//  3: If there's some leftover space, loop for sub-panels.size() time to fill that space.
		// Also calculate the biggest and smallest x and y to set the viewport

		m_Viewport.x = 0;
		m_Viewport.y = 0;

		// Calculate the sum of all the ratios
		float _ratioSum = 0, _explicitHeightSum = 0, _biggestWidth = 0;
		for (auto& _s : panels)
		{
			Vec2<float> _sizes = _s.SetDimensions({ -1, -1, -1, -1 });
			if (_s.settings.ratio == 0) // If no ratio, check what height it will become
				_explicitHeightSum += _sizes.height;
			else // Otherwise add to ratio sum
				_ratioSum += _s.settings.ratio;

			// Get biggest width
			if (_sizes.width > _biggestWidth)
				_biggestWidth = _sizes.width;
		}

		// If no width can be determined, set width to biggest of sub-component
		if (!component && settings.size.width < 0 && settings.ratio == 0)
			width = _biggestWidth + settings.padding.left + settings.padding.right;

		// Account for padding in the size, and 
		float _x = content.x;
		float _y = content.y;
		float _ratioHeight = content.height - _explicitHeightSum;
		for (auto& _s : panels)
		{
			Vec4<float> _newDims{ _x, _y, Auto, Auto };

			// When width not set, set to this width.
			if (_s.settings.size.width == Inherit) _newDims.width = content.width;

			// Set the height if ratio is defined
			if (_s.settings.ratio != 0)
				_newDims.height = _ratioHeight * _s.settings.ratio / _ratioSum;

			// Set the dimensions, taking into account margin etc. incr with height
			Vec2<float> _actual = _s.SetDimensions(_newDims);
			_y += _actual.height;

			// Alignment only if not full width
			if (_s.settings.size.width != Inherit)
				if (_s.settings.align & Align::CenterX)
				{
					_s.x = content.x + content.width / 2 - _s.width / 2;							
					Vec4<float> _offsets = _s.Offsets();
					if (_s.x - _offsets.right - content.x < m_Viewport.x)
						m_Viewport.x = _s.x - _offsets.right - content.x;
				}
				else if (_s.settings.align & Align::Right)
				{
					_s.x = content.x + content.width - _s.width;
					Vec4<float> _offsets = _s.Offsets();
					if (_s.x - _offsets.right - content.x < m_Viewport.x)
						m_Viewport.x = _s.x - _offsets.right - content.x;
				}

			// If we didn't add everything to height, we've reached max. Account for
			// that by acting like this component now has a fixed size (it's max size)
			if (_s.settings.ratio != 0 && _actual.height != _newDims.height)
				_ratioHeight -= _actual.height,   // By removing the height from ratio
				_ratioSum -= _s.settings.ratio; // And removing ratio from sum.
		}

		// If there is remaining space due to the max size of some components,
		// we'll loop here until that space is <= 1.
		float _toFill = (content.y + content.height) - _y;
		for (int tries = 0; tries < panels.size() && _toFill > 1; tries++)
		{
			// y + padding + height = goal for x to reach, difference needs to be filled
			_toFill = (content.y + content.height) - _y;
			_y = content.y; // Reset y
			for (auto& _s : panels)
			{
				Vec4<float> _offsets = _s.Offsets();

				// If the height != max height, we can add stuff
				if (_s.settings.ratio != 0 && _s.height != _s.max.height)
				{
					Vec4<float> _newDims{ _x, _y, Auto, Auto };

					// When width not set, set to this width.
					if (_s.settings.size.width == Inherit) _newDims.width = content.width;

					// using the offsets, calculate the new width by adding the ratio of
					// toFill to the width.
					_newDims.height = _offsets.top + _offsets.bottom + _s.height
						+ _toFill * _s.settings.ratio / _ratioSum;

					// Set the dimensions, taking into account margin etc. incr with height
					Vec2<float> _actual = _s.SetDimensions(_newDims);
					_y += _actual.height;

					// Alignment only if not full width
					if (_s.settings.size.width != Inherit)
						if (_s.settings.align & Align::CenterX)
						{
							_s.x = content.x + content.width / 2 - _s.width / 2;
							if (_s.x - _offsets.right - content.x < m_Viewport.x)
								m_Viewport.x = _s.x - _offsets.right - content.x;
						}
						else if (_s.settings.align & Align::Right)
						{
							_s.x = content.x + content.width - _s.width;
							if (_s.x - _offsets.right - content.x < m_Viewport.x)
								m_Viewport.x = _s.x - _offsets.right - content.x;
						}

					// If we didn't add everything to height, we've reached max. Account for
					// that by acting like this component now has a fixed size (it's max size)
					if (_s.settings.ratio != 0 && _actual.height != _newDims.height)
						_ratioHeight -= _actual.height,		// By removing the width from ratio
						_ratioSum -= _s.settings.ratio;	// And removing ratio from sum.
				}
				// Otherwise just set the correct new x coord, and increment _x
				else
				{
					_s.y = _y + _offsets.top;
					_y += _s.height + _offsets.top + _offsets.bottom;
				}
			}
		}

		m_Viewport.width = _biggestWidth - content.width + width;
		m_Viewport.height = _y - content.y - content.height + height;
	}

	void Panel::RefreshScrollbars()
	{
		Vec2<float> _scrollbarOffsets{ 0, 0 };

		if (scrollbar.y.Necessary() && settings.overflow.y == Overflow::Scroll)
			_scrollbarOffsets.x = scrollbar.y.width;
		if (scrollbar.x.Necessary() && settings.overflow.x == Overflow::Scroll)
			_scrollbarOffsets.y = scrollbar.x.height;

		scrollbar.x.range.start = m_Viewport.x;
		scrollbar.x.range.end = m_Viewport.x + m_Viewport.width - width;
		scrollbar.x.visibleRange = width;
		scrollbar.x.position = { x, y + height - scrollbar.x.height };
		scrollbar.x.width = width - _scrollbarOffsets.x;
		scrollbar.x.axis = Scrollbar::Horizontal;

		scrollbar.y.range.start = m_Viewport.y;
		scrollbar.y.range.end = m_Viewport.y + m_Viewport.height - height;
		scrollbar.y.visibleRange = height;
		scrollbar.y.position = { x + width - scrollbar.y.width, y };
		scrollbar.y.height = height - _scrollbarOffsets.y;
		scrollbar.y.axis = Scrollbar::Vertical;
	}

	void Panel::ForwardUpdate()
	{
		RefreshScrollbars();

		RefreshLayout();

		scrollbar.x.State<Visible>(scrollbar.x.Necessary() && settings.overflow.x == Overflow::Scroll);
		scrollbar.y.State<Visible>(scrollbar.y.Necessary() && settings.overflow.y == Overflow::Scroll);
		scrollbar.x.ForwardUpdate();
		scrollbar.y.ForwardUpdate();

		if (component)
			component->ForwardUpdate();

		else
			for (auto& _s : panels)
				_s.ForwardUpdate();
	}

	void Panel::ForwardRender(CommandCollection& d)
	{
		CalculateOrder();
		d.PushClip();
		d.PushMatrix();
		d.Fill(settings.background);
		d.Quad(dimensions);

		RenderBorder(d);

		if (settings.overflow.x != Overflow::Show || settings.overflow.y != Overflow::Show)
		{
			Vec2<float> _scrollbarOffsets{ 0, 0 };

			if (scrollbar.y.Necessary() && settings.overflow.y == Overflow::Scroll) 
				_scrollbarOffsets.x = scrollbar.y.width;
			if (scrollbar.x.Necessary() && settings.overflow.x == Overflow::Scroll) 
				_scrollbarOffsets.y = scrollbar.x.height;

			d.Clip({ 
				x, 
				y, 
				width - _scrollbarOffsets.x,
				height - _scrollbarOffsets.y
			});
		}

		if (component)
			component->ForwardRender(d);
		else
		{
			std::list<Panel*> _panels;
			for (auto& _span : panels)
				if (_span.BoundingBox().Overlaps(BoundingBox()))
					_panels.push_back(&_span);

			_panels.sort([](Panel* a, Panel* b) -> bool { return a->settings.zIndex < b->settings.zIndex; });

			for (auto& _span : _panels)
				_span->ForwardRender(d);
		}

		d.PopMatrix();
		d.PopClip();

		if (scrollbar.x.State<Visible>())
			scrollbar.x.ForwardRender(d);

		if (scrollbar.y.State<Visible>())
			scrollbar.y.ForwardRender(d);
	}

	void Panel::RenderBorder(CommandCollection& d)
	{
		Vec4<float> _widths{ 0, 0, 0, 0 };
		Vec4<Color> _colors{ {}, {}, {}, {} };

		_widths.left = settings.border.width;
		_widths.top = settings.border.width;
		_widths.right = settings.border.width;
		_widths.bottom = settings.border.width;

		_colors.left = settings.border.color;
		_colors.top = settings.border.color;
		_colors.right = settings.border.color;
		_colors.bottom = settings.border.color;

		if (settings.border.left.width > 0)
			_widths.left = settings.border.left.width, _colors.left = settings.border.left.color;
		if (settings.border.top.width > 0)
			_widths.top = settings.border.top.width, _colors.top = settings.border.top.color;
		if (settings.border.right.width > 0)
			_widths.right = settings.border.right.width, _colors.right = settings.border.right.color;
		if (settings.border.bottom.width > 0)
			_widths.bottom = settings.border.bottom.width, _colors.bottom = settings.border.bottom.color;

		if (_widths.left)
		{
			d.Fill(_colors.left);
			d.Quad({ x - _widths.left, y, _widths.left, height });
		}

		if (_widths.top)
		{
			d.Fill(_colors.top);
			d.Quad({ x - _widths.left, y - _widths.top, width + _widths.left + _widths.right, _widths.top });
		}

		if (_widths.right)
		{
			d.Fill(_colors.right);
			d.Quad({ x + width, y, _widths.right, height });
		}

		if (_widths.bottom)
		{
			d.Fill(_colors.bottom);
			d.Quad({ x - _widths.left, y + height, width + _widths.left + _widths.right, _widths.bottom });
		}
	}

	/**
	 * Set the dimensions given the rectangle that's made available by parent Panel.
	 * this will account for border and margin. It returns the full size including
	 * the margin and border, so the parent can account for the full dimensions.
	 */
	Vec2<float> Panel::SetDimensions(const Vec4<float>& newDims)
	{
		Vec4<float> _offsets = Offsets(); // get offsets

		x = newDims.x + _offsets.left; // Positions + offsets
		y = newDims.y + _offsets.top;

		// If newDims was set to -1, it means use the size in our settings.
		// This is done over here because we then know not to adjust with offsets.
		if (newDims.width < 0)
		{
			if (settings.size.width >= 0)
				width = settings.size.width;
			else if (component)
				width = component->width;
		}
		else
			width = newDims.width - _offsets.left - _offsets.right;

		if (newDims.height < 0)
		{
			if (settings.size.height >= 0)
				height = settings.size.height;
			else if (component)
				height = component->height;
		}
		else
			height = newDims.height - _offsets.top - _offsets.bottom;

		ConstrainSize();

		return { width + _offsets.left + _offsets.right, height + _offsets.top + _offsets.bottom };
	}

	Vec4<float> Panel::Offsets()
	{
		Vec4<float> _offsets;

		// Add margin to offsets
		_offsets.left = settings.margin.left;
		_offsets.top = settings.margin.top;
		_offsets.right = settings.margin.right;
		_offsets.bottom = settings.margin.bottom;

		// Add border to offsets
		if (settings.border.left.width == 0) _offsets.left += settings.border.width;
		else _offsets.left += settings.border.width;
		if (settings.border.top.width == 0) _offsets.top += settings.border.width;
		else _offsets.top += settings.border.width;
		if (settings.border.right.width == 0) _offsets.right += settings.border.width;
		else _offsets.right += settings.border.width;
		if (settings.border.bottom.width == 0) _offsets.bottom += settings.border.width;
		else _offsets.bottom += settings.border.width;

		return _offsets;
	}

	void Panel::ConstrainSize()
	{
		Vec2<float> _maxSizes{ settings.padding.left + settings.padding.right, settings.padding.top + settings.padding.bottom };
		Vec2<float> _minSizes{ settings.padding.left + settings.padding.right, settings.padding.top + settings.padding.bottom };
		
		// Only calculate if needed, since it is a relatively heavy calculation
		if (settings.min.width == Auto || settings.min.height == Auto 
			|| settings.max.width == Auto || settings.max.height == Auto)
			for (auto& i : panels)
			{
				Vec4<float> _offsets = i.Offsets();
				_maxSizes += i.size + Vec2<float>{ _offsets.left + _offsets.right, _offsets.top + _offsets.bottom },
				_minSizes += i.size + Vec2<float>{ _offsets.left + _offsets.right, _offsets.top + _offsets.bottom };
			}

		if (settings.min.width >= 0)
			min.width = settings.min.width;
		else if (settings.min.width == Auto && component)
			min.width = component->min.width;
		else if (settings.min.width == Auto)
			min.width = _minSizes.width;

		if (settings.min.height >= 0)
			min.height = settings.min.height;
		else if (settings.min.height == Auto && component)
			min.height = component->min.height;
		else if (settings.min.height == Auto)
			min.height = _minSizes.height;

		if (settings.max.width >= 0)
			max.width = settings.max.width;
		else if (settings.max.width == Auto && component)
			max.width = component->max.width;
		else if (settings.max.width == Auto)
			max.width = _maxSizes.width;

		if (settings.max.height >= 0)
			max.height = settings.max.height;
		else if (settings.max.height == Auto && component)
			max.height = component->max.height;
		else if (settings.max.height == Auto)
			max.height = _maxSizes.height;

		if (min.width != -1 && width < min.width)
			width = min.width;
		if (max.width != -1 && width > max.width)
			width = max.width;
		if (min.height != -1 && height < min.height)
			height = min.height;
		if (max.height != -1 && height > max.height)
			height = max.height;
	}
}