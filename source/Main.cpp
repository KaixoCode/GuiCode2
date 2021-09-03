#include "GuiCode2/pch.hpp"

#include "GuiCode2/Component.hpp"
#include "GuiCode2/BasicEvents.hpp"
#include "GuiCode2/Panel.hpp"
#include "GuiCode2/Layout.hpp"
#include "GuiCode2/Frame.hpp"

using namespace GuiCode;

/*
 * aspects:
 *  - Events
 *  - Layouts
 *  - Scrolling
 *  - Graphics
 *  - Windows
 * 
 *  - Need to know which object the mouse is over.
 *  - Need to know the viewport of each object.
 * 
 * Your object -> Register for events in other object.
 * 
 * ideas:
 *  - ability to add own events
 * 
 * 
 * 
 * Layout
 *  
 * 
 * 
 * 
 */

class Apple : public Component
{
public:
	Apple(int ids)
		: id(ids)
	{
		listener += [this](const Focus& e)
		{
			std::cout << "Focus!" << id << std::endl;
		};

		listener += [this](const Unfocus& e)
		{
			std::cout << "Unfocus!" << id << std::endl;
		};

		listener += [this](const MouseEnter& e)
		{
			std::cout << "Enter!" << id << std::endl;
		};

		listener += [this](const MouseExit& e)
		{
			std::cout << "Exit!" << id << std::endl;
		};

		listener += [this](const MousePress& e)
		{
			press = e.pos;
			std::cout << "Press!" << id << std::endl;
		};
		
		listener += [this](const MouseClick& e)
		{
			std::cout << "Click!" << id << std::endl;
		};

		listener += [this](const MouseRelease& e)
		{
			zIndex = 0;
			std::cout << "Release!" << id << std::endl;
		};

		listener += [this](const MouseMove& e)
		{
			//std::cout << "Move!" << id << std::endl;
		};

		listener += [this](const MouseDrag& e)
		{
			if (press.x > x + width - 10 && press.y > y + height - 10)
			{
				size = e.pos - position;
				press = e.pos;
			}
			else {
				position += e.pos - press;
				press = e.pos;
				zIndex = 10;
			}
			//std::cout << "Drag!" << id << std::endl;
		};
	}

	void Render(CommandCollection& d) const override
	{
		if (State<Focused>())
			d.Fill({ 0xFFD5B2 });
		else if (State<Pressed>())
			d.Fill({ 0x95D5B2 });
		else if (State<Hovering>())
			d.Fill({ 0xD8F3DC });
		else
			d.Fill({ 0xB7E4C7 });

		d.Quad(dimensions);

	}

	void Update() override
	{

	}

	Vec2<float> press;
	int id;
};

int main()
{

	Frame window{ {
		.name = "Hello", 
		.dimensions{ 500, 100, 500, 500 }, 
		.state = Show
	} };

	Apple _comp{1};
	Apple _comp2{2};
	Apple _comp3{3};
	Apple _comp4{4};
	Apple _comp5{5};
	Apple _comp6{6};
	Apple _comp7{7};
	Apple _comp8{8};
	Apple _comp9{9};
	Apple _comp10{10};
	Apple _comp11{11};
	Apple _comp12{12};

	_comp12.size = _comp11.size = _comp10.size = _comp9.size = _comp8.size = _comp7.size = _comp6.size = 
		_comp5.size = _comp4.size = _comp3.size = _comp2.size = _comp.size = { 50, 50 };

	window.titlebar.close.hover = { 0xed5b51 };
	window.titlebar.close.press = { 0xeb4034 };
	window.titlebar.minimize.hover = { 0x2D6A4F };
	window.titlebar.minimize.press = { 0x1B4332 };
	window.titlebar.maximize.hover = { 0x2D6A4F };
	window.titlebar.maximize.press = { 0x1B4332 };
	window.titlebar.color = { 0x40916C };
	window.background = { 0x40916C };

	Span::Id _scrollSpan1;
	Span::Id _scrollSpan2;

	window.panel.span = 
	{
		{
			.layout = Layout::Column,
			.overflow = Overflow::Hide,
			.padding{ 8, 8, 8, 8 },
			.margin{ 8, 8, 8, 8 },
			.background{ 0x52B788 }
		},
		{
			{
				{
					.id = _scrollSpan1,
					.ratio = 1,
					.layout = Layout::Row,
					.overflow = Overflow::Scroll,
					.padding{ 8, 8, 8, 8 },
					.margin{ 4, 4, 4, 4 },
					.background{ 0x74C69D }
				},
				{
					{ {.ratio = 0, .margin{ 4, 4, 4, 4 }, .min{ 110, -1 } }, _comp },
					{ {.ratio = 1, .margin{ 4, 4, 4, 4 }, .min{ 150, -1 } }, _comp2 },
					{ {.ratio = 2, .margin{ 4, 4, 4, 4 }, .min{ 180, -1 } }, _comp3 },
					{ {.ratio = 3, .margin{ 4, 4, 4, 4 }, .min{ 110, -1 } }, _comp4 },
					{ {.ratio = 3, .margin{ 4, 4, 4, 4 }, .min{ 120, -1 } }, _comp5 },
					{ {.ratio = 1, .margin{ 4, 4, 4, 4 }, .min{ 150, -1 } }, _comp6 },
				}
			},
			{
				{
					.id = _scrollSpan2,
					.ratio = 2,
					.layout = Layout::Row,
					.overflow = Overflow::Scroll,
					.padding{ 8, 8, 8, 8 },
					.margin{ 4, 4, 4, 4 },
					.background{ 0x74C69D }
				},
				{
					{ {.ratio = 1, .margin{ 4, 4, 4, 4 }, .min{ 120, -1 } }, _comp7 },
					{ {.ratio = 3, .margin{ 4, 4, 4, 4 }, .min{ 190, -1 } }, _comp8 },
					{ {.ratio = 2, .margin{ 4, 4, 4, 4 }, .min{ 180, -1 } }, _comp9 },
					{ {.ratio = 2, .margin{ 4, 4, 4, 4 }, .min{ 120, -1 } }, _comp10 },
					{ {.ratio = 1, .margin{ 4, 4, 4, 4 }, .min{ 150, -1 } }, _comp11 },
					{ {.ratio = 5, .margin{ 4, 4, 4, 4 }, .min{ 110, -1 } }, _comp12 },
				}
			},
		}
	};

	Span* _scroll1 = window.panel.span.Find(_scrollSpan1);

	_scroll1->scrollbar.x.background = { 0x4ba67c };
	_scroll1->scrollbar.x.bar.base = { 0x40916C };
	_scroll1->scrollbar.x.bar.State<Pressed>(0x327356);
	_scroll1->scrollbar.x.bar.State<Hovering>(0x3b8765);

	_scroll1->scrollbar.y.background = { 0x4ba67c };
	_scroll1->scrollbar.y.bar.base = { 0x40916C };
	_scroll1->scrollbar.y.bar.State<Pressed>(0x327356);
	_scroll1->scrollbar.y.bar.State<Hovering>(0x3b8765);

	Span* _scroll2 = window.panel.span.Find(_scrollSpan2);

	_scroll2->scrollbar.x.background = { 0x4ba67c };
	_scroll2->scrollbar.x.bar.base = { 0x40916C };
	_scroll2->scrollbar.x.bar.State<Pressed>(0x327356);
	_scroll2->scrollbar.x.bar.State<Hovering>(0x3b8765);
		   
	_scroll2->scrollbar.y.background = { 0x4ba67c };
	_scroll2->scrollbar.y.bar.base = { 0x40916C };
	_scroll2->scrollbar.y.bar.State<Pressed>(0x327356);
	_scroll2->scrollbar.y.bar.State<Hovering>(0x3b8765);

	while (window.Loop())
	{
		LIMIT_FPS(60);
	}
}