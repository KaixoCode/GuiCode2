#include "GuiCode2/Layout.hpp"

namespace GuiCode
{

	Span* Span::Find(int id)
	{
		if (settings.id == id)
			return this;

		for (auto& i : spans)
		{
			Span* _div = i.Find(id);
			if (_div)
				return _div;
		}

		return nullptr;
	}

	void Span::Layout() 
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
			if (spans.size() == 0)
				return;

			// layouting
			switch (settings.layout)
			{
			case Layout::Row:
			{
				// Calculate the sum of all the ratios
				float _ratioSum = 0, _explicitWidthSum = 0;
				for (auto& _s : spans)
					if (_s.settings.ratio == 0) // If no ratio, check what width it will become
						_explicitWidthSum += _s.SetDimensions({ -1, -1, -1, -1 }).width;
					else // Otherwise add to ratio sum
						_ratioSum += _s.settings.ratio;

				// Account for padding in the size, and 
				float _x = x + settings.padding.left;
				float _y = y + settings.padding.top;
				float _width = width - settings.padding.left - settings.padding.right;
				float _height = height - settings.padding.top - settings.padding.bottom;
				float _ratioWidth = _width - _explicitWidthSum;
				for (auto& _s : spans)
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
				float _toFill = (x + settings.padding.left + _width) - _x;
				for (int tries = 0; tries < spans.size() && _toFill > 1; tries++)
				{   
					// x + padding + width = goal for x to reach, difference needs to be filled
					_toFill = (x + settings.padding.left + _width) - _x;
					_x = x + settings.padding.left; // Reset x
					for (auto& _s : spans)
					{
						Vec4<float> _offsets = _s.Offsets();

						// If the width != max width, we can add stuff
						if (_s.settings.ratio != 0 && _s.width != _s.max.width)
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
				break;
			}
			case Layout::Column:
			{
				// Calculate the sum of all the ratios
				float _ratioSum = 0, _explicitHeightSum = 0;
				for (auto& _s : spans)
					if (_s.settings.ratio == 0) // If no ratio, check what height it will become
						_explicitHeightSum += _s.SetDimensions({ -1, -1, -1, -1 }).height;
					else // Otherwise add to ratio sum
						_ratioSum += _s.settings.ratio;

				// Account for padding in the size, and 
				float _x = x + settings.padding.left;
				float _y = y + settings.padding.top;
				float _width = width - settings.padding.left - settings.padding.right;
				float _height = height - settings.padding.top - settings.padding.bottom;
				float _ratioHeight = _height - _explicitHeightSum;
				for (auto& _s : spans)
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
				float _toFill = (y + settings.padding.top + _height) - _y;
				for (int tries = 0; tries < spans.size() && _toFill > 1; tries++)
				{
					// y + padding + height = goal for x to reach, difference needs to be filled
					_toFill = (y + settings.padding.top + _height) - _y;
					_y = y + settings.padding.top; // Reset y
					for (auto& _s : spans)
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
				break;
			}
			}
		}
	}

	void Span::ForwardUpdate()
	{
		Layout();
		for (auto& _s : spans)
			_s.ForwardUpdate();
	}

	void Span::ForwardRender(CommandCollection& d)
	{
		d.PushClip();
		d.PushMatrix();
		d.Fill(settings.background);
		d.Quad(dimensions);

		// Fill borders

		// If overflow == Hide || Scroll: add Clip/Translation
		
		if (component)
			component->ForwardRender(d);

		std::list<Span*> _spans;
		for (auto& _span : spans)
			_spans.push_back(&_span);

		_spans.sort([](Span* a, Span* b) -> bool { return a->settings.zIndex < b->settings.zIndex; });

		for (auto& _span : _spans)
			_span->ForwardRender(d);

		scrollbar.x.ForwardRender(d);
		scrollbar.y.ForwardRender(d);
		d.PopMatrix();
		d.PopClip();
	}
}