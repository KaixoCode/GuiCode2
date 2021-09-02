#pragma once
#include "GuiCode2/Component.hpp"
namespace GuiCode
{	
	struct Align
	{
		enum {
			Left = 0x0000,    // aligned to left on x-axis
			Right = 0x0001,   // aligned to right on x-axis
			CenterX = 0x0002, // aligned to center on x-axis
			Top = 0x0000,     // aligned to top on y-axis
			Bottom = 0x0010,  // aligned to bottom on y-axis
			CenterY = 0x0020, // aligned to center on y-axis
			Center = 0x0022   // aligned to center on both axis
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
			int id = NextId();                 // Unique Id
			float ratio = 0;                   // Ratio used to determine size in parent Span
			int layout = Layout::Row;          // Type of layout
			int overflow = Overflow::Show;     // Overflow
			Vec4<float> padding{ 0, 0, 0, 0 }; // Padding for all sides
			Vec4<float> margin{ 0, 0, 0, 0 };  // Margin for all sides
			struct Border
			{
				struct Side
				{
					float width = 0;
					Color color{ 0, 0, 0, 0 };
				};
	
				Side left; 					   // Border can be picked
				Side right; 				   // for individual sides
				Side top; 					   // or for the entire border
				Side bottom;				   // at once. If side is specified
				float width = 0;			   // it will override width/color
				Color color{ 0, 0, 0, 0 };	   // specified for entire border
			} border{};
			float zIndex = 0;				   // zIndex for this Span in parent Span.
			Vec2<float> size{ -1, -1 };		   // prefered size, will be constrained to min/max
			Vec2<float> min{ -1, -1 };		   // minimum size for this Span
			Vec2<float> max{ -1, -1 };		   // maximum size for this Span
			int align = Align::Center;		   // alignment of span in area given by parent
			Color background{ 0, 0, 0, 0 };	   // background color of span
		};

		Span()
		{ Init(); }

		Span(const Settings& s)
			: settings(s)
		{ Init(); }

		Span(const Settings& s, const std::list<Span>& d)
			: settings(s), spans(d)
		{ Init(); }

		Span(const Settings& s, Component& c)
			: settings(s), component(&c)
		{ Init(); }

		Settings settings{};
		Component* component = nullptr;
		std::list<Span> spans{};

		struct { Scrollbar x, y; } scrollbar;

		/**
		 * Set the dimensions given the rectangle that's made available by parent Span.
		 * this will account for border and margin. It returns the full size including
		 * the margin and border, so the parent can account for the full dimensions.
		 */
		Vec2<float> SetDimensions(const Vec4<float>& newDims);
		Vec4<float> Offsets();
		Span* Find(int id);
		void RefreshLayout();

		void ForwardRender(CommandCollection&) override;
		void ForwardUpdate() override;

		bool Hitbox(const Vec2<float>& v) const override;
		void ConstrainSize() override;

	private:
		void Init()
		{
			components.push_back(&scrollbar.x);
			components.push_back(&scrollbar.y);
		}
	};
}