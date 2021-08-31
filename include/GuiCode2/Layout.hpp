#pragma once
#include "GuiCode2/Component.hpp"
namespace GuiCode
{	






	/**
	 * Layout properties:
	 *  - Direction: row, rowReverse, column, columnReverse
	 *  - Ratio: Calculates ratio based on value, 0 = use own size.
	 *  - Overflow: 
	 *     - Fit: Make sure content fits
	 *     - Hidden: Hides overflow
	 *     - Wrap: Wraps overflow to next col/row
	 *     - WrapReverse: Wraps, but starts at
	 *     - Scroll: Adds scrollbar
	 *  - Align: Alignment of result.
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
			Fit,         // Makes sure content fits
			Hidden,      // Hides overflowing content
			Wrap,        // Wraps content
			WrapReverse, // Wraps content, but in reverse
			Scroll       // Adds scrollbar when overflows
		};
	};

	class Span
	{
		static inline int id = 0;
	public:
		static inline int NextId() { return id++; }

		struct Settings
		{
			int id = NextId();
			int align = Align::Center;
			int direction = Direction::Row;
			int overflow = Overflow::Hidden;
			Dimensions dimensions{ 0, 0, 1, 1 };
		};

		Span()
		{}

		Span(const Settings& s)
			: settings(s)
		{}

		Span(const std::vector<Span>& d)
			: divs(d)
		{}

		Span(const Settings& s, const std::vector<Span>& d)
			: settings(s), divs(d)
		{}

		Span(const Settings& s, Component& c)
			: settings(s), component(&c)
		{}

		Span(Component& c)
			: component(&c)
		{}

		Settings settings;
		Component* component = nullptr;
		std::vector<Span> divs;

		Span* Find(int id);
		void Layout();
	};
}