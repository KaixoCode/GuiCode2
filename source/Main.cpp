#include "GuiCode2/pch.hpp"

#include "GuiCode2/Component.hpp"
#include "GuiCode2/BasicEvents.hpp"
#include "GuiCode2/Panel.hpp"
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
			if (e.pos.x > x + width - 10)
				if (e.pos.y > y + height - 10)
					dragging = true;
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
			if (dragging)
				size = e.pos - position;

			

			//else {
			//	position += e.pos - press;
			//	press = e.pos;
			//	zIndex = 10;
			//}
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
	bool dragging = false;
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

	Panel::Id _scrollpanel1;
	Panel::Id _scrollpanel2;
	Panel::Id _scrollpanel3;

	window.panel = 
	{
		{
			.layout = Layout::Row,
			.overflow = Overflow::Hide,
			.padding{ 8, 8, 8, 8 },
			.margin{ 8, 8, 8, 8 },
			.background{ 0x52B788 }
		},
		{
			{
				{
					.id = _scrollpanel1,
					.ratio = 1,
					.layout = Layout::Row,
					.overflow = Overflow::Scroll,
					.padding{ 8, 8, 8, 8 },
					.margin{ 4, 4, 4, 4 },
					.background{ 0x74C69D }
				},
				{
					{ {.ratio = 0, .margin{ 4, 4, 4, 4 }, .size{ Auto, Auto }, .min{ 110, -1 } }, _comp4 },
					{ {.ratio = 0, .margin{ 4, 4, 4, 4 }, .size{ Auto, Auto }, .min{ 120, -1 } }, _comp5 },
					{ {.ratio = 0, .margin{ 4, 4, 4, 4 }, .size{ Auto, Auto }, .min{ 150, -1 } }, _comp6 },
					{ {.ratio = 0, .margin{ 4, 4, 4, 4 }, .size{ Auto, Auto }, .min{ 190, -1 } }, _comp8 },
				}
			},
			{
				{
					.id = _scrollpanel2,
					.ratio = 1,
					.layout = Layout::Column,
					.overflow = Overflow::Scroll,
					.padding{ 8, 8, 8, 8 },
					.margin{ 4, 4, 4, 4 },
					.background{ 0x74C69D }
				},
				{
					{
						{
							.id = _scrollpanel3,
							.ratio = 5,
							.layout = Layout::Column,
							.overflow = Overflow::Scroll,
							.padding{ 8, 8, 8, 8 },
							.margin{ 4, 4, 4, 4 },
							.border{ 4, 0x52B788 },
							.min{ -1, 300 },
							.background{ 0x74C69D }
						},
						{
							{ {.ratio = 0, .margin{ 4, 4, 4, 4 }, .size{ Auto, Auto }, .min{ -1, 110 }, .align = Align::Center  }, _comp },
							{ {.ratio = 0, .margin{ 4, 4, 4, 4 }, .size{ Auto, Auto }, .min{ -1, 150 }, .align = Align::Right }, _comp2 },
							{ {.ratio = 0, .margin{ 4, 4, 4, 4 }, .size{ Auto, Auto }, .min{ -1, 180 }, .align = Align::Left }, _comp3 },
							{ {.ratio = 0, .margin{ 4, 4, 4, 4 }, .size{ Auto, Auto }, .min{ -1, 120 }, .align = Align::Right }, _comp7 },
						}
					},
					{ {.ratio = 0, .margin{ 4, 4, 4, 4 }, .size{ Auto, Auto }, .min{ -1, 180 }, .align = Align::Left }, _comp9 },
					{ {.ratio = 0, .margin{ 4, 4, 4, 4 }, .size{ Auto, Auto }, .min{ -1, 120 }, .align = Align::Right }, _comp10 },
					{ {.ratio = 0, .margin{ 4, 4, 4, 4 }, .size{ Auto, Auto }, .min{ -1, 150 }, .align = Align::Center }, _comp11 },
					{ {.ratio = 0, .margin{ 4, 4, 4, 4 }, .size{ Auto, Auto }, .min{ -1, 110 }, .align = Align::Right }, _comp12 },
				}
			},
		}
	};

	Panel* _scroll1 = window.panel.Find(_scrollpanel1);
	Panel* _scroll2 = window.panel.Find(_scrollpanel2);
	Panel* _scroll3 = window.panel.Find(_scrollpanel3);

	_scroll1->scrollbar.x.background =
	_scroll1->scrollbar.y.background =
	_scroll2->scrollbar.x.background =
	_scroll2->scrollbar.y.background =
	_scroll3->scrollbar.x.background =
	_scroll3->scrollbar.y.background = { 0x4ba67c };
	_scroll1->scrollbar.x.bar.base = { 0x40916C };
	_scroll1->scrollbar.x.bar.State<Pressed>(0x327356);
	_scroll1->scrollbar.x.bar.State<Hovering>(0x3b8765);
	_scroll1->scrollbar.y.bar = 
	_scroll2->scrollbar.x.bar = 
	_scroll2->scrollbar.y.bar = 
	_scroll3->scrollbar.x.bar = 
	_scroll3->scrollbar.y.bar = _scroll1->scrollbar.x.bar;


	while (window.Loop())
	{
		//LIMIT_FPS(60);
		MEASURE_FPS;
	}
}