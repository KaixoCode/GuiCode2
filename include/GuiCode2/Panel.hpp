#pragma once
#include "GuiCode2/Component.hpp"
#include "GuiCode2/BasicEvents.hpp"
#include "GuiCode2/StateColors.hpp"
#include "GuiCode2/Scrollbar.hpp"
#include "GuiCode2/Parser.hpp"

namespace GuiCode
{
	enum class Layout
	{
		//Free,         // not aligned
		//Grid,         // grid aligned
		Row,          // left to right on x-axis
		//RowReverse,   // right to left on x-axis
		Column,       // top to bottom on y-axis
		//ColumnReverse // bottom to top on y-axis
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
	class PanelParser;
	class Panel : public Component
	{
	public:
		/**
		 * Panel id class, generates unique id to be used in a panel.
		 */
		struct Settings;
		struct Id
		{
			Id();
			Id(const Id& other);

			Id& operator=(const Id& other);
			bool operator==(const Id& other) const { return other.m_Id == m_Id; }

			Id(int id) : m_Id(id) {};
		private:
			static inline int m_Counter = 0;
			int m_Id;
			friend class GuiCode::Panel::Settings;
		};

		/**
		 * Panel settings, contains visual settings like padding/margin
		 * size information, align information, an id for lookup, etc.
		 */
		struct Settings
		{
			Id id{ -1 };                        // Unique Id
			float ratio = 0;                    // Ratio used to determine size in parent Panel
			Layout layout = Layout::Row;        // Type of layout
			struct OverflowStruct
			{
				OverflowStruct(Overflow o) : x(o), y(o) {}
				OverflowStruct(Overflow x, Overflow y) : x(x), y(y) {}
				Overflow x; // Overflow
				Overflow y; // Overflow
			} overflow = Overflow::Show;
			Vec4<float> padding{ 0, 0, 0, 0 }; // Padding for all sides
			Vec4<float> margin{ 0, 0, 0, 0 };  // Margin for all sides
			Border border;
			float zIndex = 0;				      // zIndex for this Panel in parent Panel.
			Vec2<float> size{ Inherit, Inherit }; // prefered size, will be constrained to min/max
			Vec2<float> min{ Inherit, Inherit };  // minimum size for this Panel
			Vec2<float> max{ Inherit, Inherit };  // maximum size for this Panel
			int align = Align::Top | Align::Left; // alignment of panel in area given by parent
			Color background{ 0, 0, 0, 0 };	      // background color of panel
		};

		Panel();
		Panel(const Settings& s);
		Panel(const Settings& s, const std::list<Pointer<Panel>>& d);
		Panel(const Settings& s, const Pointer<Component>& c);
		Panel(Panel&& other);
		Panel(const Panel& other) = delete;
		Panel& operator=(Panel&& other);
		Panel& operator=(const Panel& other) = delete;

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
		Panel* Find(const Id& id);

		/**
		 * Recalculates the layout.
		 */
		void RefreshLayout();

		Settings settings;
		Pointer<Component> component;
		struct Panels
		{
			using iterator = std::list<Pointer<Panel>>::iterator;
			using value_type = Pointer<Panel>;
			using pointer = Panel*;
			using reference = Panel&;

			Panels(Panel& me, const std::list<Pointer<Panel>>& data = {});

			Panel& push_back(const Pointer<Panel>& panel);
			void pop_back();

			void remove(int index);
			void remove(Panel& panel);
			void clear();

			auto begin() { return data.begin(); }
			auto end() { return data.end(); }
			auto begin() const { return data.begin(); }
			auto end() const { return data.end(); }
			auto size() const { return data.size(); }

		private:
			Panel& me;
			std::list<Pointer<Panel>> data;

			friend class Panel;
		} panels{ *this };
		struct { Scrollbar x, y; } scrollbar;

		void ForwardRender(CommandCollection&) override;
		void ForwardUpdate() override;
		void ConstrainSize() override;

		void Init() override;

		Id& id = settings.id;
		float& ratio = settings.ratio;
		Layout& layout = settings.layout;
		Settings::OverflowStruct& overflow = settings.overflow;
		Vec4<float>& padding = settings.padding;
		Vec4<float>& margin = settings.margin;
		Border& border = settings.border;
		int& align = settings.align;
		Color& background = settings.background;

	private:
		Vec4<float> m_Viewport; // Actual dimensions of the used space in this panel (used by scrollbars)

		void RefreshScrollbars();
		void RenderBorder(CommandCollection&);

		void RowLayout(Vec4<float>& content);
		void ColumnLayout(Vec4<float>& content);

		Ref<Id> m_Id = settings.id;
		Ref<float> m_Ratio = settings.ratio;
		Ref<Layout> m_Layout = settings.layout;
		Ref<Settings::OverflowStruct> m_Overflow = settings.overflow;
		Ref<Vec4<float>> m_Padding = settings.padding;
		Ref<Vec4<float>> m_Margin = settings.margin;
		Ref<Border> m_Border = settings.border;
		Ref<float> m_ZIndex = settings.zIndex;
		Ref<Vec2<float>> m_Size = settings.size;
		Ref<Vec2<float>> m_Min = settings.min;
		Ref<Vec2<float>> m_Max = settings.max;
		Ref<int> m_Align = settings.align;
		Ref<Color> m_Background = settings.background;
		friend class PanelParser;
	};

	class PanelParser : public TagParser
	{
	public:
		PanelParser()
		{
			settings.name = "panel";
			Attribute("id", &Panel::m_Id);
			Attribute("ratio", &Panel::m_Ratio);
			Attribute("layout", &Panel::m_Layout);
			Attribute("overflow", &Panel::m_Overflow);
			Attribute("overflow.x", &Panel::Settings::OverflowStruct::x);
			Attribute("overflow.y", &Panel::Settings::OverflowStruct::y);
			Attribute("padding", &Panel::m_Padding);
			Attribute("margin", &Panel::m_Margin);
			Attribute("border", &Panel::m_Border);
			Attribute("border.width", &Border::width);
			Attribute("border.color", &Border::color);
			Attribute("border.left", &Border::left);
			Attribute("border.left.width", &Border::Side::width);
			Attribute("border.left.color", &Border::Side::color);
			Attribute("border.right", &Border::right);
			Attribute("border.right.width", &Border::Side::width);
			Attribute("border.right.color", &Border::Side::color);
			Attribute("border.top", &Border::top);
			Attribute("border.top.width", &Border::Side::width);
			Attribute("border.top.color", &Border::Side::color);
			Attribute("border.bottom", &Border::bottom);
			Attribute("border.bottom.width", &Border::Side::width);
			Attribute("border.bottom.color", &Border::Side::color);
			Attribute("z-index", &Panel::m_ZIndex);
			Attribute("size", &Panel::m_Size);
			Attribute("size.width", &Vec2<float>::width);
			Attribute("size.height", &Vec2<float>::height);
			Attribute("min", &Panel::m_Min);
			Attribute("min.width", &Vec2<float>::width);
			Attribute("min.height", &Vec2<float>::height);
			Attribute("max", &Panel::m_Max);
			Attribute("max.width", &Vec2<float>::width);
			Attribute("max.height", &Vec2<float>::height);
			Attribute("align", &Panel::m_Align);
			Attribute("background", &Panel::m_Background);

			enumMap["row"] = (int)Layout::Row;
			enumMap["column"] = (int)Layout::Column;
			enumMap["show"] = (int)Overflow::Show;
			enumMap["hide"] = (int)Overflow::Hide;
			enumMap["scroll"] = (int)Overflow::Scroll;
			enumMap["none"] = (int)Sizing::None;
			enumMap["auto"] = (int)Sizing::Auto;
			enumMap["inherit"] = (int)Sizing::Inherit;
			enumMap["left"] = (int)Align::Left;
			enumMap["right"] = (int)Align::Right;
			enumMap["center-x"] = (int)Align::CenterX;
			enumMap["top"] = (int)Align::Top;
			enumMap["bottom"] = (int)Align::Bottom;
			enumMap["center-y"] = (int)Align::CenterY;
			enumMap["center"] = (int)Align::Center;
			enumMap["text-top"] = (int)Align::TextTop;
			enumMap["text-bottom"] = (int)Align::TextBottom;
			enumMap["baseline"] = (int)Align::Baseline;
			enumMap["middle"] = (int)Align::Middle;
		}

		Pointer<Component> Create() override
		{
			return new Panel{};
		};

		void Append(Component& c, Pointer<Component>&& obj) override
		{
			Panel* _t = dynamic_cast<Panel*>(&c);
			Panel* _other = obj; // Dynamic casts to Panel
			if (_t && _other)
			{
				_t->panels.push_back(std::move(obj));
			}
			else
			{
				_t->panels.push_back(new Panel{ {.size{ Auto, Auto }}, std::move(obj) });
			}
		}
	};
}