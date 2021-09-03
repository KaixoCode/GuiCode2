#pragma once
#include "GuiCode2/Component.hpp"
#include "GuiCode2/BasicEvents.hpp"

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
			Hide,        // Hides overflowing content
			Scroll       // Adds scrollbar when overflows
		};
	};

	class Span : public Component
	{
	public:

		/**
		 * Very simple scrollbar class for overflow: Scroll.
		 */
		class Scrollbar : public Component
		{
		public:
			static inline bool vertical = true;
			static inline bool horizontal = false;

			Scrollbar();

			bool Necessary() const;
			void Update() override;
			void Render(CommandCollection& d) const override;

			float visibleRange; // Visible range on the screen, used to calculate bar width
			float pvalue = 0;   // value when mousepressed
			float value = 0;    // value
			float min = 0;      // actual minimum coordinate
			float max = 0;      // actual maximum coordinate
			bool axis = false;  // axis == true means vertical, otherwise horizontal
			bool dragging = false; // true when mouse was pressed on bar, until mouse release

			Color background{ 96, 96, 96 };
		};

		/**
		 * Span id class, generates unique id to be used in a span.
		 */
		struct Id
		{
			Id();
			Id(const Id& other);

			Id& operator=(const Id& other);
			bool operator==(const Id& other) const { return other.m_Id == m_Id; }
			
		private:
			static inline int m_Counter = 0;
			int m_Id;
			mutable bool m_Used = false;
		};

		/**
		 * Span settings, contains visual settings like padding/margin
		 * size information, align information, an id for lookup, etc.
		 */
		struct Settings
		{
			Id id;                             // Unique Id
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

		Span();
		Span(const Settings& s);
		Span(const Settings& s, const std::list<Span>& d);
		Span(const Settings& s, Component& c);
		Span(const Span& other);

		Span& operator=(const Span& other);

		/**
		 * Set the dimensions of this span using the rectangle. This function
		 * will shave off the margin and border, so the entire span will fit
		 * in the rectangle provided by the caller. It might not fill the entire
		 * rectangle if some maximum size is provided, which is why it returns its size.
		 * @return the new size this span occupies.
		 */
		Vec2<float> SetDimensions(const Vec4<float>& rect);

		/**
		 * Get the offsets of the span, this is the margin + border, since
		 * that is not included in the dimensions of a span.
		 * @return offsets
		 */
		Vec4<float> Offsets();

		/**
		 * Find a span with the given id. Also searches in sub-spans. 
		 * @param id id
		 * @return span or nullptr if no span found
		 */
		Span* Find(Id& id);

		/**
		 * Recalculates all the sizes.
		 */
		void RefreshLayout();

		void ForwardRender(CommandCollection&) override;
		void ForwardUpdate() override;
		void ConstrainSize() override;

		Settings settings{};
		Component* component = nullptr;
		std::list<Span> spans{};
		struct { Scrollbar x, y; } scrollbar;

	private:
		Vec4<float> m_Viewport; // Actual size of the span (used by scrollbars)

		void Init();
	};
}