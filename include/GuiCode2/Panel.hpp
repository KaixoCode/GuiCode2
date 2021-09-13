#pragma once
#include "GuiCode2/Component.hpp"
#include "GuiCode2/BasicEvents.hpp"
#include "GuiCode2/StateColors.hpp"
#include "GuiCode2/Scrollbar.hpp"

namespace GuiCode
{
	enum class Layout
	{
		Free,         // not aligned
		Grid,         // grid aligned
		Row,          // left to right on x-axis
		RowReverse,   // right to left on x-axis
		Column,       // top to bottom on y-axis
		ColumnReverse // bottom to top on y-axis
	};

	enum class Overflow
	{
		Show,        // Overflowing content is visible
		Hide,        // Hides overflowing content
		Scroll       // Adds scrollbar when overflows
	};

	enum Sizing
	{
		None = -1,   // No min/max, can't be used with normal size.
		Auto = -2,   // Automatically size to the content. Min/max will be taken from content
		Inherit = -3 // Give parent full control over the size. Min/max will be parent size
	};

	struct Border
	{
		struct Side
		{
			float width = 0;
			Color color;
		};

		float width = 0;		   // Border can be picked
		Color color;               // for individual sides
		Side left; 				   // or for the entire border
		Side right; 			   // at once. If side is specified
		Side top; 				   // it will override width/color
		Side bottom;			   // specified for entire border
	};

	/**
	 * A Panel contains settings for creating scroll areas
	 * and complex layouts.
	 */
	class Panel : public Component
	{
	public:
		/**
		 * Panel id class, generates unique id to be used in a panel.
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
		 * Panel settings, contains visual settings like padding/margin
		 * size information, align information, an id for lookup, etc.
		 */
		struct Settings
		{
			Id id;                              // Unique Id
			float ratio = 0;                    // Ratio used to determine size in parent Panel
			Layout layout = Layout::Row;        // Type of layout
			struct _
			{
				_(Overflow o) : x(o), y(o) {}
				_(Overflow x, Overflow y) : x(x), y(y) {}
				Overflow x; // Overflow
				Overflow y; // Overflow
			} overflow = Overflow::Show;
			Vec4<float> padding{ 0, 0, 0, 0 }; // Padding for all sides
			Vec4<float> margin{ 0, 0, 0, 0 };  // Margin for all sides
			Border border;
			float zIndex = 0;				      // zIndex for this Panel in parent Panel.
			Vec2<float> size{ Inherit, Inherit }; // prefered size, will be constrained to min/max
			Vec2<float> min{ Inherit, Inherit };        // minimum size for this Panel
			Vec2<float> max{ Inherit, Inherit };        // maximum size for this Panel
			int align = Align::Top | Align::Left; // alignment of panel in area given by parent
			Color background{ 0, 0, 0, 0 };	      // background color of panel
		};

		Panel();
		Panel(const Settings& s);
		Panel(const Settings& s, const std::list<Panel>& d);
		Panel(const Settings& s, const Pointer<Component>&& c);
		Panel(const Panel& other);
		Panel& operator=(const Panel& other);

		/**
		 * Set the dimensions of this panel using the rectangle. This function
		 * will shave off the margin and border, so the entire panel will fit
		 * in the rectangle provided by the caller. It might not fill the entire
		 * rectangle if some maximum size is provided, which is why it returns its size.
		 * @return the new size this panel occupies.
		 */
		Vec2<float> SetDimensions(const Vec4<float>& rect);

		/**
		 * Get the offsets of the panel, this is the margin + border, since
		 * that is not included in the dimensions of a panel.
		 * @return offsets
		 */
		Vec4<float> Offsets();

		/**
		 * Find a panel with the given id. Also searches in sub-panels.
		 * @param id id
		 * @return panel or nullptr if no panel found
		 */
		Panel* Find(Id& id);

		/**
		 * Recalculates the layout.
		 */
		void RefreshLayout();

		Settings settings;
		Pointer<Component> component;
		struct Panels
		{
			Panels(Panel& me, const std::list<Panel>& data = {});

			Panel& Emplace(Panel&& panel);

			void Remove(int index);
			void Remove(Panel& panel);
			void Clear();

			auto begin() { return data.begin(); }
			auto end() { return data.end(); }
			auto Size() const { return data.size(); }

		private:
			Panel& me;
			std::list<Panel> data;

			friend class Panel;
		} panels{ *this };
		struct { Scrollbar x, y; } scrollbar;

		void ForwardRender(CommandCollection&) override;
		void ForwardUpdate() override;
		void ConstrainSize() override;

	private:
		Vec4<float> m_Viewport; // Actual dimensions of the used space in this panel (used by scrollbars)

		void Init();
		void RefreshScrollbars();

		void RenderBorder(CommandCollection&);

		void RowLayout(const Vec4<float>& content);
		void ColumnLayout(const Vec4<float>& content);
	};
}