#pragma once
#include "GuiCode2/Component.hpp"
namespace GuiCode
{
	struct Align
	{
		enum {
			Left = 0x0000,
			Right = 0x0001,
			XCenter = 0x0002,
			Top = 0x0000,
			Bottom = 0x0010,
			YCenter = 0x0020,

			Horizontal = 0x0000,
			Vertical = 0x0100,
		};
	};

	class Span
	{
		enum
		{
			Fit,   // Resizes Div to size of biggest containing component
			Fill,  // Resizes all components to the dimension specified in the div
			Stack, // Stacks all components with their own size
		};

		static inline int id = 0;
	public:
		static inline int NextId() { return id++; }

		struct Settings
		{
			int id = NextId();
			int align = Align::Vertical;
			Vec2<int> layout{ Fill, Fill };
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