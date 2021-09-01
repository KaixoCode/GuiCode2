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
			std::cout << "Move!" << id << std::endl;
		};

		listener += [this](const MouseDrag& e)
		{
			if (press.x > x + width - 10)
			{
				width = e.pos.x - x;
				press = e.pos;
			}
			std::cout << "Drag!" << id << std::endl;
		};
	}

	void Render(CommandCollection& d) const override
	{
		d.Fill({ 255, 255, 255, 255 });
		d.Quad(dimensions);

		if (State<Pressed>())
			d.Fill({ 200, 0, 0, 255 });
		else if (State<Hovering>())
			d.Fill({ 235, 0, 0, 255 });
		else
			d.Fill({ 255, 0, 0, 255 });

		d.Quad(dimensions - 1);

	}

	void Update() override
	{

	}

	Vec2<float> press;
	int id;
};

int main()
{
	constexpr int size = sizeof Span::Settings;

	Frame window{ {
		.name = "Hello", 
		.dimensions{ 500, 100, 500, 500 }, 
		.state = Show
	} };

	window.panel.State<UseDepth>(true);
	Apple& _comp = window.panel.Emplace<Apple>(1);
	Apple& _comp2 = window.panel.Emplace<Apple>(2);
	Apple& _comp3 = window.panel.Emplace<Apple>(3);
	Apple& _comp4 = window.panel.Emplace<Apple>(4);
	Apple& _comp5 = window.panel.Emplace<Apple>(5);
	Apple& _comp6 = window.panel.Emplace<Apple>(6);
	_comp6.size = _comp5.size = _comp4.size = _comp3.size = _comp2.size = _comp.size = { 50, 50 };

	_comp.zIndex = 1;
	_comp2.zIndex = 1;
	_comp2.zIndex = 1;

	Span::Settings _test
	{
		.direction = Direction::Row,
		.overflow = Overflow::Hidden,
		.padding = 
		{ 
			.left = 1, 
			.top = 1, 
			.right = 1,
			.bottom = 1 
		},
		.margin = 
		{ 
			.left = 1, 
		    .top = 1, 
			.right = 1, 
			.bottom = 1 
		},
		.border = 
		{
			.width = 1,
			.color{ 255, 255, 255, 255 } 
		},
		.zIndex = 1,
		.size = 
		{
			.width = 200, 
			.height = 200 
		},
	    .align = Align::Left | Align::Top,
		.background = 
		{
			.r = 0, 
			.g = 0, 
			.b = 0, 
			.a = 255 
		}
	};

	//window.panel.span =
	//	Span{ {.direction = Direction::Row },
	//	{
	//		Span{ {.ratio = 1,
	//			   .zIndex = 2,
	//			   .direction = Direction::Row,
	//			   .layout = Layout::Fit,
	//			   .padding{ 8, 8, 8, 8 },
	//			   .background{ 0, 0, 128, 255 } },
	//			{
	//				Span{ {.ratio = 0 }, _comp },
	//				Span{ {.ratio = 0 }, _comp2 },
	//				Span{ {.ratio = 0 }, _comp3 },
	//			}
	//		},
	//		Span{ {.ratio = 2,
	//			   .direction = Direction::Row,
	//			   .layout = Layout::Free,
	//			   .padding{ 8, 0, 8, 8 },
	//			   .background{ 0, 128, 0, 255 } },
	//			{
	//				Span{ {.ratio = 1,
	//					   .direction = Direction::Column,
	//					   .padding{ 0, 0, 8, 0 },
	//					   .background{ 0, 128, 128, 255 } },
	//					{
	//						Span{ {.ratio = 2 }, _comp4 },
	//						Span{ {.ratio = 1 }, _comp5 },
	//					}
	//				},
	//				Span{ {.ratio = 1 }, _comp6 },
	//			}
	//		}
	//	}
	//};

	while (window.Loop())
	{
		window.panel.span.Layout();

		Component* _hovering = window.panel.Get(Focused);

		if (_hovering)
			std::cout << _hovering << std::endl;

		LIMIT_FPS(60);
	}
}