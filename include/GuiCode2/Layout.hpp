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
			XCenter = 0x0002, // Content aligned to center on x-axis
			Top = 0x0000,     // Content aligned to top on y-axis
			Bottom = 0x0010,  // Content aligned to bottom on y-axis
			YCenter = 0x0020, // Content aligned to center on y-axis
			Center = 0x0022   // Content aligned to center on both axis
		};
	};

	struct Direction
	{
		enum
		{
			Row,          // Content is left to right on x-axis
			RowReverse,   // Content is right to left on x-axis
			Column,       // Content is top to bottom on y-axis
			ColumnReverse // Content is bottom to top on y-axis
		};
	};

	struct Overflow
	{
		enum
		{
			Free,        // Content is free to be anywhere
			Fit,         // Makes sure content fits, overflow not hidden
			Hidden,      // Hides overflowing content
			Wrap,        // Wraps content
			WrapReverse, // Wraps content, but in reverse
			Scroll       // Adds scrollbar when overflows
		};
	};

	class Span : public Dimensions
	{
		static inline int id = 0;
	public:
		static inline int NextId() { return id++; }

		struct Settings
		{
			int id = NextId();
			float ratio = 1;
			int direction = Direction::Row;
			int overflow = Overflow::Fit;
			Vec4<float> padding{ 0, 0, 0, 0 };
			Vec4<float> margin{ 0, 0, 0, 0 };
			struct
			{
				struct B
				{
					float width = -1;
					Color color;
				};

				B left; B right; B top; B bottom;
				float width = -1;
				Color color;
			} border;
			float zIndex = 0;
			Vec2<float> size{ -1, -1 };
			Vec2<float> min{ -1, -1 };
			Vec2<float> max{ -1, -1 };
			int align = Align::Center;
			Color background{ 0, 0, 0, 0 };
		};

		Span()
		{}

		Span(const Settings& s)
			: settings(s)
		{}

		Span(const std::list<Span>& d)
			: spans(d)
		{}

		Span(const Settings& s, const std::list<Span>& d)
			: settings(s), spans(d)
		{}

		Span(const Settings& s, Component& c)
			: settings(s), component(&c)
		{}

		Span(Component& c)
			: component(&c)
		{}

		Settings settings;
		Component* component = nullptr;
		std::list<Span> spans;

		Span* Find(int id);
		void Layout();
		void ForwardRender(CommandCollection&);
	};
}