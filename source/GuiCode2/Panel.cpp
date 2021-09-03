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
			component->dimensions = dimensions;


			//// Positioning
			//if (settings.align & Align::Right) component->dimensions.x = settings.dimensions.x + settings.dimensions.width - component->dimensions.width;
			//else if (settings.align & Align::XCenter) component->dimensions.x = settings.dimensions.x + settings.dimensions.width / 2 - component->dimensions.width / 2;
			//else component->dimensions.x = settings.dimensions.x;

			//if (settings.align & Align::Bottom) component->dimensions.y = settings.dimensions.y + settings.dimensions.height - component->dimensions.height;
			//else if (settings.align & Align::YCenter) component->dimensions.y = settings.dimensions.y + settings.dimensions.height / 2 - component->dimensions.height / 2;
			//else component->dimensions.y = settings.dimensions.y;
		}
		else
		{
			if (panels.size() == 0)
				return;

			Vec2<float> _scrollTranslate{ 0, 0 };
			Vec2<float> _scrollbarOffsets{ 0, 0 };

			if (scrollbar.x.Necessary())
				_scrollTranslate.x = -scrollbar.x.value,
				_scrollbarOffsets.y = scrollbar.x.height;

			if (scrollbar.y.Necessary())
				_scrollTranslate.y = -scrollbar.y.value,
				_scrollbarOffsets.x = scrollbar.y.width;

			float _contentX = x + settings.padding.left + _scrollTranslate.x;
			float _contentY = y + settings.padding.top + _scrollTranslate.y;
			float _width = width - settings.padding.left - settings.padding.right - _scrollbarOffsets.x;
			float _height = height - settings.padding.top - settings.padding.bottom - _scrollbarOffsets.y;

			// layouting
			switch (settings.layout)
			{
			case Layout::Row:
			{
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
				if (!component && settings.size.height == -1 && settings.ratio == 0)
					height = _biggestHeight + settings.padding.top + settings.padding.bottom;

				// Account for padding in the size, and 
				float _x = _contentX;
				float _y = _contentY;
				float _ratioWidth = _width - _explicitWidthSum;

				// Loop
				for (auto& _s : panels)
				{
					Vec4<float> _newDims{ _x, _y, -1, -1 };

					// When height not set, set to this height.
					if (_s.settings.size.height == -1) _newDims.height = _height;

					// Set the width if ratio is defined
					if (_s.settings.ratio != 0)
						_newDims.width = _ratioWidth * _s.settings.ratio / _ratioSum;

					// Set the dimensions, taking into account margin etc. incr with width
					float _addedWidth = _s.SetDimensions(_newDims).width;
					_x += _addedWidth;

					// If we didn't add everything to width, we've reached max. Account for
					// that by acting like this component now has a fixed size (it's max size)
					if (_s.settings.ratio != 0 && _addedWidth != _newDims.width)
						_ratioWidth -= _addedWidth,     // By removing the width from ratio
						_ratioSum -= _s.settings.ratio; // And removing ratio from sum.
				}

				// If there is remaining space due to the max size of some components,
				// we'll loop here until that space is <= 1.
				float _toFill = (_contentX + _width) - _x;
				for (int tries = 0; tries < panels.size(); tries++)
				{
					// x + padding + width = goal for x to reach, difference needs to be filled
					_toFill = (_contentX + _width) - _x;
					if (_toFill < 1 && _toFill > -1)
						break;
					_x = _contentX; // Reset x
					for (auto& _s : panels)
					{
						Vec4<float> _offsets = _s.Offsets();

						// If the width != max width, we can add stuff
						if (_s.settings.ratio != 0 && _s.width != _s.max.width && _s.width != _s.min.width)
						{
							Vec4<float> _newDims{ _x, _y, -1, -1 };

							// When height not set, set to this height.
							if (_s.settings.size.height == -1) _newDims.height = _height;

							// using the offsets, calculate the new width by adding the ratio of
							// toFill to the width.
							_newDims.width = _offsets.left + _offsets.right + _s.width
								+ _toFill * _s.settings.ratio / _ratioSum;

							// Set the dimensions, taking into account margin etc. incr with width
							float _addedWidth = _s.SetDimensions(_newDims).width;
							_x += _addedWidth;

							// If we didn't add everything to width, we've reached max. Account for
							// that by acting like this component now has a fixed size (it's max size)
							if (_newDims.width != -1 && _addedWidth != _newDims.width)
								_ratioWidth -= _addedWidth,		// By removing the width from ratio
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

				m_Viewport.width = (_x - _contentX) + settings.padding.right + settings.padding.left + _scrollbarOffsets.x;
				m_Viewport.height = (_biggestHeight + settings.padding.top + settings.padding.bottom + _scrollbarOffsets.y);

				break;
			}
			case Layout::Column:
			{
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
				if (!component && settings.size.width == -1 && settings.ratio == 0)
					width = _biggestWidth + settings.padding.left + settings.padding.right;

				// Account for padding in the size, and 
				float _x = _contentX;
				float _y = _contentY;
				float _ratioHeight = _height - _explicitHeightSum;
				for (auto& _s : panels)
				{
					Vec4<float> _newDims{ _x, _y, -1, -1 };

					// When width not set, set to this width.
					if (_s.settings.size.width == -1) _newDims.width = _width;

					// Set the height if ratio is defined
					if (_s.settings.ratio != 0)
						_newDims.height = _ratioHeight * _s.settings.ratio / _ratioSum;

					// Set the dimensions, taking into account margin etc. incr with height
					float _addedHeight = _s.SetDimensions(_newDims).height;
					_y += _addedHeight;

					// If we didn't add everything to height, we've reached max. Account for
					// that by acting like this component now has a fixed size (it's max size)
					if (_s.settings.ratio != 0 && _addedHeight != _newDims.height)
						_ratioHeight -= _addedHeight,   // By removing the height from ratio
						_ratioSum -= _s.settings.ratio; // And removing ratio from sum.
				}

				// If there is remaining space due to the max size of some components,
				// we'll loop here until that space is <= 1.
				float _toFill = (_contentY + _height) - _y;
				for (int tries = 0; tries < panels.size() && _toFill > 1; tries++)
				{
					// y + padding + height = goal for x to reach, difference needs to be filled
					_toFill = (_contentY + _height) - _y;
					_y = _contentY; // Reset y
					for (auto& _s : panels)
					{
						Vec4<float> _offsets = _s.Offsets();

						// If the height != max height, we can add stuff
						if (_s.settings.ratio != 0 && _s.height != _s.max.height)
						{
							Vec4<float> _newDims{ _x, _y, -1, -1 };

							// When width not set, set to this width.
							if (_s.settings.size.width == -1) _newDims.width = _width;

							// using the offsets, calculate the new width by adding the ratio of
							// toFill to the width.
							_newDims.height = _offsets.top + _offsets.bottom + _s.height
								+ _toFill * _s.settings.ratio / _ratioSum;

							// Set the dimensions, taking into account margin etc. incr with width
							float _addedHeight = _s.SetDimensions(_newDims).height;
							_y += _addedHeight;

							// If we didn't add everything to height, we've reached max. Account for
							// that by acting like this component now has a fixed size (it's max size)
							if (_s.settings.ratio != 0 && _addedHeight != _newDims.height)
								_ratioHeight -= _addedHeight,		// By removing the width from ratio
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

				m_Viewport.width = (_biggestWidth + settings.padding.right + settings.padding.left + _scrollbarOffsets.x);
				m_Viewport.height = (_y - _contentY) + settings.padding.top + settings.padding.bottom + _scrollbarOffsets.y;

				break;
			}
			}
		}
	}

	void Panel::ForwardUpdate()
	{
		Vec2<float> _scrollbarOffsets{ 0, 0 };

		if (scrollbar.y.Necessary()) _scrollbarOffsets.x = scrollbar.y.width;
		if (scrollbar.x.Necessary()) _scrollbarOffsets.y = scrollbar.x.height;

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

		RefreshLayout();

		scrollbar.x.State<Visible>(scrollbar.x.Necessary());
		scrollbar.y.State<Visible>(scrollbar.y.Necessary());
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

		// Fill borders

		if (settings.overflow != Overflow::Show)
			d.Clip({ x, y, width - scrollbar.y.Necessary() * 16, height - scrollbar.x.Necessary() * 16 });

		if (component)
			component->ForwardRender(d);
		else
		{
			std::list<Panel*> _panels;
			for (auto& _span : panels)
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
		if (newDims.width == -1)
		{
			if (settings.size.width != -1)
				width = settings.size.width;
			else if (component)
				width = component->width;
		}
		else
			width = newDims.width - _offsets.left - _offsets.right;

		if (newDims.height == -1)
		{
			if (settings.size.height != -1)
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
		if (settings.min.width != -1)
			min.width = settings.min.width;
		else if (component)
			min.width = component->min.width;

		if (settings.min.height != -1)
			min.height = settings.min.height;
		else if (component)
			min.height = component->min.height;

		if (settings.max.width != -1)
			max.width = settings.max.width;
		else if (component)
			max.width = component->max.width;

		if (settings.max.height != -1)
			max.height = settings.max.height;
		else if (component)
			max.height = component->max.height;

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