#pragma once
#include "GuiCode2/Component.hpp"
namespace GuiCode
{
	enum Align
	{
		Left    = 0x0000,
		Right   = 0x0001,
		XCenter = 0x0002,
		Top     = 0x0000,
		Bottom  = 0x0010,
		YCenter = 0x0020,

		Horizontal = 0x0000,
		Vertical   = 0x0100,
	};

	enum Layout
	{
		Fit,   // Resizes Div to size of biggest containing component
		Fill,  // Resizes all components to the dimension specified in the div
		Stack, // Stacks all components with their own size
	};

	class Div
	{
		static inline int id = 0;
	public:
		static inline int NextId() { return id++; }

		struct Settings
		{
			int id = NextId();
			int align = Vertical;
			Vec2<Layout> layout{ Fill, Fill };
			Dimensions dimensions{ 0, 0, 1, 1 };
		};

		Div()
		{}

		Div(const Settings& s)
			: settings(s)
		{}

		Div(const std::vector<Div>& d)
			: divs(d)
		{}

		Div(const Settings& s, const std::vector<Div>& d)
			: settings(s), divs(d)
		{}

		Div(const Settings& s, Component& c)
			: settings(s), component(&c)
		{}

		Div(Component& c)
			: component(&c)
		{}

		Settings settings;
		Component* component = nullptr;
		std::vector<Div> divs;

		Div* Find(int id);
		void Layout();
	};
}