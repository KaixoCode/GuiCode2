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
			float Dimensions::* _axisMainp;
			float Dimensions::* _sizeMainp;
			float Vec2<float>::* _sizeMainsp;
			float Dimensions::* _axisOtherp;
			float Dimensions::* _sizeOtherp;
			float Vec2<float>::* _sizeOthersp;

			bool _variableMain;
			bool _variableOther;
			float _maxMain;
			float _maxOther;
			float _startMain;
			float _startOther;

			// Retrieve max main/other size and the main/other axis/size of sub-span, 
			// if size == -1, set to own size, which is set by parent as usable size.
			if (settings.direction == Direction::Row || settings.direction == Direction::RowReverse)
				_axisMainp = &Dimensions::x, _sizeMainp = &Dimensions::width,
				_axisOtherp = &Dimensions::y, _sizeOtherp = &Dimensions::height,
				_sizeMainsp = &Vec2<float>::width, _sizeOthersp = &Vec2<float>::height,
				_variableMain = settings.size.width == -1,
				_variableOther = settings.size.height == -1,
				_maxMain = (_variableMain ? width : settings.size.width) - settings.padding.x - settings.padding.width,
				_maxOther = (_variableOther ? height : settings.size.height) - settings.padding.y - settings.padding.height,
				_startMain = x + settings.padding.x, _startOther = y + settings.padding.y;
			
			else
				_axisMainp = &Dimensions::y, _sizeMainp = &Dimensions::height,
				_axisOtherp = &Dimensions::x, _sizeOtherp = &Dimensions::width,
				_sizeMainsp = &Vec2<float>::height, _sizeOthersp = &Vec2<float>::width,
				_variableMain = settings.size.height == -1,
				_variableOther = settings.size.width == -1,
				_maxMain = (_variableMain ? height : settings.size.height) - settings.padding.y - settings.padding.height,
				_maxOther = (_variableOther ? width : settings.size.width) - settings.padding.x - settings.padding.width,
				_startMain = y + settings.padding.y, _startOther = x + settings.padding.x;


			// Determine max ratio/max size
			float _maxRatio = 0;
			float _maxExplicitSize = 0;
			float _maxSubSize = _maxOther;
			for (auto& _span : spans)
			{
				// Get size from sun-span settings
				float& _sizeMain = _span.settings.size.*_sizeMainsp;
				float& _sizeOther = _span.settings.size.*_sizeOthersp;

				// If there is a ratio, add to ratio, otherwise to max explicit size
				if (_span.settings.ratio > 0)
					_maxRatio += _span.settings.ratio;
				else if (!_span.component || _span.settings.size.*_sizeMainsp != -1)
					_maxExplicitSize += _sizeMain;
				else
					_maxExplicitSize += _span.component->size.*_sizeMainsp;

				// Retrieve max size
				if (_maxSubSize < _sizeOther)
					_maxSubSize = _sizeOther;
			}

			// Constrain explicit size to prevent size inversion
			float _overflowMult = 1;
			if (_maxMain - _maxExplicitSize < 0 && _maxExplicitSize > 0)
			{
				_overflowMult = _maxMain / _maxExplicitSize;
				_maxExplicitSize = _maxMain;
			}

			switch (settings.overflow)
			{
			case Overflow::Free:
			{
				for (auto& _span : spans)
					_span.Layout();
			}
			case Overflow::Fit:
			{
				switch(settings.direction)
				{
				case Direction::Row:
				case Direction::Column:
				{
					float _position = _startMain;

					// Go through all sub-spans
					for (auto& _span : spans)
					{
						// Retrieve their axis and sizes.
						float& _axisMain = (_span).*_axisMainp;
						float& _axisOther = (_span).*_axisOtherp;
						float& _sizeMain = (_span).*_sizeMainp;
						float& _sizeOther = (_span).*_sizeOtherp;

						// Use ratio, or defined size. Use overflow multiplier to prevent overflow.
						if (_span.settings.ratio > 0)
							_sizeMain = (_maxMain - _maxExplicitSize) * _span.settings.ratio / _maxRatio;
						else if (!_span.component || _span.settings.size.*_sizeMainsp != -1)
							_sizeMain = _span.settings.size.*_sizeMainsp * _overflowMult;
						else
							_sizeMain = _span.component->size.*_sizeMainsp;

						// If no size defined for other axis
						if (_span.settings.size.*_sizeOthersp == -1)
							_sizeOther = _variableOther ? _maxSubSize : _maxOther;

						// Otherwise set to defined size for sub-span
						else
							_sizeOther = _span.settings.size.*_sizeOthersp;

						_axisMain = _position;
						_axisOther = _startOther;

						_position += _sizeMain;

						_span.Layout();
					}
					break;
				}
				case Direction::RowReverse:
				case Direction::ColumnReverse:
				{
					
					break;
				}
				}
				break;
			}
			case Overflow::Hidden: break;
			case Overflow::Wrap: break;
			case Overflow::WrapReverse: break;
			case Overflow::Scroll: break;
			}


















			// Determine max size:
			//  - If defined
			//  - Otherwise variable size

			// Determine axis

			// If variable size in 'direction' axis:
			//  - Fit: fills space of parent, uses ratio to determine sub-span sizes
			//  - Hidden: Fit. If no ratio, and sizes of sub-spans overflow: hidden
			//  - Wrap: Fit. If no ratio, and sizes of sub-spans overflow: wrap
			//  - WrapReverse: Wrap
			//  - Scroll: Hidden, but scroll instead.

			// If variable size in other 'direction':
			//  - Fit: Uses sub-span size, if not defined: use biggest sub-span size
			//  - Hidden: Fit, overflow hidden
			//  - Wrap: Fit, but when wrapping, biggest size resets
			//  - WrapReverse: Wrap
			//  - Scroll: Fit

			// If fixed size in 'direction' axis:
			//  - Fit: Uses ratio to determine sub-span sizes
			//  - Hidden: Fit. If no ratio, and sizes of sub-spans overflow: hidden
			//  - Wrap: Fit. If no ratio, and sizes of sub-spans overflow: wrap
			//  - WrapReverse: Wrap
			//  - Scroll: Hidden, but scroll instead.

			// If fixed size in other 'direction':
			//  - Fit: Uses sub-span size. If no sub-span size, fill, overflow is not hidden.
			//  - Hidden: Fit. Overflow hidden
			//  - Wrap: Fit.
			//  - WrapReverse: Fit.
			//  - Scroll: Fit, when overflow: scrollbar.




		}
	}

	void Span::ForwardRender(CommandCollection& d)
	{
		d.Fill(settings.background);
		d.Quad(dimensions);
		
		if (component)
			component->ForwardRender(d);

		std::list<Span*> _spans;
		for (auto& _span : spans)
			_spans.push_back(&_span);

		_spans.sort([](Span* a, Span* b) -> bool { return a->settings.zIndex < b->settings.zIndex; });

		for (auto& _span : _spans)
			_span->ForwardRender(d);
	}
}