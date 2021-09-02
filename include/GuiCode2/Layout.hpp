#pragma once
#include "GuiCode2/Component.hpp"
namespace GuiCode
{	






	/**
	 * Layout properties:
	 *  - Direction: row, rowReverse, column, columnReverse
	 *  - Ratio: Calculates ratio based on value, 0 = use own size.
	 *  - Overflow: 
	 *     - Show: Shows overflow
	 *     - Hide: Hides overflow
	 *     - Scroll: Adds scrollbar
	 *  - Align: Alignment of result.
	 *     
	 * 
	 * 
	 * Properties:
	 *  - Id
	 *  - Direction
	 *  - Overflow
	 *  - Ratio
	 *  - Padding
	 *  - Margin
	 *  - Border
	 *  - zIndex
	 *  - size
	 *  - min size
	 *  - max size
	 *  - Alignment
	 *  - Background
	 * 
	 * 
	 * 
	 * 
	 */

	struct Align
	{
		enum {
			Left = 0x0000,    // Content aligned to left on x-axis
			Right = 0x0001,   // Content aligned to right on x-axis
			CenterX = 0x0002, // Content aligned to center on x-axis
			Top = 0x0000,     // Content aligned to top on y-axis
			Bottom = 0x0010,  // Content aligned to bottom on y-axis
			CenterY = 0x0020, // Content aligned to center on y-axis
			Center = 0x0022   // Content aligned to center on both axis
		};
	};

	struct Layout
	{
		enum
		{
			Free,         // not aligned
			Grid,         // grid aligned
			Row,          // left to right on x-axis
			RowReverse,   // right to left on x-axis
			Column,       // top to bottom on y-axis
			ColumnReverse // bottom to top on y-axis
		};
	};

	struct Overflow
	{
		enum
		{
			Show,        // Overflowing content is visible
			Hidden,      // Hides overflowing content
			Scroll       // Adds scrollbar when overflows
		};
	};

	class Span : public Component
	{
		class Scrollbar : public Component
		{
		public:
			float value;
			float min;
			float max;
			float realMin;
			float realMax;

		};

		static inline int id = 0;
	public:
		static inline int NextId() { return id++; }

		struct Settings
		{
			int id = NextId();
			float ratio = 0;
			int layout = Layout::Row;
			int overflow = Overflow::Show;
			Vec4<float> padding{ 0, 0, 0, 0 };
			Vec4<float> margin{ 0, 0, 0, 0 };
			struct Border
			{
				struct Side
				{
					float width = 0;
					Color color{ 0, 0, 0, 0 };
				};

				Side left; Side right; Side top; Side bottom;
				float width = 0;
				Color color{ 0, 0, 0, 0 };
			} border;
			float zIndex = 0;
			Vec2<float> size{ -1, -1 };
			Vec2<float> min{ -1, -1 };
			Vec2<float> max{ -1, -1 };
			int align = Align::Center;
			Color background{ 0, 0, 0, 0 };
		};

		Span()
		{
			Init();
		}

		Span(const Settings& s)
			: settings(s)
		{
			Init();
		}

		Span(const std::list<Span>& d)
			: spans(d)
		{
			Init();
		}

		Span(const Settings& s, const std::list<Span>& d)
			: settings(s), spans(d)
		{
			Init();
		}

		Span(const Settings& s, Component& c)
			: settings(s), component(&c)
		{
			Init();
		}

		Span(Component& c)
			: component(&c)
		{
			Init();
		}

		Span(Span&& other)
			: settings(other.settings), spans(other.spans),
			component(other.component)
		{}

		Span(const Span& other)
			: settings(other.settings), spans(other.spans),
			component(other.component)
		{}

		Span& operator=(Span&& other)
		{
			settings = other.settings;
			spans = other.spans;
			component = other.component;
			return *this;
		}

		Span& operator=(const Span& other)
		{
			settings = other.settings;
			spans = other.spans;
			component = other.component;
			return *this;
		}

		Settings settings;
		Component* component = nullptr;
		std::list<Span> spans;

		struct
		{
			Scrollbar x;
			Scrollbar y;
		} scrollbar;

		Span* Find(int id);
		void Layout();
		void ForwardRender(CommandCollection&) override;
		void ForwardUpdate() override;

		bool Hitbox(const Vec2<float>& v) const override
		{
			if (scrollbar.x.Hitbox(v) || scrollbar.y.Hitbox(v))
				return true;
	
			for (auto& _s : spans)
				if (_s.Hitbox(v))
					return true;

			return false;
		}

		/**
		 * Set the dimensions given the rectangle that's made available by parent Span.
		 * this will account for border and margin. It returns the full size including
		 * the margin and border, so the parent can account for the full dimensions.
		 */
		Vec2<float> SetDimensions(const Vec4<float>& newDims)
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

		inline Vec4<float> Offsets()
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

		void ConstrainSize() override
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

	private:
		void Init()
		{
			RegisterComponent(scrollbar.x);
			RegisterComponent(scrollbar.y);
		}
	};
}